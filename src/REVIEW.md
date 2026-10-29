# Code Review of src

Last review: 2026-10-27

## Périmètre

Cette revue porte sur le comportement à l'exécution, la cohérence de configuration entre cartes, et le flux de commandes/contrôle dans `src/driver`.

## Constats

### 1. Haut : support Uno/Nano retiré (corrigé)

- Emplacement : `src/driver/setting.h`, `src/driver/setting_esp32.cpp`, `src/driver/driver.ino`, `src/driver/led_protocol.cpp`, `src/driver/trackers.cpp`, `src/driver/driver.cpp`
- Problème initial : les implémentations Uno/Nano de `settingInit()` ne respectaient pas le contrat `Setting* settingInit()`.
- Correction : seule la carte ESP32 est supportée. Les fichiers `setting_uno.cpp` et `setting_nano.cpp` sont supprimés, et les gardes `BOARD_*` sont retirées. `BOARD_ESP32` n'étant plus défini, `setting_esp32.cpp` était compilé à vide et le link échouait (`undefined reference to settingInit`) ; les gardes ont été supprimées pour que le code ESP32 soit inconditionnel.

### 2. Moyen : l'état des commandes reste verrouillé sans retour à idle/running

- Emplacement : `src/driver/driver_command.cpp`, `src/driver/driver_command_state.cpp`, `src/driver/driver.cpp`, `src/driver/driver.ino`
- Problème : les callbacks de maintien passent l'état à `STOP` ou `RESET`, mais rien ne ramène l'état de commande à `IDLE` après consommation.
- État : corrigé en partie. `Driver::update()` n'appelle `stop()` qu'à la transition vers STOP (plus à chaque boucle), et `_trackers->update()` est ignoré tant que l'état est STOP. La boucle `loop()` de `driver.ino` ne traite plus STOP à chaque itération.
- Reste à faire : STOP reste verrouillé (pas de retour à `IDLE`). RESET arrête les moteurs puis reste sur RESET.

### 3. Moyen : conflits de GPIO dans la table des broches (corrigé)

- Emplacement : `src/driver/setting_esp32.cpp`, `src/driver/setting.cpp`
- Problème : les broches 25, 26, 27 et 32 étaient utilisées deux fois (moteurs et boutons de commande, LDR up et mode manuel).
- Correction : boutons deploy/retract sur 18/19, stop sur 21, mode manuel sur 15, définis par des `#define` nommés. Le mode manuel est porté par `SettingBoardPinTrackerCommand.mode` (par tracker). `assertSetting()` vérifie l'unicité des broches (`assertPinsUnique()`) et indique les deux champs en conflit.

### 3 bis. Moyen : broches de strapping ESP32 (corrigé)

