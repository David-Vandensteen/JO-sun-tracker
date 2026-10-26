# Code Review of src

Last review: 2026-08-31

## Scope

This pass reviews runtime behavior, board configuration parity, and command/control flow in `src/driver`.

## Findings

### 1. High: Uno/Nano `settingInit()` implementations do not match the shared API

- Location: `src/driver/setting.h`, `src/driver/setting_uno.cpp`, `src/driver/setting_nano.cpp`, `src/driver/driver.ino`
- Issue: `setting.h` declares `Setting* settingInit()` and `driver.ino` assigns its return value, but the Uno/Nano files implement `void settingInit()` and assign to `setting = getSetting();`. That `setting` symbol is not declared in those translation units.
- Impact: selecting `BOARD_UNO` or `BOARD_NANO` is expected to fail compilation before runtime validation can run.
- Verification: the ESP32 implementation returns `Setting*`, while Uno/Nano diverge from that signature and from the call site contract.

### 2. Medium: command state is latched with no return to idle/running behavior

- Location: `src/driver/command.cpp`, `src/driver/command_state.cpp`, `src/driver/tracker.cpp`
- Issue: hold callbacks set `_state` to `STOP` or `RESET`, but `Command::update()` only updates hold timing and returns the current state. No path clears the command state back to `IDLE`.
- Impact: once STOP is triggered, `Tracker::update()` keeps stopping and returning early. RESET currently stops the motors and then falls through with a TODO, but the command remains latched as RESET on later updates.
- Verification: `CommandState` has `setStateIdle()`, but it is not called by `Command` or `Tracker` after a command is consumed.

### 3. Medium: GPIO conflicts in board pin map (ESP32 corrigé, Uno/Nano à traiter)

- Location: `src/driver/setting_esp32.cpp`, `src/driver/setting.cpp`, `src/driver/setting_uno.cpp`, `src/driver/setting_nano.cpp`
- Issue (ESP32, corrigé) : les broches 25, 26, 27 et 32 étaient utilisées deux fois (moteurs et boutons de commande, LDR up et mode manuel).
- Correction (ESP32) : boutons deploy/retract sur 18/19, stop sur 21, mode manuel sur 23, définis par des `#define` nommés. `assertSetting()` vérifie désormais l'unicité des broches (`assertPinsUnique()`) et indique les deux champs en conflit.
- Uno/Nano (corrigé) : sur Uno, la LED tracker passe de `LED_BUILTIN` (13, conflit avec `motors.enb`) à GPIO 7. Sur Nano, la LED de statut passe de `LED_BUILTIN` (13, conflit avec `motors.enb`) à GPIO 5.

## Verified Improvements

- Interval decision loop is connected: `Tracker::update()` advances `_everyInterval`, and decisions run in `Tracker::interval()`.
- One-cycle stale decision risk is reduced by sampling (`_ldrs.update()`) inside the interval callback.
- Deadband now actively stops motion when tracker is moving.
- Manual mode pin is explicitly initialized in `Tracker::init()`.
- Manual mode forces stop and exits interval tick early, preventing residual motion after mode switch.
- Motor movements use timeout-aware calls through `Motors::deploy()` and `Motors::retract()` when `program.motors.timeout` is configured.
- `Ldr` callback wiring is AVR-safe via context callbacks in `DV_EveryInterval`.
- Stale symbols were removed (`Ldr::raw`, `LdrsComparison::NOT_UPDATED`).
- ESP32 settings initialize all currently validated program fields, including `program.trackers.interval`.

## Test Gaps

- No automated compile matrix for `BOARD_ESP32`, `BOARD_UNO`, and `BOARD_NANO`.
- No automated test for STOP/RESET consumption semantics, including whether STOP should latch permanently or return to IDLE after release.
- Aucun test automatisé pour les doublons de broches (la validation existe mais n'est pas testée).
- No motor behavior test proving that timeout-aware deploy/retract calls keep motors active for the intended window.

## Suggested Next Fixes

1. Align Uno/Nano `settingInit()` with the shared `Setting* settingInit()` contract.
2. Decide whether command states are events or latches; if they are events, clear them after `Tracker` consumes STOP/RESET.
3. Vérifier la compilation Uno et Nano avec la validation des doublons de broches.

## Conclusion

The tracker control loop is in better shape than the earlier review suggested, but the board matrix now needs attention. The highest-priority stabilization item is the Uno/Nano `settingInit()` API mismatch because it blocks those targets from compiling. After that, command-state consumption and ESP32 pin uniqueness are the main runtime risks to clarify.