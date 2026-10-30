# Code Review of src

Dernière revue : 2026-10-30

## Périmètre

Revue complète de `src/driver/` (firmware ESP32, `driver.ino`), faite par lecture directe du code, sans sous-agent. Les fichiers `setting_secret_wifi.h` (ignoré par git) ne sont pas lus.

- Compilation : `arduino-cli compile --fqbn esp32:esp32:esp32 driver.ino` passe (vérifié le 2026-10-30, après les corrections des constats 8 à 16).
- Matériel : aucun test réalisé. Tous les constats « à vérifier » restent ouverts.
- Aucune modification du firmware pendant cette revue.

## Synthèse

| # | Sévérité | Statut | Sujet |
|---|----------|--------|-------|
| 1 | Haut | Corrigé | Support Uno/Nano retiré |
| 2 | Moyen | Résolu | État de commande verrouillé (STOP supprimé) |
| 3 | Moyen | Corrigé | Conflits de GPIO |
| 3 bis | Moyen | Partiel | LED d'état sur GPIO 2 (strapping) |
| 3 ter | Moyen | Résolu | Durée de pression STOP (supprimée) |
| 4 | Haut | Corrigé sur ESP32 | Commandes deploy/retract |
| 5 | Moyen | Partiel | `MotorState` |
| 6 | Ajout | Corrigé, à tester | Reset par maintien en mode manuel |
| 7 | Moyen | Ouvert | Logs TRACE dans le chemin de jog |
| 8 | Moyen | Corrigé | `resetHoldMs` non validé |
| 9 | Moyen | Corrigé (paramétrable) | Délai de 10 s au démarrage |
| 10 | Bas | Corrigé | Indicateur « waiting » affiché après le délai |
| 11 | Bas | Corrigé | Code mort (`Tracker::_state`, `_settingProgramMotors`, `Ldr::getPin`) |
| 12 | Bas | Corrigé | Garde `#if defined(HEARTBEAT_INTERVAL)` toujours vraie |
| 13 | Bas | Ouvert | LED tracker (GPIO 4) configurée mais jamais pilotée |
| 14 | Bas | Corrigé | Accès direct aux membres publics de `Ldrs` |
| 15 | Bas | Corrigé | Tableau de broches de taille fixe sans contrôle de bornes |
| 16 | Bas | Corrigé | Journaux de configuration incorrects ou incomplets |
| 17 | Bas | À surveiller | Journal `ArduinoLog` écrit depuis deux cœurs |
| 18 | Bas | Ouvert | JSON de `/status` : troncature silencieuse possible |
| 19 | Info | À confirmer | Régulation automatique par impulsions de 1 s ; mode nuit non implémenté |
| 20 | Bas | Corrigé | Commentaire d'en-tête de `driver.ino` obsolète (« Uno/Nano ») |
| 21 | Bas | Corrigé | SSID WiFi journalisé en TRACE, visible sur `/log` sans authentification |
| 22 | Bas | Corrigé | `Motors::state()` ne lit que le moteur 1 (champ `motor` de `/status`) |
| 23 | Bas | Accepté | `AppLogOutput::copyTo()` garde la section critique pendant la copie de 4096 octets |

## Constats

### 1. Haut : support Uno/Nano retiré (corrigé)

- Emplacement : `src/driver/setting.h`, `src/driver/setting_esp32.cpp`, `src/driver/driver.ino`, `src/driver/led_protocol.cpp`, `src/driver/driver.cpp`
- Problème initial : les implémentations Uno/Nano de `settingInit()` ne respectaient pas le contrat `Setting* settingInit()`.
- Correction : seule la carte ESP32 est supportée. Les fichiers `setting_uno.cpp` et `setting_nano.cpp` sont supprimés, et les gardes `BOARD_*` sont retirées.

### 2. Moyen : état de commande verrouillé sans retour à idle (résolu)

- Problème initial : les callbacks de maintien passaient l'état à `STOP` ou `RESET`, sans retour à `IDLE`.
- État : résolu par suppression du bouton STOP, de `DriverCommand`, `DriverCommandState` et de l'état `STOP`. Le seul état de reset restant (`RESET`) est volontairement terminal : il se termine par le redémarrage de l'ESP32.

### 3. Moyen : conflits de GPIO dans la table des broches (corrigé)