- Emplacement : `src/driver/setting_esp32.cpp`
- Problème : GPIO 12 (`motors.in3`) et GPIO 2 (LED d'état) sont des broches de strapping ; leur état au démarrage peut perturber le boot.
- Correction : `motors.in3` passe de 12 à 22. La LED d'état reste sur GPIO 2 (choix retenu : LED intégrée de la plupart des DevKit). GPIO 2 reste une broche de strapping : elle ne doit pas être tirée fortement au démarrage.
- Point à vérifier : GPIO 16 n'est plus utilisée (PSRAM sur les modules WROVER).
- Point traité : le switch de mode est passé de GPIO 15 à GPIO 33 (pull-up interne), et le STOP de GPIO 21 à GPIO 32. GPIO 15 n'est plus utilisée.

### 3 ter. Moyen : durée de pression STOP et délai STOP non configurés (corrigé)

- Emplacement : `src/driver/setting.h`, `src/driver/setting.cpp`, `src/driver/setting_esp32.cpp`, `src/driver/driver.cpp`, `src/driver/driver_command.cpp`
- Problème : `assertSetting()` exigeait un délai STOP supérieur à zéro. `DriverCommand` utilisait une constante fixe (`DEBOUNCE_THRESHOLD`) au lieu du paramètre programme.
- Correction : le champ est renommé `pressMs` (durée de pression, pas un anti-rebond), défini par `SETTING_PROGRAM_COMMAND_STOP_PRESS_MS` (1000) sur les trois cartes. `DriverCommand` reçoit `SettingProgramCommandStop*` et appelle `setPressMs()` avec cette valeur.

### 4. Haut : commandes deploy/retract du tracker (corrigé sur ESP32)

- Emplacement : `src/driver/tracker_command.h`, `src/driver/tracker_command.cpp`, `src/driver/tracker_command_state.h`, `src/driver/tracker_command_state.cpp`, `src/driver/tracker.cpp`
- Problème : `TrackerCommand::update()` lisait les boutons sans exploiter leur état, et ses `ADebouncer` n'étaient pas initialisés.
- Correction : `TrackerCommand` suit le pattern `driver_command` (`init()`, `update()` et `state()`, `ADebouncer` en membres par valeur). Le debounce est injecté via `SettingProgramTrackersCommand`. `TrackerCommandState` (`tracker_command_state.h/.cpp`) est un `enum class` pour éviter les conflits de noms (`IDLE`).
- Règles de commande :
  - Deux boutons manuels appuyés en même temps : état `IDLE` (arrêt sûr), au lieu d'un DEPLOY arbitraire.
  - Un seul bouton : `DEPLOY` ou `RETRACT`.
  - Au relâchement : `stop()`.
- Mode manuel (jog) : tant que le bouton est maintenu, `deploy()` ou `retract()` est relancé à chaque boucle. Le timeout moteur est réarmé à chaque appel, donc il ne coupe plus un mouvement maintenu. Aucune limite de sécurité : le moteur tourne tant que le bouton est enfoncé.
- Sortie du mode manuel : le switch de mode est géré par `TrackerCommand`. C'est une bascule à deux positions : LOW (broche reliée à GND, pull-up interne) = manuel, HIGH = automatique. Son entrée est débouncée avec la même période que les boutons (`debounce`). En sortie de manuel, `handleCommand()` arrête le moteur s'il était en DEPLOY ou RETRACT, puis la commande passe à `AUTO`. Un bouton encore maintenu au retour en manuel est pris en compte. En automatique, seule la régulation LDR (`interval()`) pilote le moteur.
- Reste à faire : polarité du switch à vérifier au câblage. Test sur matériel ou simulation non réalisé.

### 5. Moyen : état moteur ajouté (`MotorState`) pour l'anti-répétition

- Emplacement : `src/driver/motor_state.h`, `src/driver/motor_state.cpp`, `src/driver/motor.h`, `src/driver/motor.cpp`, `src/driver/tracker.cpp`
- Problème : le timeout moteur (`motors.timeout`, 1000 ms) arrêtait le moteur sans état mémorisé, et `Tracker` relançait `deploy()`/`retract()` à chaque boucle.
- Correction : `MotorState` (`enum class MotorStateType { IDLE, DEPLOY, RETRACT }`) suit le pattern de `driver_state` et `tracker_state`. `Motor` possède `_state` et `state()`. `deploy`/`retract` passent à DEPLOY/RETRACT, `stop` et `init` passent à IDLE, et le timeout passe aussi à IDLE.
- Reste à faire : `Tracker` ne lit pas encore `MotorState` ; la décision de relancer le mouvement repose sur `TrackerCommandState`.

## Améliorations vérifiées

- La boucle de décision par intervalle est connectée : `Tracker::update()` fait avancer `_everyInterval`, et les décisions sont prises dans `Tracker::interval()`.
- Le risque de décision périmée d'un cycle est réduit par l'échantillonnage (`_ldrs.update()`) dans le callback d'intervalle.
- La zone morte arrête activement le mouvement lorsque le tracker se déplace.
- La broche du mode manuel est initialisée (pull-up) et débouncée dans `TrackerCommand::init()` et `update()`.
- Le mode manuel sort tôt du tick d'intervalle, et la sortie du mode manuel arrête le mouvement manuel en cours.
- Les mouvements des moteurs utilisent des appels avec timeout via `Motors::deploy()` et `Motors::retract()` lorsque `program.motors.timeout` est configuré. En manuel, le jog relance le mouvement tant que le bouton est maintenu.
- Le câblage des callbacks `Ldr` est compatible AVR grâce aux callbacks avec contexte de `DV_EveryInterval`.
- Les symboles obsolètes ont été supprimés (`Ldr::raw`, `LdrsComparison::NOT_UPDATED`).
- Les réglages ESP32 initialisent tous les champs programme validés actuellement, y compris `program.trackers.interval`.

## Connectivité WiFi et serveur web (ajout)

- Les identifiants WiFi viennent de `setting_secret_wifi.h` (ignoré par git). Le modèle `setting_secret_wifi_sample.h` définit les mêmes macros (`SETTING_SECRET_WIFI_SSID`, `SETTING_SECRET_WIFI_PASSWORD`) avec des valeurs vides. Un SSID vide désactive le réseau avec un avertissement, sans bloquer le firmware.
- La classe s'appelle `AppNetworkService` (fichiers `app_network_service.h/.cpp`). Le nom `Network` entre en conflit avec l'objet global `Network` du core ESP32, et `network.h` entre en collision avec `Network.h` de la bibliothèque WiFi sur macOS (système de fichiers insensible à la casse).
- La tâche réseau tourne sur le cœur 0 (`xTaskCreatePinnedToCore`), avec une pile de `program.app.network.taskStackSize` octets. La boucle `loop()` reste sur le cœur 1. Elle tente une reconnexion WiFi tous les `program.app.network.wifiReconnectInterval` ms (10 s par défaut).
- Réglages (`setting_esp32.cpp`) : `program.app.webServer.port` (80), `program.app.log.bufferSize` (4096), `program.app.network.taskStackSize` (8192), `program.app.network.wifiReconnectInterval` (10000). Ces valeurs ne servent qu'à l'application web. L'écran utilise `program.display` (adresse 0x3C, 128x64).
- Le cœur 1 publie un instantané `AppSystemStatus` (`app_status.h`) protégé par `portENTER_CRITICAL`. Le serveur le lit sans pointeur partagé vers des données mutables.
- Routes en lecture seule : `/` (page HTML qui interroge `/status` chaque seconde), `/status` (JSON) et `/log` (texte brut).
- `/status` contient trois objets :
  - `status` : driver, uptime, ldrUp, ldrDown, mode, motor, `nextComparisonMs`, et `buttons` (deploy, retract, stop en `pressed`/`released`, mode en `manual`/`auto`).
  - `setting` : version, trackers, ldr, ldrs, motors, command.stopPressMs, et `gpio` (broches du tracker 1, LED et STOP). Les identifiants WiFi ne sont jamais exposés.
  - Les valeurs de `buttons` et `gpio` sont lues sur le cœur 1 et publiées dans l'instantané.
- Les journaux : `AppLogOutput` (classe `Print`, `app_log_buffer.h/.cpp`) écrit sur Serial et dans un tampon circulaire protégé par un verrou. La taille (`program.app.log.bufferSize`, 4 Ko par défaut) est allouée au démarrage par `AppLogOutput::init()`. La page `/log` affiche le contenu le plus récent.
- Le journal `ArduinoLog` est écrit depuis deux cœurs. Les lignes peuvent se mélanger sous forte charge (à surveiller).
- Hypothèses non vérifiées : le commutateur de mode est câblé vers GND (LOW = manuel) avec pull-up interne ; la page n'a pas été testée dans un navigateur.
- Compte à rebours avant la prochaine comparaison LDR : `Tracker` mémorise l'échéance (la bibliothèque `DV_EveryInterval` n'expose pas la prochaine exécution). Le champ `status.nextComparisonMs` donne les millisecondes restantes, et la page décrémente le compteur chaque seconde.

