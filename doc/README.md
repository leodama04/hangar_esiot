# Smart Drone Hangar

**Embedded Systems and IoT – Assignment #02**
*A.Y. 2025/2026*

---

## Overview

**Smart Drone Hangar** is an embedded system prototype that implements an automated hangar for a drone. It consists of two cooperating subsystems:

- **Drone Hangar** – Arduino-based physical controller
- **Drone Remote Unit (DRU)** – PC-side GUI application communicating via serial line

The system manages the full lifecycle of a drone: rest inside the hangar, take-off, flight, and landing — with integrated temperature-based alarm management.

---

## System Architecture

### Drone Hangar (Arduino)

The Arduino subsystem controls all physical components and implements the core logic through **task-based architecture** and **synchronous Finite State Machines**.

| Component | Type | Role |
|---|---|---|
| DPD | PIR sensor | Detects drone presence near the hangar |
| DDD | Sonar | Measures drone distance inside the hangar |
| HD | Servo motor | Controls the hangar door |
| LCD | I2C LCD | Displays system status to the operator |
| L1 | Green LED | ON when drone is inside |
| L2 | Green LED | Blinks during take-off and landing |
| L3 | Red LED | ON during alarm state |
| RESET | Tactile button | Resets the system after an alarm |
| TEMP | Analog sensor | Monitors temperature inside the hangar |

### Drone Remote Unit (PC)

A GUI application that acts as a bridge to the drone. It communicates with the Arduino over serial and allows the operator to:

- Send **take-off** and **landing** commands to the hangar
- Monitor the **drone state** (rest, taking off, operating, landing)
- Monitor the **hangar state** (normal, alarm)
- View the **current distance to ground** during landing

---

## Behaviour

### Startup

The system boots with the door **closed** and the drone assumed to be **inside at rest**. L1 is ON, L2 and L3 are OFF, and the LCD shows `DRONE INSIDE`.

### Take-off Phase

1. The DRU sends the take-off command.
2. HD opens; LCD shows `TAKE OFF`; L2 starts blinking (period: 0.5 s).
3. The system monitors DDD: when distance > `D1` for more than `T1` seconds, the drone is considered gone.
4. HD closes; LCD shows `DRONE OUT`; L2 turns OFF.

### Landing Phase

1. The DRU sends the landing command.
2. If DPD detects the drone, HD opens; LCD shows `LANDING`; L2 starts blinking.
3. The system monitors DDD: when distance < `D2` for more than `T2` seconds, the drone is considered landed.
4. HD closes; LCD shows `DRONE INSIDE`; L2 turns OFF.

### Temperature Monitoring

Temperature is monitored whenever the drone is inside the hangar.

| Condition | Duration | Effect |
|---|---|---|
| Temp ≥ `Temp1` | > `T3` s | **Pre-alarm**: new take-offs/landings suspended |
| Temp drops < `Temp1` | — | Returns to normal operation |
| Temp ≥ `Temp2` | > `T4` s | **Alarm**: HD closed, L3 ON, LCD `ALARM` |
| RESET pressed | — | System returns to normal |

If an alarm triggers while the drone is outside, the `ALARM` message is also sent to the DRU. All operations are suspended until RESET is pressed.

---

## Finite State Machines

The Arduino software is structured around four FSMs:

### 1. Take-off FSM
`Waiting for takeoff msg` → `Waiting door to open` → `Waiting drone to exit` → `Waiting to close door` → `Waiting door to close`

### 2. Landing FSM
`Waiting for landing msg` → `Detecting drone` → `Waiting door to open` → `Waiting drone to enter` → `Waiting to close door` → `Waiting door to close`

### 3. Alarm FSM
`Normal state` → `Waiting pre-alarm` → `Pre-alarm state` → `Waiting alarm` → `[Waiting door to close]` → `Alarm state`

Transitions back to normal occur when temperature drops below `Temp1`, or when RESET is pressed from the alarm state.

### 4. Blinking FSM (L2)
`Turning on` ↔ `Turning off` — L2 blinks while take-off or landing is in progress.

---

## Configuration Parameters

These values are intentionally left flexible for prototyping and testing:

| Parameter | Description |
|---|---|
| `D1` | Distance threshold for detecting drone exit (take-off) |
| `D2` | Distance threshold for detecting drone landing |
| `T1` | Time drone must stay beyond `D1` to confirm exit |
| `T2` | Time drone must stay below `D2` to confirm landing |
| `T3` | Time above `Temp1` before entering pre-alarm |
| `T4` | Time above `Temp2` before entering full alarm |
| `Temp1` | Pre-alarm temperature threshold |
| `Temp2` | Full alarm temperature threshold (> `Temp1`) |

---

## Project Structure

```
assignment-02/
├── drone-hangar/        # Arduino C++/Wiring source code
├── drone-remote-unit/   # PC-side application (Python)
└── doc/                 # Documentation
```

---

## Implementation Notes

- The Arduino program follows a **task-based architecture**: each logical concern (take-off, landing, alarm, blinking) is implemented as an independent task with its own FSM.
- FSMs are **synchronous**: state transitions are evaluated at each task tick, with no blocking calls.
- Serial communication between Arduino and DRU uses a simple message protocol.
- The DRU GUI reflects real-time state changes received from the Arduino.

---