- Emplacement : `src/driver/setting_esp32.cpp`, `src/driver/setting.cpp`
- Problème initial : les broches 25, 26, 27 et 32 étaient utilisées deux fois.
- Correction : `assertSetting()` vérifie l'unicité de toutes les broches via `assertPinsUnique()`, y compris SDA/SCL de l'écran et la LED d'état. Le conflit est signalé avec les deux champs concernés.

### 3 bis. Moyen : broche de strapping pour la LED d'état (partiel)

- Emplacement : `src/driver/setting_esp32.cpp` (`setting->board.pin.led = 2`)
- Reste ouvert : GPIO 2 est une broche de strapping. Elle ne doit pas être tirée fortement au démarrage. Le choix est assumé (LED intégrée de la plupart des DevKit), mais le démarrage doit être vérifié sur le matériel.

### 3 ter. Moyen : durée de pression STOP (résolu)

- Problème initial : `SettingProgramCommandStop` (`pressMs`) était validé, alors que `DriverCommand` utilisait une constante fixe.
- État : résolu par suppression. La durée de maintien est désormais `program.tracker.command.resetHoldMs` (voir constat 8 pour sa validation).

### 4. Haut : commandes deploy/retract du tracker (corrigé sur ESP32)

- Emplacement : `src/driver/tracker_command.h`, `src/driver/tracker_command.cpp`, `src/driver/tracker_command_state.h`, `src/driver/tracker_command_state.cpp`, `src/driver/tracker.cpp`
- Règles de commande (en mode manuel uniquement) :
  - Deux boutons enfoncés en même temps : état `IDLE` (arrêt sûr). Si ce maintien dure `resetHoldMs`, l'état passe à `RESET` (constat 6).
  - Un seul bouton : `DEPLOY` ou `RETRACT`.
  - Au relâchement : `stop()`.
- Jog manuel : tant que le bouton est maintenu, `deploy()` ou `retract()` est relancé à chaque boucle. Le timeout moteur est réarmé à chaque appel, donc il ne coupe pas un mouvement maintenu. Aucune limite de course : le moteur tourne tant que le bouton est enfoncé.
- Sortie du mode manuel : `Tracker::handleCommand()` arrête le moteur s'il était en `DEPLOY` ou `RETRACT`, puis la commande passe à `AUTO`. Un bouton encore maintenu au retour en manuel est pris en compte.
- Le commutateur de mode est une bascule à deux positions : LOW (broche reliée à GND, pull-up interne) = manuel, HIGH = automatique. Il est débouncé avec `debounce`.
- À vérifier : la polarité du commutateur au câblage, et le comportement du jog (voir constat 7 sur la journalisation).

### 5. Moyen : état moteur ajouté (`MotorState`) (partiel)

- Emplacement : `src/driver/motor_state.h`, `src/driver/motor_state.cpp`, `src/driver/motor.h`, `src/driver/motor.cpp`, `src/driver/motors.cpp`, `src/driver/tracker.cpp`
- Problème initial : `Tracker` relançait `deploy()`/`retract()` à chaque boucle, sans état mémorisé.
- État actuel : `Motor` mémorise `IDLE`, `DEPLOY` ou `RETRACT`. `Tracker::AppStatus()` s'en sert pour le statut web. La décision de relancer le mouvement ne lit pas `MotorState` : elle repose sur `TrackerCommandState` et sur la réarmement du timeout à chaque boucle. L'objectif d'anti-répétition est donc couvert autrement ; `MotorState` sert au statut.

### 6. Ajout : reset par maintien en mode manuel (corrigé, à tester)

- Emplacement : `src/driver/tracker_command.h`, `src/driver/tracker_command.cpp`, `src/driver/tracker_command_state.h`, `src/driver/tracker_command_state.cpp`, `src/driver/tracker.h`, `src/driver/tracker.cpp`, `src/driver/driver.cpp`, `src/driver/driver_state.h`, `src/driver/setting.h`, `src/driver/setting_esp32.cpp`
- Comportement : le reset n'est possible qu'en mode manuel. Si `deploy` et `retract` restent enfoncés ensemble pendant `program.tracker.command.resetHoldMs` (5000 ms par défaut), `TrackerCommandState` passe à `RESET`. `Tracker::isResetRequested()` le expose, `Driver::update()` passe `DriverState` à `RESET`, et `loop()` de `driver.ino` appelle `Driver::reset()` (`ESP.restart()`).
- Pendant le maintien, les moteurs ne bougent pas (état `IDLE`). Hors mode manuel, ou dès qu'un bouton est relâché, le chronomètre est remis à zéro.
- Reste à faire : test sur matériel du délai réel et du redémarrage.

