# Seeed Arduino SPA06-003 Library

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Arduino CI](https://github.com/Seeed-Studio/Seeed_Arduino_SPA06/actions/workflows/run-ci-arduino.yml/badge.svg)](https://github.com/Seeed-Studio/Seeed_Arduino_SPA06/actions)
[![Issues](https://img.shields.io/github/issues/Seeed-Studio/Seeed_Arduino_SPA06.svg)](https://github.com/Seeed-Studio/Seeed_Arduino_SPA06/issues)

This is an Arduino library for the **SPA06-003** pressure and temperature sensor. The library provides a comprehensive and user-friendly API to interface with the sensor, enabling high-precision measurements for a wide range of applications, including weather monitoring, altitude tracking, and industrial systems.

---

## Features

- **Pressure and Temperature Measurement**:
  - High-precision pressure and temperature readings.
  - Supports both single-shot and continuous measurement modes.

- **Configurable Sampling**:
  - Adjustable oversampling and measurement rates for optimized performance.
  - Configurable pressure and temperature sources.

- **Interrupt Support**:
  - Hardware interrupt support for real-time applications.
  - Configurable interrupt thresholds.

- **I2C Communication**:
  - Fully compatible with the Arduino Wire library.
  - Supports both default and alternate I2C addresses.

- **SPI Communication**
  - Fully compatible with the Arduino SPI library.

- **Cross-Platform Compatibility**:
  - Compatible with all Arduino boards and architectures.

---

## Installation

### Using the Arduino Library Manager
1. Open the Arduino IDE.
2. Navigate to the Arduino Library Manager
3. Search for `Seeed Arduino SPA06`.
4. Click **Install**.

### Manual Installation
1. Clone or download this repository.
2. Copy the folder into your Arduino libraries directory (e.g., `~/Arduino/libraries/`).
3. Restart the Arduino IDE.

---

## Getting Started

### Hardware Setup By I2C
1. Connect the SPA06-003 sensor to your Arduino board via I2C:
   - **VCC**: 3.3V or 5V (depending on your board).
   - **GND**: Ground.
   - **SCL**: I2C clock pin.
   - **SDA**: I2C data pin.

2. Ensure the I2C address matches the sensor configuration (default: `0x77`).

### Hardware Setup By SPI
1. Connect the SPA06-003 sensor to your Arduino board via SPI:
   - **VCC**: 3.3V or 5V (depending on your board).
   - **GND**: Ground.
   - **CS** : Use the SS pin defined by yourself
   - **SCK**: SPI clock pin.
   - **SDO**: SPI MISO pin.
   - **SDI**: SPI MOSI pin.

### Example Code

#### Basic Pressure and Temperature Reading
```cpp
#include <Wire.h>
#include "SPL07-003.h"

// Define SPL07-006 I2C address
#define SPL07_ADDR SPL07_ADDR_DEF // Default I2C address (SDO=high)
// #define SPL07_ADDR SPL07_ADDR_ALT // Alternate I2C address (SDO=low)

// Create SPL07-003 sensor instance
SPL07_003 spl;

//HardwareSerial SerialOut(PA10, PA9); //for STM32F103C8Tx

// Runs at startup
void setup() {

  // Begin Serial
  Serial.begin(115200);

  // Configure & start I2C
  //Wire.setSDA(PB7); //for STM32F103C8Tx
  //Wire.setSCL(PB6); //for STM32F103C8Tx
  Wire.begin();

  // Connect to SPL07-003
  if (spl.begin(SPL07_ADDR,&Wire) == false) {
    Serial.println("Error initializing SPL07-003 :(");
    while (1) {}
  }//if
  Serial.println("Connected to SPL07-003! :)");

  // Set pressure & temperature sampling settings
  spl.setPressureConfig(SPL07_4HZ, SPL07_32SAMPLES);
  spl.setTemperatureConfig(SPL07_4HZ, SPL07_1SAMPLE);

  // Set SPL07-003 to continuous measurements
  spl.setMode(SPL07_CONT_PRES_TEMP);

}//setup()


// Runs continuously
void loop() {

  // Wait for available reading
  if (spl.pressureAvailable() || spl.temperatureAvailable()) {
    // Read latest values
    double pres = spl.readPressure();
    double temp = spl.readTemperature();
    double altitude = spl.calcAltitude();
    // Print to serial
    Serial.print("Pres: ");
    Serial.print(pres, 3);
    Serial.print(" Pa, Temp: ");
    Serial.print(temp, 3);
    Serial.print(" C, Altitude: ");
    Serial.print(altitude, 3);
    Serial.println(" m");
  }//if

}//loop()
```

For more examples, refer to the `examples` folder in this repository.

---

## API Reference

### Initialization
- `bool begin(uint8_t addr = SPL07_ADDR_DEF, TwoWire *wire = &Wire, uint8_t id = SPL07_EXPECTED_ID);`
  - Initializes the sensor and reads calibration coefficients.
  - Returns `true` if successful, `false` otherwise.

### Measurement
- `double readPressure();`
  - Reads the current pressure in Pascals (Pa).
- `double readTemperature();`
  - Reads the current temperature in degrees Celsius (°C).
- `double calcAltitude();`
  - Calculate altitude (M).

### Configuration
- `void setMode(SPL07_Modes mode);`
  - Set the sensor mode.
- `void setPressureConfig(SPL07_Measure_Rates rate, SPL07_Oversample_Rates oversample);`
  - Configures pressure oversampling and measurement rate.
- `void setTemperatureConfig(SPL07_Measure_Rates rate, SPL07_Oversample_Rates oversample);`
  - Configures temperature oversampling and measurement rate.
- `void setTemperatureSource(SPL07_Temperature_Source src);`
  - Configure the source of the temperature sensor.

### Interrupts
- `void setInterruptActiveHigh(bool activeHigh);`
  - Sets if the interrupt pin should function as active high or active low.
- `void configureInterrupt(SPL07_Interrupt_Options opt);`
  - Configures the interrupt function
- `uint8_t getInterruptStatus();`
  - Returns the current interrupt register value annd resets the interrupt status register when run.

---

## Contributing

We welcome contributions to improve this library! Please follow these steps:
1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Submit a pull request with a detailed description of your changes.

---

## License

This library is licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0). You are free to use, modify, and distribute this library under the terms of the license.

---

## Support

For questions or issues, please open an [issue](https://github.com/Seeed-Studio/Seeed_Arduino_SPA06/issues) on GitHub. For additional support, visit the [Seeed Studio Forum](https://forum.seeedstudio.com/).

---

## Acknowledgments

This library is maintained by [Seeed Studio](https://www.seeedstudio.com/). Special thanks to the open-source community for their contributions and feedback.


## Source Statement

This project is based on [SPL07-003-Arduino-Library](https://github.com/Kenneract/SPL07-003-Arduino-Library) (created by [Kenneract](https://github.com/Kenneract))