## Écran OLED SSD1306 (ajout)

- Affichage des valeurs moyennes des LDR (UP et DOWN, en %) sur un SSD1306 128x64 à l'adresse I2C `0x3C` (`display.h/.cpp`, bibliothèque `Adafruit_SSD1306`).
- Broches I2C : SDA = GPIO 21, SCL = GPIO 23. GPIO 5 (strapping SDIO) n'est plus utilisée. La LED d'état est sur GPIO 2.
- Rafraîchissement limité à 500 ms depuis `Driver::publishStatus()`. Si l'écran n'est pas détecté au démarrage, un avertissement est journalisé et le firmware continue sans affichage.
- Non testé sur matériel : câblage, adresse réelle de l'écran et rendu du texte.

## Revue P2 (corrigé)

- Allocation mémoire : `malloc` du tampon de `/log` (`app_network_service.cpp`) et du tampon circulaire (`app_log_buffer.cpp`) est vérifié. Si `/log` ne peut pas allouer, la route renvoie HTTP 500 et le serveur continue. `AppLogOutput::copyTo()` refuse un pointeur nul et n'écrit rien dans ce cas.
- Validation des GPIO : `assertPinsUnique()` (`setting.cpp`) couvre désormais `board.pin.display.sda` et `board.pin.display.scl`, en plus des trackers, de la LED d'état et du STOP.
- LED d'état : déplacée de GPIO 16 (PSRAM sur les modules WROVER) vers GPIO 2, choix retenu. GPIO 2 est une broche de strapping, mais c'est la LED intégrée de la plupart des DevKit.

