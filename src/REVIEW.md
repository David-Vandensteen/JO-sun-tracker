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

### 3. Medium: ESP32 manual-mode input conflicts with tracker LED configuration

- Location: `src/driver/setting_esp32.cpp`, `src/driver/tracker.cpp`, `src/driver/setting.cpp`
- Issue: ESP32 sets `board.pin.tracker[TRACKER_1].led = 2` and `board.pin.mode.manual = 2`. `Tracker::init()` configures the manual pin as `INPUT`, while validation only checks that pins are non-zero and does not detect duplicate assignments.
- Impact: this is at least an ambiguous board map. If tracker LEDs are wired or implemented later, GPIO 2 will be shared between an input mode selector and a tracker LED signal.
- Verification: `assertSetting()` validates presence/range but not uniqueness across board pins.

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
- No validation test for duplicate pin assignments in board settings.
- No motor behavior test proving that timeout-aware deploy/retract calls keep motors active for the intended window.

## Suggested Next Fixes

1. Align Uno/Nano `settingInit()` with the shared `Setting* settingInit()` contract.
2. Decide whether command states are events or latches; if they are events, clear them after `Tracker` consumes STOP/RESET.
3. Move either the ESP32 tracker LED or manual-mode pin away from GPIO 2, then add duplicate-pin validation.

## Conclusion

The tracker control loop is in better shape than the earlier review suggested, but the board matrix now needs attention. The highest-priority stabilization item is the Uno/Nano `settingInit()` API mismatch because it blocks those targets from compiling. After that, command-state consumption and ESP32 pin uniqueness are the main runtime risks to clarify.