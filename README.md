# Microprocessors Lab – ATMega328PB IoT Project

This repository contains the work completed during a university course on embedded systems, focusing on programming in Assembly and C, and programming the ATMega328PB microcontroller.  
The repository includes source code for all 8 laboratory exercises, progressing from basic assembly tasks to a final IoT project integrating sensors, actuators, and network communication.

> **Note:** Lab descriptions and reference solutions are provided as PDFs in Greek, as required by the course. The included source code is documented in English and demonstrates the core implementations of each lab.

## Technologies Used
- **Programming Languages:** Assembly, C
- **Microcontroller:** ATMega328PB
- **IDE:** MPLAB X IDE

## Labs Overview

### Lab 1: Basic Assembly Programming and Time Delays
- Implemented assembly code to create accurate time delays using `rcall` and register pairs.
- Calculated logical functions in a loop with variable increments.
- Simulated a moving cart using `PORTD` bits, with direction control and 1-second stops when changing direction.
- Tested and simulated using MPLAB X IDE.

### Lab 2: External Interrupts and Button Debouncing
- Handled external interrupts (INT0, INT1) in Assembly.
- Implemented button press detection with debounce handling.
- Created a counter and LED display controlled by button inputs.
- Developed implementations in both Assembly and C.

### Lab 3: Lighting Automation and PWM Control
- Used interrupts and Timer1 to control lights with reset behavior.
- Generated PWM signals on PB1 to control brightness in 8% steps.
- Adjusted PWM frequency (125Hz–1kHz) via button inputs.

### Lab 4: ADC Handling and CO Gas Detection
- Performed ADC conversions via interrupt-driven and polling methods.
- Monitored CO gas levels and updated LEDs and LCD warnings.
- Generated a 5kHz PWM signal with duty cycle based on button input.
- Displayed sensor readings on LCD.

### Lab 5: Logical Functions, I/O Expansion, and Display
- Implemented logical functions and displayed outputs on LEDs.
- Controlled LEDs via an I/O expander (PCA9555).
- Optionally displayed student name and surname on LCD.

### Lab 6: Keypad Scanning and Electronic Lock
- Scanned a keypad and mapped pressed keys to ASCII characters.
- Simulated an electronic lock with LED status indicators.
- Incorporated debouncing and error signaling via LED blinking.

### Lab 7: Temperature Sensing and Communication
- Communicated with DS1820 sensor using the one-wire protocol.
- Measured and retrieved temperature with 0.5°C precision.
- Displayed temperature on LCD; showed "NO Device" if no sensor detected.

### Lab 8: IoT System Integration (Final Project)
#### ESP8266 Network Connection
- Connected ATMega328PB to ESP8266 via UART and verified network status.
- Displayed connection results on LCD.

#### URL Command and Data Transmission
- Sent HTTP commands to set URL for sensor data reporting.
- Integrated sensor data (temperature and pressure) into the payload.

#### Status Logic
- Managed “NURSE CALL,” “CHECK PRESSURE,” and “CHECK TEMP” statuses based on inputs and sensor readings.
- Displayed real-time status on the LCD.

#### Sensor and Payload Integration
- Temperature and pressure data were formatted and transmitted via ESP8266 to the server.

## Summary

This repository demonstrates progressive development from basic embedded programming to a fully integrated IoT system.  
It highlights assembly and C programming, peripheral handling, sensor interfacing, and network communication.  
Each lab’s source code is included and documented in English for clarity and reproducibility.

## License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.