## Lacunes de tests

- Pas de matrice de compilation : seule la cible `esp32:esp32:esp32` est vérifiée (`arduino-cli compile` depuis `src/driver`).
- Pas de test automatisé pour la consommation des commandes STOP/RESET, notamment pour savoir si STOP doit rester verrouillé ou revenir à IDLE après relâchement.
- Aucun test automatisé pour les doublons de broches (la validation existe mais n'est pas testée).
- Pas de test de comportement moteur prouvant que les appels deploy/retract avec timeout maintiennent les moteurs actifs pendant la fenêtre prévue.
- Pas de test du jog : maintenir un bouton au-delà de `motors.timeout` (1000 ms) doit garder le moteur en marche.
- Pas de test de la transition manuel vers automatique : le moteur doit s'arrêter immédiatement.
- Pas de test matériel du WiFi ni du serveur web : connexion, IP sur le port série, page `/`, JSON `/status`, `/log`, reconnexion après coupure.
- Pas de test du chemin d'échec d'allocation de `/log` (réponse 500) ni du tampon circulaire désactivé (taille 0).
- Pas de test matériel de l'état des boutons sur la page : vérifier le niveau de chaque bouton et du commutateur de mode (manuel/auto).
- Le JSON de `/status` est construit à la main avec `snprintf` : une accolade oubliée rend la réponse invalide et la page reste bloquée (régression rencontrée lors de l'ajout de `nextComparisonMs`). Aucun test ne valide la syntaxe du JSON ; à ajouter si le format évolue.

## Prochaines corrections suggérées

1. Décider si l'état STOP/RESET est un événement ou un verrou ; s'il est un événement, le ramener à `IDLE` après traitement.
2. Ajouter un scénario de test (Wokwi dans `tests/`) pour le jog et la transition de mode.
3. Vérifier sur le matériel que GPIO 2 (LED d'état) ne perturbe pas le démarrage.
4. Vérifier le câblage des boutons STOP (GPIO 32) et mode (GPIO 33), et des LDR (GPIO 34 et 35, entrées seules sans pull-up).

## Conclusion

La boucle de contrôle du tracker est en meilleur état que ne le laissait penser la revue précédente. Les corrections récentes couvrent le jog manuel, la règle « deux boutons = arrêt », la transition de mode, les broches de strapping ESP32 et la durée STOP configurable. Le support est limité à l'ESP32, et la compilation est vérifiée sur cette cible. Le verrouillage de l'état STOP/RESET, le câblage du commutateur de mode (GPIO 33), GPIO 2 au démarrage et l'absence de test matériel restent les principaux risques.