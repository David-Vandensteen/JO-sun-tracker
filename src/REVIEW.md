# Code Review of src

Last review: 2026-10-27

## Périmètre

Cette revue porte sur le comportement à l'exécution, la cohérence de configuration entre cartes, et le flux de commandes/contrôle dans `src/driver`.

## Constats

### 1. Haut : les implémentations Uno/Nano de `settingInit()` ne respectent pas l'API commune

- Emplacement : `src/driver/setting.h`, `src/driver/setting_uno.cpp`, `src/driver/setting_nano.cpp`, `src/driver/driver.ino`
- Problème : `setting.h` déclare `Setting* settingInit()` et `driver.ino` en affecte la valeur de retour, mais les fichiers Uno/Nano implémentent `void settingInit()` et affectent `setting = getSetting();`. Le symbole `setting` n'est pas déclaré dans ces unités de compilation.
- Impact : sélectionner `BOARD_UNO` ou `BOARD_NANO` devrait faire échouer la compilation avant que la validation à l'exécution ne puisse s'exécuter.
- Vérification : l'implémentation ESP32 renvoie `Setting*`, alors que Uno/Nano diffèrent de cette signature et du contrat d'appel.

### 2. Moyen : l'état des commandes reste verrouillé sans retour à idle/running

- Emplacement : `src/driver/driver_command.cpp`, `src/driver/driver_command_state.cpp`, `src/driver/driver.cpp`, `src/driver/driver.ino`
- Problème : les callbacks de maintien passent l'état à `STOP` ou `RESET`, mais rien ne ramène l'état de commande à `IDLE` après consommation.
- État : corrigé en partie. `Driver::update()` n'appelle `stop()` qu'à la transition vers STOP (plus à chaque boucle), et `_trackers->update()` est ignoré tant que l'état est STOP. La boucle `loop()` de `driver.ino` ne traite plus STOP à chaque itération.
- Reste à faire : STOP reste verrouillé (pas de retour à `IDLE`). RESET arrête les moteurs puis reste sur RESET.

### 3. Moyen : conflits de GPIO dans la table des broches (ESP32 corrigé, Uno/Nano à traiter)

- Emplacement : `src/driver/setting_esp32.cpp`, `src/driver/setting.cpp`, `src/driver/setting_uno.cpp`, `src/driver/setting_nano.cpp`
- Problème (ESP32, corrigé) : les broches 25, 26, 27 et 32 étaient utilisées deux fois (moteurs et boutons de commande, LDR up et mode manuel).
- Correction (ESP32) : boutons deploy/retract sur 18/19, stop sur 21, mode manuel sur 15, définis par des `#define` nommés. Le mode manuel est désormais porté par `SettingBoardPinTrackerCommand.mode` (par tracker). `assertSetting()` vérifie désormais l'unicité des broches (`assertPinsUnique()`) et indique les deux champs en conflit.
- Uno/Nano (corrigé) : sur Uno, la LED tracker passe de `LED_BUILTIN` (13, conflit avec `motors.enb`) à GPIO 7. Sur Nano, la LED de statut passe de `LED_BUILTIN` (13, conflit avec `motors.enb`) à GPIO 5. Les boutons de commande Uno/Nano sont aussi réaffectés (Uno : deploy 3, retract 2, stop 4, manuel 5 ; Nano : deploy 4, retract 6, stop 2, manuel 3).

### 3 bis. Moyen : broches de strapping ESP32 (corrigé)