### 7. Moyen : journalisation TRACE dans le chemin de jog (ouvert)

- Emplacement : `src/driver/motor.cpp` (lignes 28, 36, 43, 51, 58), `src/driver/motors.cpp` (`retract()` appelle `stop()`), `src/driver/setting.h` (`LOG_LEVEL_TRACE`)
- Problème : en mode manuel, `Tracker::handleCommand()` appelle `_motors.deploy()` ou `_motors.retract()` à chaque boucle. Chaque appel écrit des lignes `Log.traceln` pour chaque moteur. Un jog produit environ 4 lignes par boucle (`deploy`), et environ 6 pour `retract` (`stop` puis `retract`). Ces écritures série peuvent ralentir la boucle principale, donc retarder la lecture des boutons et le réarmement du timeout (si la boucle s'allonge au-delà de `motors.timeout`, le moteur s'arrête en plein jog).
- Recommandation : passer ces logs en `verboseln`, ou les retirer du chemin de jog. Passer `LOG_LEVEL` à `LOG_LEVEL_NOTICE` pour l'usage normal.

### 8. Moyen : `resetHoldMs` non validé (corrigé)

- Emplacement : `src/driver/setting.cpp` (`assertSetting()`)
- Correction : ajout du contrôle `resetHoldMs > 0`, avec le message « Tracker command reset hold must be greater than 0 ».

### 9. Moyen : délai de 10 s au démarrage (corrigé, paramétrable)

- Emplacement : `src/driver/driver.cpp` (`Driver::init()`), `src/driver/setting.h`
- Correction : le délai est désormais la constante `LOG_BOOT_DELAY_MS` (10000 par défaut, conservé pour le moniteur série). Mettre `0` pour le supprimer.
- Reste à décider : conditionner le délai à la présence d'un moniteur série, si souhaité.

### 10. Bas : indicateur « waiting » affiché après le délai (corrigé)

- Emplacement : `src/driver/driver.cpp` (`Driver::init()`)
- Problème : `waiting()` était appelé après le délai de démarrage. La LED n'était donc pas animée pendant ce délai.
- Correction : `waiting()` est appelé avant le délai. L'attente est une boucle qui appelle `_ledProtocol->update()` jusqu'à `LOG_BOOT_DELAY_MS`, au lieu d'un `delay()` bloquant. Le message « Waiting before starting... » est journalisé dans le même bloc.

### 11. Bas : code mort (corrigé)

- Suppression de `TrackerState _state` et de `tracker_state.h/.cpp` (fichiers retirés), de `Tracker::_settingProgramMotors` (le paramètre du constructeur reste utilisé pour `Motors`), et de la déclaration non définie `Ldr::getPin()`.

### 12. Bas : garde `#if` toujours vraie (corrigé)

- Emplacement : `src/driver/driver.cpp` (`Driver::update()`)
- Correction : garde `#if defined(HEARTBEAT_INTERVAL)` retirée ; le contrôle est `LOG && HEARTBEAT`.

### 13. Bas : LED tracker jamais pilotée (ouvert)

- Emplacement : `src/driver/setting_esp32.cpp` (`board.pin.tracker.led = 4`)
- Problème : la broche est validée, journalisée et exposée dans `/status`, mais aucun code de commande ne l'utilise. Seule la LED d'état (`board.pin.led`) est pilotée, par `LedProtocol`.
- À confirmer : LED prévue pour une future indication, ou réglage obsolète à retirer.

### 14. Bas : accès direct aux membres publics (corrigé)

- Emplacement : `src/driver/ldrs.h`, `src/driver/ldrs.cpp`, `src/driver/tracker.cpp` (`Tracker::AppStatus()`)
- Correction : `ldrUp` et `ldrDown` sont privés ; ajout de `getUpAveragePercentValue()` et `getDownAveragePercentValue()`, utilisés par `AppStatus()`.

### 15. Bas : tableau de broches sans contrôle de bornes (corrigé)

- Emplacement : `src/driver/setting.cpp` (`assertPinsUnique()`, `registerPin()`)
- Correction : taille du tableau définie par `PIN_CAPACITY` (16), et `registerPin()` refuse l'enregistrement une fois la capacité atteinte (journal fatal « Too many pins registered »).

