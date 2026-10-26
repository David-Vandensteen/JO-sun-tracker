# Code Review of src

Last review: 2026-08-31

## Périmètre

Cette revue porte sur le comportement à l'exécution, la cohérence de configuration entre cartes, et le flux de commandes/contrôle dans `src/driver`.

## Constats

### 1. Haut : les implémentations Uno/Nano de `settingInit()` ne respectent pas l'API commune

- Emplacement : `src/driver/setting.h`, `src/driver/setting_uno.cpp`, `src/driver/setting_nano.cpp`, `src/driver/driver.ino`
- Problème : `setting.h` déclare `Setting* settingInit()` et `driver.ino` en affecte la valeur de retour, mais les fichiers Uno/Nano implémentent `void settingInit()` et affectent `setting = getSetting();`. Le symbole `setting` n'est pas déclaré dans ces unités de compilation.
- Impact : sélectionner `BOARD_UNO` ou `BOARD_NANO` devrait faire échouer la compilation avant que la validation à l'exécution ne puisse s'exécuter.
- Vérification : l'implémentation ESP32 renvoie `Setting*`, alors que Uno/Nano diffèrent de cette signature et du contrat d'appel.

### 2. Moyen : l'état des commandes reste verrouillé sans retour à idle/running

- Emplacement : `src/driver/command.cpp`, `src/driver/command_state.cpp`, `src/driver/tracker.cpp`
- Problème : les callbacks de maintien passent `_state` à `STOP` ou `RESET`, mais `Command::update()` ne fait que mettre à jour la temporisation du maintien et renvoie l'état courant. Aucun chemin ne ramène l'état de commande à `IDLE`.
- Impact : une fois STOP déclenché, `Tracker::update()` continue d'arrêter et de sortir tôt. RESET arrête actuellement les moteurs puis continue avec un TODO, mais la commande reste verrouillée sur RESET aux mises à jour suivantes.
- Vérification : `CommandState` possède `setStateIdle()`, mais `Command` et `Tracker` ne l'appellent pas après qu'une commande a été consommée.

### 3. Moyen : conflits de GPIO dans la table des broches (ESP32 corrigé, Uno/Nano à traiter)

- Emplacement : `src/driver/setting_esp32.cpp`, `src/driver/setting.cpp`, `src/driver/setting_uno.cpp`, `src/driver/setting_nano.cpp`
- Problème (ESP32, corrigé) : les broches 25, 26, 27 et 32 étaient utilisées deux fois (moteurs et boutons de commande, LDR up et mode manuel).
- Correction (ESP32) : boutons deploy/retract sur 18/19, stop sur 21, mode manuel sur 23, définis par des `#define` nommés. `assertSetting()` vérifie désormais l'unicité des broches (`assertPinsUnique()`) et indique les deux champs en conflit.
- Uno/Nano (corrigé) : sur Uno, la LED tracker passe de `LED_BUILTIN` (13, conflit avec `motors.enb`) à GPIO 7. Sur Nano, la LED de statut passe de `LED_BUILTIN` (13, conflit avec `motors.enb`) à GPIO 5.

### 4. Haut : commandes deploy/retract du tracker (corrigé sur ESP32)

- Emplacement : `src/driver/tracker_command.h`, `src/driver/tracker_command.cpp`, `src/driver/tracker_command_state.h`, `src/driver/tracker_command_state.cpp`, `src/driver/tracker.cpp`
- Problème : `TrackerCommand::update()` lisait les boutons sans exploiter leur état, et ses `ADebouncer` n'étaient pas initialisés.
- Correction : `TrackerCommand` suit le pattern `driver_command` (`init()`, `update()` renvoyant un `TrackerCommandState`, `ADebouncer` en membres par valeur). `Tracker` possède `_trackerCommand`, appelle `init()` et, en mode manuel, lance `deploy()`/`retract()` tant que le bouton est maintenu, puis `stop()` au relâchement. Le debounce est injecté via `SettingProgramTrackersCommand`.
- Reste à faire : vérifier la compilation Uno et Nano (non compilées pour l'instant).

## Améliorations vérifiées

- La boucle de décision par intervalle est connectée : `Tracker::update()` fait avancer `_everyInterval`, et les décisions sont prises dans `Tracker::interval()`.
- Le risque de décision périmée d'un cycle est réduit par l'échantillonnage (`_ldrs.update()`) dans le callback d'intervalle.
- La zone morte arrête activement le mouvement lorsque le tracker se déplace.
- La broche du mode manuel est explicitement initialisée dans `Tracker::init()`.
- Le mode manuel force l'arrêt et sort tôt du tick d'intervalle, ce qui évite un mouvement résiduel après un changement de mode.
- Les mouvements des moteurs utilisent des appels avec timeout via `Motors::deploy()` et `Motors::retract()` lorsque `program.motors.timeout` est configuré.
- Le câblage des callbacks `Ldr` est compatible AVR grâce aux callbacks avec contexte de `DV_EveryInterval`.
- Les symboles obsolètes ont été supprimés (`Ldr::raw`, `LdrsComparison::NOT_UPDATED`).
- Les réglages ESP32 initialisent tous les champs programme validés actuellement, y compris `program.trackers.interval`.

## Lacunes de tests

- Pas de matrice de compilation automatisée pour `BOARD_ESP32`, `BOARD_UNO` et `BOARD_NANO`.
- Pas de test automatisé pour la consommation des commandes STOP/RESET, notamment pour savoir si STOP doit rester verrouillé ou revenir à IDLE après relâchement.
- Aucun test automatisé pour les doublons de broches (la validation existe mais n'est pas testée).
- Pas de test de comportement moteur prouvant que les appels deploy/retract avec timeout maintiennent les moteurs actifs pendant la fenêtre prévue.

## Prochaines corrections suggérées

1. Aligner les `settingInit()` Uno/Nano sur le contrat commun `Setting* settingInit()`.
2. Décider si les états de commande sont des événements ou des verrous ; s'ils sont des événements, les effacer après que `Tracker` a consommé STOP/RESET.
3. Vérifier la compilation Uno et Nano avec la validation des doublons de broches.

## Conclusion

La boucle de contrôle du tracker est en meilleur état que ne le laissait penser la revue précédente, mais la matrice des cartes demande maintenant de l'attention. La correction de stabilisation la plus prioritaire est le décalage de l'API `settingInit()` Uno/Nano, car il empêche la compilation de ces cibles. Ensuite, la consommation des états de commande et l'unicité des broches ESP32 sont les principaux risques à l'exécution à clarifier.