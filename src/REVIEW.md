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
- Correction : `motors.in3` passe de 12 à 22, la LED d'état passe de 2 à 16.
- Point à vérifier : GPIO 16 est utilisée par la PSRAM sur les modules WROVER. Le type exact de module n'est pas confirmé (le build utilise `PSRAM=disabled`).
- Point à traiter : le switch de mode est sur GPIO 15, aussi une broche de strapping, et `setting_esp32.cpp` le signale comme à éviter. À déplacer ou à valider selon le câblage.

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
- La classe s'appelle `NetworkService` (fichiers `network_service.h/.cpp`). Le nom `Network` entre en conflit avec l'objet global `Network` du core ESP32, et `network.h` entre en collision avec `Network.h` de la bibliothèque WiFi sur macOS (système de fichiers insensible à la casse).
- La tâche réseau tourne sur le cœur 0 (`xTaskCreatePinnedToCore`). La boucle `loop()` reste sur le cœur 1. Elle reconnecte le WiFi toutes les 10 s.
- Le cœur 1 publie un instantané `SystemStatus` (`status.h`) protégé par `portENTER_CRITICAL`. Le serveur le lit sans pointeur partagé vers des données mutables.
- Routes en lecture seule : `/` (page HTML qui interroge `/status` chaque seconde) et `/status` (JSON : driver, uptime, ldrUp, ldrDown, mode, motor).
- Le journal `ArduinoLog` est écrit depuis deux cœurs. Les lignes peuvent se mélanger sous forte charge (à surveiller).

## Lacunes de tests

- Pas de matrice de compilation : seule la cible `esp32:esp32:esp32` est vérifiée (`arduino-cli compile` depuis `src/driver`).
- Pas de test automatisé pour la consommation des commandes STOP/RESET, notamment pour savoir si STOP doit rester verrouillé ou revenir à IDLE après relâchement.
- Aucun test automatisé pour les doublons de broches (la validation existe mais n'est pas testée).
- Pas de test de comportement moteur prouvant que les appels deploy/retract avec timeout maintiennent les moteurs actifs pendant la fenêtre prévue.
- Pas de test du jog : maintenir un bouton au-delà de `motors.timeout` (1000 ms) doit garder le moteur en marche.
- Pas de test de la transition manuel vers automatique : le moteur doit s'arrêter immédiatement.
- Pas de test matériel du WiFi ni du serveur web : connexion, IP sur le port série, page `/` et JSON `/status`, reconnexion après coupure.

## Prochaines corrections suggérées

1. Décider si l'état STOP/RESET est un événement ou un verrou ; s'il est un événement, le ramener à `IDLE` après traitement.
2. Ajouter un scénario de test (Wokwi dans `tests/`) pour le jog et la transition de mode.
3. Confirmer le type de module ESP32 (WROVER ou DevKit) pour valider l'usage de GPIO 16.
4. Déplacer le switch de mode hors de GPIO 15 (strapping), ou valider son usage au démarrage.

## Conclusion

La boucle de contrôle du tracker est en meilleur état que ne le laissait penser la revue précédente. Les corrections récentes couvrent le jog manuel, la règle « deux boutons = arrêt », la transition de mode, les broches de strapping ESP32 et la durée STOP configurable. Le support est limité à l'ESP32, et la compilation est vérifiée sur cette cible. Le verrouillage de l'état STOP/RESET, la broche 15 du mode et l'absence de test matériel restent les principaux risques.