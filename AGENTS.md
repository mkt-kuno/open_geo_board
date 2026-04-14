# AGENTS.md - OpenGeoBoard Development Guide

## Overview

OpenGeoBoard is an Arduino Uno/Nano R4 firmware for an AD/DA board with:
- 16ch Analog Input (8x HX711 load cells, 2x ADS1115 ADCs)
- 8ch Analog Output (4x GP8403 DACs)
- ModbusRTU communication (Serial USB, 38400 baud, Slave ID: 1)

## Build Commands

### Arduino CLI (Recommended)

```bash
# Install
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
export PATH="$HOME/.arduino-cli/bin:$PATH"

# Setup
arduino-cli config init
arduino-cli core update-index
arduino-cli core install arduino:renesas_uno:unor4nano

# Install libraries
arduino-cli lib install "HX711" "ADS1115_WE" "DFRobot_GP8403" "ModbusRTUSlave"

# Build
arduino-cli compile --board arduino:renesas_uno:unor4nano --output-dir ./build .
```

### Single File Build / Verification

```bash
arduino-cli compile --verify --board arduino:renesas_uno:unor4nano .
```

### GitHub Actions

CI runs on push to main and pull requests. See `.github/workflows/build.yml` for details.

### Linting / Testing

No dedicated linter or automated tests. Use `arduino-cli compile --verify` for syntax checking.
Manual testing via Modbus client or hardware verification.

## Code Style Guidelines

### File Organization

- Single main sketch file: `open_geo_board.ino`
- For larger projects, organize into `.h` files in same directory
- Keep related functionality together (sensors, DACs, etc.)

### Naming Conventions

| Element | Style | Example |
|---------|-------|---------|
| Defines/Macros | UPPER_CASE | `#define HX711_NUM 8` |
| Constants (const) | UPPER_CASE | `const uint8_t hx711_dout[] = {2, 4, ...}` |
| Variables | snake_case | `inputReg`, `ads_current_channel` |
| Functions | snake_case | `void setup()`, `void loop()` |
| Classes/Libraries | PascalCase | `HX711`, `ADS1115_WE` |
| Hardware pins | hx/ads/gp prefix | `hx711_dout`, `hx711_sck` |

### Indentation

- Use **tabs** (not spaces) - matches Arduino IDE default
- Configure your editor: tab size = 2 or 4 spaces equivalent

### Comments

```cpp
// C++ style single-line comments (preferred)
// Section headers with separator:
//====================================================================
// setup()
//====================================================================

/* Block comments for file headers
 * Multi-line description
 */
```

### Imports

Order (group by blank lines):
1. Standard Arduino (`Arduino.h`, `Wire.h`)
2. External libraries (HX711, ADS1115_WE, etc.)
3. Local includes

```cpp
#include <Arduino.h>
#include <Wire.h>
#include "HX711.h"
#include <ADS1115_WE.h>
#include <DFRobot_GP8403.h>
#include <ModbusRTUSlave.h>
```

### Type Usage

| Type | Usage |
|------|-------|
| `int8_t`, `uint8_t` | Small integers, array indices |
| `int16_t`, `uint16_t` | Modbus registers, ADC values |
| `int32_t`, `uint32_t` | Timing, large counts |
| `long` | HX711 raw read values |
| `unsigned long` | Millis(), timeouts |

### Error Handling

- No try/catch (Arduino doesn't support exceptions)
- Use polling-based patterns (check `.is_ready()`, `.isBusy()`)
- Initialize all peripherals; handle init failures silently in loop
- Consider return codes for critical operations

### Timing Patterns

Use non-blocking delays via `millis()`:
```cpp
unsigned long lastRead = 0;
const unsigned long READ_INTERVAL = 100;
if (millis() - lastRead >= READ_INTERVAL) { lastRead = millis(); }
```

### Device Management

Use arrays for multiple instances:
```cpp
#define HX711_NUM 8
HX711 hx711[HX711_NUM];
const uint8_t hx711_dout[HX711_NUM] = {2, 4, 6, 8, 10, 12, 14, 16};
const uint8_t hx711_sck[HX711_NUM]  = {3, 5, 7, 9, 11, 13, 15, 17};
```

### Modbus Register Layout

- Input Registers (0-15): AI0-AI15
- Holding Registers (0-7): AO0-AO7 (mV values)

### Architecture Notes

- **Event-driven loop**: `loop()` runs continuously
- **Task distribution**: Spread work across loop iterations
- **I2C**: Shared bus; use `Wire.begin()` and 400kHz clock
- **Serial**: ModbusRTU on USB Serial (38400 baud)

## Developer Setup

1. Install Arduino CLI or Arduino IDE
2. Install required libraries via Library Manager
3. Configure board: Arduino Uno R4 or Nano R4
4. Connect hardware and upload

## License

GPLv3 - See LICENSE file for details.

## References

- [Arduino CLI Documentation](https://arduino.github.io/arduino-cli/latest/)
- [HX711 Library](https://github.com/bogde/HX711)
- [ADS1115_WE Library](https://github.com/wollewald/ADS1115_WE)
- [DFRobot_GP8403 Library](https://github.com/DFRobot/DFRobot_GP8403)
- [ModbusRTUSlave Library](https://github.com/CMB27/ModbusRTUSlave)