### 16. Bas : journaux de configuration incorrects ou incomplets (corrigé)

- `src/driver/setting.cpp` (`logSetting()`) : `motors.timeout` affiché avec `%lu`, et `resetHoldMs` journalisé.
- `src/driver/tracker.cpp` (`Tracker::interval()`) : la coquille `"automa"` est corrigée en `"auto"`.

### 17. Bas : journal `ArduinoLog` écrit depuis deux cœurs (à surveiller)

- Emplacement : `src/driver/app_network_service.cpp` (tâche réseau, cœur 0), `src/driver/driver.cpp` (cœur 1)
- Problème : `ArduinoLog` n'est pas protégé. Les lignes peuvent se mélanger sous forte charge. Le tampon `AppLogOutput` est protégé par `portENTER_CRITICAL`, donc seule la sortie `ArduinoLog` est concernée.

### 18. Bas : JSON de `/status` construit à la main (ouvert)

- Emplacement : `src/driver/app_network_service.cpp` (`handleStatus()`, `char body[1536]`, `snprintf`)
- État : les accolades sont équilibrées, et la taille actuelle reste sous 1536 octets. Mais `snprintf` tronque silencieusement : un champ qui grandit produirait un JSON invalide, et la page resterait bloquée.
- Recommandation : vérifier la valeur de retour de `snprintf` (taille ≥ `sizeof(body)`), ou passer à un sérialiseur JSON.
- Point mineur : `motors.timeout` est casté en `(int)` pour `%d`, alors que le constat 16 impose `%lu` pour ce champ dans les logs. Le résultat est correct, mais l'écriture est incohérente.

### 19. Info : points de conception à confirmer

- Régulation automatique : chaque décision LDR lance un mouvement de `motors.timeout` (1000 ms), arrêté par `Motor::update()`. Le tracker avance donc par impulsions de 1 s, pas vers une position cible. C'est probablement voulu, à confirmer.
- Mode nuit : dans `Tracker::interval()`, le cas `NIGHT` appelle `stop()` avec `// TODO: night behavior`. Le comportement de nuit (rangement, position de repos) n'est pas implémenté.
- Pas de limite de course en jog manuel : le moteur tourne tant que le bouton est enfoncé.

### 20. Bas : commentaire d'en-tête de `driver.ino` obsolète (corrigé)

- Emplacement : `src/driver/driver.ino` (ligne 4)
- Problème : le commentaire indiquait « Driver code for Arduino Uno/Nano/ESP32 ». Le support Uno/Nano a été retiré (constat 1).
- Correction : le commentaire indique désormais « Driver code for ESP32 ».

### 21. Bas : SSID WiFi journalisé en TRACE et exposé sur `/log` (corrigé)

- Emplacement : `src/driver/setting.cpp` (`logSetting()`, ligne 215), `src/driver/app_network_service.cpp` (`handleLog()`)
- Problème : `logSetting()` écrivait le SSID à chaque démarrage. `/log` renvoie ce tampon sans authentification.
- Correction : le SSID est affiché comme le mot de passe, c'est-à-dire « set » ou « empty ». `/log` n'expose plus le nom du réseau, et l'état reste utile au diagnostic.

### 22. Bas : `Motors::state()` ne reflétait que le moteur 1 (corrigé)

- Emplacement : `src/driver/motors.cpp` (`Motors::state()`), `src/driver/tracker.cpp` (`AppStatus()`, lignes 120-121)
- Problème : `Motors::state()` retournait `_motor1.state()` uniquement. Le champ `motor` de `/status` était donc calculé à partir du moteur 1 seul. Si un seul moteur s'arrêtait, le statut restait faux.
- Correction : `Motors::state()` combine les deux moteurs. Il renvoie `DEPLOY` si l'un des deux avance, sinon `RETRACT` si l'un des deux recule, sinon `IDLE`.

### 23. Bas : section critique longue dans `AppLogOutput::copyTo()` (accepté)