- Emplacement : `src/driver/setting_esp32.cpp`
- Problème : GPIO 12 (`motors.in3`) et GPIO 2 (LED d'état) sont des broches de strapping ; leur état au démarrage peut perturber le boot.
- Correction : `motors.in3` passe de 12 à 22, la LED d'état passe de 2 à 16.
- Point à vérifier : GPIO 16 est utilisée par la PSRAM sur les modules WROVER. Le type exact de module n'est pas confirmé (le build utilise `PSRAM=disabled`).

### 3 ter. Moyen : durée de pression STOP et délai STOP non configurés (corrigé)

- Emplacement : `src/driver/setting.h`, `src/driver/setting.cpp`, `src/driver/setting_esp32.cpp`, `src/driver/setting_uno.cpp`, `src/driver/setting_nano.cpp`, `src/driver/driver.cpp`, `src/driver/driver_command.cpp`
- Problème : `assertSetting()` exigeait un délai STOP supérieur à zéro, mais Uno et Nano ne l'affectaient pas. `DriverCommand` utilisait une constante fixe (`DEBOUNCE_THRESHOLD`) au lieu du paramètre programme.
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
- Sortie du mode manuel : `handleCommand()` arrête le moteur immédiatement si un mouvement manuel était en cours, puis remet la commande à `IDLE`. Un bouton encore maintenu au retour en manuel est pris en compte. En automatique, seule la régulation LDR (`interval()`) pilote le moteur.
- Reste à faire : vérifier la compilation Uno et Nano (non compilées pour l'instant). Test sur matériel ou simulation non réalisé.

### 5. Moyen : état moteur ajouté (`MotorState`) pour l'anti-répétition

- Emplacement : `src/driver/motor_state.h`, `src/driver/motor_state.cpp`, `src/driver/motor.h`, `src/driver/motor.cpp`, `src/driver/tracker.cpp`
- Problème : le timeout moteur (`motors.timeout`, 1000 ms) arrêtait le moteur sans état mémorisé, et `Tracker` relançait `deploy()`/`retract()` à chaque boucle.
- Correction : `MotorState` (`enum class MotorStateType { IDLE, DEPLOY, RETRACT }`) suit le pattern de `driver_state` et `tracker_state`. `Motor` possède `_state` et `state()`. `deploy`/`retract` passent à DEPLOY/RETRACT, `stop` et `init` passent à IDLE, et le timeout passe aussi à IDLE.
- Reste à faire : `Tracker` ne lit pas encore `MotorState` ; la décision de relancer le mouvement repose sur `TrackerCommandState`.

## Améliorations vérifiées

- La boucle de décision par intervalle est connectée : `Tracker::update()` fait avancer `_everyInterval`, et les décisions sont prises dans `Tracker::interval()`.
- Le risque de décision périmée d'un cycle est réduit par l'échantillonnage (`_ldrs.update()`) dans le callback d'intervalle.
- La zone morte arrête activement le mouvement lorsque le tracker se déplace.
- La broche du mode manuel est explicitement initialisée dans `Tracker::init()`.
- Le mode manuel sort tôt du tick d'intervalle, et la sortie du mode manuel arrête le mouvement manuel en cours.
- Les mouvements des moteurs utilisent des appels avec timeout via `Motors::deploy()` et `Motors::retract()` lorsque `program.motors.timeout` est configuré. En manuel, le jog relance le mouvement tant que le bouton est maintenu.
- Le câblage des callbacks `Ldr` est compatible AVR grâce aux callbacks avec contexte de `DV_EveryInterval`.
- Les symboles obsolètes ont été supprimés (`Ldr::raw`, `LdrsComparison::NOT_UPDATED`).
- Les réglages ESP32 initialisent tous les champs programme validés actuellement, y compris `program.trackers.interval`.

## Lacunes de tests

- Pas de matrice de compilation automatisée pour `BOARD_ESP32`, `BOARD_UNO` et `BOARD_NANO`.
- Pas de test automatisé pour la consommation des commandes STOP/RESET, notamment pour savoir si STOP doit rester verrouillé ou revenir à IDLE après relâchement.
- Aucun test automatisé pour les doublons de broches (la validation existe mais n'est pas testée).
- Pas de test de comportement moteur prouvant que les appels deploy/retract avec timeout maintiennent les moteurs actifs pendant la fenêtre prévue.
- Pas de test du jog : maintenir un bouton au-delà de `motors.timeout` (1000 ms) doit garder le moteur en marche.
- Pas de test de la transition manuel vers automatique : le moteur doit s'arrêter immédiatement.

## Prochaines corrections suggérées

1. Aligner les `settingInit()` Uno/Nano sur le contrat commun `Setting* settingInit()`.
2. Décider si l'état STOP/RESET est un événement ou un verrou ; s'il est un événement, le ramener à `IDLE` après traitement.
3. Vérifier la compilation Uno et Nano avec la validation des doublons de broches.
4. Ajouter un scénario de test (Wokwi dans `tests/`) pour le jog et la transition de mode.
5. Confirmer le type de module ESP32 (WROVER ou DevKit) pour valider l'usage de GPIO 16.

## Conclusion

La boucle de contrôle du tracker est en meilleur état que ne le laissait penser la revue précédente. Les corrections récentes couvrent le jog manuel, la règle « deux boutons = arrêt », la transition de mode, les broches de strapping ESP32 et la durée STOP configurable. La matrice des cartes reste le point le plus prioritaire : l'API `settingInit()` Uno/Nano doit être alignée pour que ces cibles compilent. Le verrouillage de l'état STOP/RESET et l'absence de test matériel restent les principaux risques.