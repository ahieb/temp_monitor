# Temperature Monitor - Software Architecture

## 1. System Overview
A bare-metal embedded system used in a temperature monitoring and
visualization device, with indicators for OK, Warning, and Critical temperature ranges.

## 2. Requirements Summary
* ADC input --> Resolution not specified? Sampling time?
* Low jitter --> deterministic timing --> realtime
* Three gpios for LEDs
* FSM for led states --> no hysteresis?
* Temperature sensor revisions --> keep decimals as integers (int32)
* EEPROM via I2C provides config --> not timing critical

## 3. Open Questions
* Hysteresis for temperature values thresholds?
    -> Assume 2K
* No low temperature warning, only critical state?
    -> Assume this is the case
* Temperature sensor ADC resolution?
    -> Assume 12bit
* What exactly is time-critical, only sampling or signaling via LEDs, too?

## 4. Architecture Layers

The firmware follows a 3-layer architecture separating application logic from hardware
through a HAL interface boundary. Mock drivers implement the HAL for host-based simulation.

### Application Layer (`src/app/`)
`monitor_temperature.c` contains all application logic: system initialisation (`init_system`),
the main polling loop (`monitor_temperature`), ADC-to-temperature conversion, and the LED
state machine (`update_led_fsm`). `main.c` is a thin entry point that calls these two
functions sequentially.

### Hardware Abstraction Layer (`src/hal/`)
Four header-only interfaces define the hardware contract:
- `hal_adc.h` — ADC init and raw temperature read (`uint16_t`)
- `hal_eeprom.h` — EEPROM init, HW revision and serial number read via I2C
- `hal_led.h` — GPIO init and LED state set (Green / Yellow / Red)
- `hal_timer.h` — Timer init and microsecond delay set

The application layer depends only on these interfaces, never on concrete drivers.

### Driver Layer (`src/drivers/`)
Mock implementations (`mock_adc.c`, `mock_eeprom.c`, `mock_led.c`, `mock_timer.c`) satisfy
the HAL interfaces for host-based builds using native GCC. On a real target, these would be
replaced by hardware-specific drivers without touching the application layer.