- Emplacement : `src/driver/app_log_buffer.cpp` (`copyTo()`, lignes 50-55)
- Constat : la boucle de copie (jusqu'à 4096 octets) s'exécute sous `portENTER_CRITICAL(&logMux)`, ce qui bloque brièvement les écritures de logs du cœur 1. La durée est estimée à quelques dizaines de microsecondes (non mesurée).
- Décision : conservé. Copier hors verrou risquerait de lire des données écrasées pendant la copie, donc des lignes de log mélangées dans `/log`. Ce compromis n'en vaut pas la peine pour un tampon de cette taille.
- Réouverture si : des mesures montrent un blocage gênant, ou si `program.app.log.bufferSize` est augmenté fortement.

## Améliorations vérifiées

- La boucle de décision par intervalle est connectée : `Tracker::update()` fait avancer `_everyInterval`, et les décisions sont prises dans `Tracker::interval()`.
- La règle « deux boutons = IDLE, puis RESET après `resetHoldMs` » est implémentée dans `TrackerCommand::update()`, uniquement en mode manuel.
- Le switch de mode est débouncé et initialisé dans `TrackerCommand::init()`. La sortie du mode manuel arrête le mouvement en cours.
- En mode automatique, `TrackerCommand::update()` ne lit pas les boutons. À la reprise du mode manuel, les débouncers se resynchronisent après leur délai ; aucun état incohérent n'a été trouvé.
- Le timeout moteur est réarmé à chaque appel en jog (`Motors::deploy()` et `Motors::retract()`), donc il ne coupe pas un mouvement maintenu.
- La validation des broches couvre toutes les broches du tracker, la LED d'état, et l'écran (SDA/SCL).
- Il ne reste aucune trace du STOP : ni broche, ni `DriverCommand`, ni JSON `stop`.
- Le tampon de logs est protégé par `portENTER_CRITICAL`, `copyTo()` refuse un pointeur nul, et les `malloc` sont vérifiés.
- L'instantané publié vers le serveur web est protégé par `portENTER_CRITICAL` ; le serveur ne garde aucun pointeur vers des données mutables.


## Connectivité WiFi et serveur web

- Les identifiants WiFi viennent de `setting_secret_wifi.h` (ignoré par git). Le modèle `setting_secret_wifi_sample.h` définit `SETTING_SECRET_WIFI_SSID` à une chaîne vide. Un SSID vide désactive le réseau avec un avertissement (`AppNetworkService::start()`), sans bloquer le firmware.
- La classe réseau s'appelle `AppNetworkService` (`app_network_service.h/.cpp`).
- La tâche réseau tourne sur le cœur 0 (`xTaskCreatePinnedToCore`), avec une pile de `program.app.network.taskStackSize` octets. La boucle `loop()` reste sur le cœur 1. Elle tente une reconnexion WiFi tous les `program.app.network.wifiReconnectInterval` ms (10 s par défaut).
- Réglages (`setting_esp32.cpp`) : `program.app.webServer.port` (80), `program.app.log.bufferSize` (4096), `program.app.network.taskStackSize` (8192), `program.app.network.wifiReconnectInterval` (10000). L'écran utilise `program.display` (adresse 0x3C, 128x64).
- Routes en lecture seule : `/` (page HTML qui interroge `/status` chaque seconde), `/status` (JSON) et `/log` (texte brut).
- Page `/` : source éditable `src/driver/app/app_status_page.html`. Après modification, lancer `python3 src/driver/app/app_embed_status_page.py` pour régénérer `src/driver/app_status_page.h` (chaîne `STATUS_PAGE` en PROGMEM), inclus par `app_network_service.cpp`. Le fichier généré doit être committé avec la source.
- `/status` contient deux objets principaux :
  - `status` : `driver`, `uptime`, `ldrUp`, `ldrDown`, `mode`, `motor`, `nextComparisonMs`, et `buttons` (`deploy` et `retract` en `pressed`/`released`, `mode` en `manual`/`auto`).
  - `setting` : `version`, `tracker` (`interval`, `commandDebounce`, `commandResetHoldMs`), `ldr`, `ldrs`, `motors`, et `gpio` (broches du tracker et LED). Les identifiants WiFi ne sont jamais exposés.
- Changement d'API : la clé `trackers` devient `tracker`. Les clés `buttons.stop`, `command.stopPressMs` et `gpio.stop` sont supprimées. Aucun client de `tests/` ne les utilise.
- Journaux : `AppLogOutput` (classe `Print`, `app_log_buffer.h/.cpp`) écrit sur Serial et dans un tampon circulaire. Sa taille (`program.app.log.bufferSize`) est allouée au démarrage par `AppLogOutput::init()`. La page `/log` affiche le contenu le plus récent.
- Compte à rebours : `Tracker` mémorise l'échéance de la prochaine comparaison dans `_nextComparisonMs` (`tracker.h`, ligne 39, calculée dans `tracker.cpp`). `status.nextComparisonMs` donne les millisecondes restantes, bornées à 0.
- Hypothèses non vérifiées : commutateur de mode câblé vers GND (LOW = manuel) avec pull-up interne ; page non testée dans un navigateur.

## Écran OLED SSD1306

- Affichage des valeurs moyennes des LDR (UP et DOWN, en %) sur un SSD1306 128x64 à l'adresse I2C `0x3C` (`display.h/.cpp`, bibliothèque `Adafruit_SSD1306`).
- Broches I2C : SDA = GPIO 21, SCL = GPIO 23.
- Rafraîchissement limité à 500 ms depuis `Driver::publishStatus()`. Si l'écran n'est pas détecté au démarrage, un avertissement est journalisé et le firmware continue sans affichage.
- Non testé sur matériel : câblage, adresse réelle de l'écran et rendu du texte.

## Revue P2

- Allocation mémoire : `malloc` du tampon de `/log` et du tampon circulaire sont vérifiés. Si `/log` ne peut pas allouer, la route renvoie HTTP 500 et le serveur continue.
- Validation des GPIO : `assertPinsUnique()` couvre désormais l'écran (SDA/SCL), en plus des broches du tracker et de la LED d'état (voir constat 15 pour la taille du tableau).
- Ouverts : constats 7, 13 et 18. À surveiller ou à confirmer : constats 17 et 19. Partiels : constats 3 bis et 5. Corrigés ou acceptés : les autres.

## Lacunes de tests

- Aucun test automatisé : le dossier `tests/` contient des sketches et des schémas Wokwi de démonstration, pas de tests du firmware `src/driver/`.
- Pas de matrice de compilation : seule la cible `esp32:esp32:esp32` est vérifiée (`arduino-cli compile` depuis `src/driver`).
- Pas de test du reset par maintien : deux boutons tenus moins de `resetHoldMs` ne doivent pas redémarrer l'ESP32, et les tenir au-delà doit le faire, uniquement en mode manuel.
- Pas de test du jog : maintenir un bouton au-delà de `motors.timeout` (1000 ms) doit garder le moteur en marche.
- Pas de test de la transition manuel vers automatique : le moteur doit s'arrêter immédiatement.
- Pas de test de la validation des broches dupliquées ni de `resetHoldMs` (constat 8).
- Pas de test du chemin d'échec d'allocation de `/log` (réponse 500) ni du tampon circulaire désactivé (taille 0).
- Pas de test matériel du WiFi ni du serveur web : connexion, IP sur le port série, page `/`, JSON `/status`, `/log`, reconnexion après coupure.
- Pas de test matériel de l'état des boutons sur la page : niveau de chaque bouton et du commutateur de mode.
- Aucun test ne valide la syntaxe du JSON de `/status` (construit avec `snprintf`, voir constat 18).

## Prochaines corrections suggérées

1. Retirer les logs TRACE du chemin de jog (constat 7).
2. Décider si le délai de démarrage (constat 9) doit être conditionné à la présence d'un moniteur série.
3. Ajouter un scénario de test (Wokwi dans `tests/`) pour le jog, la transition de mode et le reset par maintien.
4. Vérifier sur le matériel : polarité du commutateur de mode (GPIO 33), câblage des LDR (GPIO 34 et 35, entrées seules sans pull-up), démarrage avec GPIO 2 (LED d'état), et délai réel du reset.
5. Constat 23 (section critique de `copyTo()`) accepté : à rouvrir seulement si des mesures montrent un blocage gênant.

## Conclusion

La boucle de contrôle du tracker est fonctionnelle sur le papier : jog manuel, règle « deux boutons = arrêt », transition de mode, reset par maintien de deux boutons en mode manuel, et suppression complète du STOP. Le support est limité à l'ESP32, et la compilation est vérifiée sur cette cible (2026-10-30).

Les risques principaux restent matériels : polarité du commutateur de mode, GPIO 2 au démarrage, délai réel du reset, et journalisation TRACE qui peut ralentir le jog (constat 7). Côté logiciel, le point à traiter en priorité est désormais le retrait des logs TRACE du jog (constat 7). Les constats 20 à 23 sont de faible impact et ne bloquent pas le fonctionnement.

