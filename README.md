# Color Marble Sorter

An automated marble sorting system that identifies marble colors using a TEPT5600 light sensor and sorts them into designated bins using a dual-stepper-motor mechanism. This project combines embedded programming, sensor integration, motor control, and mechanical design to automate the sorting process.

## Overview

The system uses a TEPT5600 light sensor to measure the reflected light from individual marbles. Sensor readings are compared against calibrated thresholds to determine the marble's color.

Once a color is identified, the first stepper motor positions a sorting arm above the appropriate collection bin. A second stepper motor actuates a push mechanism that releases the marble from a gravity-fed holding area into the selected bin.

The entire process is automated and repeats for each marble introduced into the system.

## My Contributions

Working alongside Adam Nguyen, I contributed to the design, development, and testing of the system's mechanical and control components.

Key contributions included:

- Designed custom 3D-printed components in Tinkercad to support the marble staging, sorting, and actuation mechanisms.
- Evaluated part tolerances and mechanical fit to ensure reliable movement and alignment between printed components.
- Assisted in developing and refining the gravity-fed marble holding system.
- Calibrated and tested both stepper motors to achieve consistent and repeatable operation.
- Developed the mechanism that converts rotational motion from a stepper motor into linear motion for controlled marble release.
- Assisted in designing and validating the rotating sorting arm used to position marbles above the correct collection bin.
- Performed system integration testing and iterative mechanical adjustments to improve sorting accuracy and reliability.

## Features

- TEPT5600 light sensor-based color detection
- Sensor calibration for improved classification accuracy
- Gravity-fed marble holding and staging system
- Stepper motor-controlled sorting arm for bin selection
- Independent stepper motor-driven marble release mechanism
- Automated color-based sorting workflow
- Integration of mechanical, electrical, and software subsystems

## Hardware Components

- Arduino Microcontroller
- TEPT5600 Ambient Light Sensor
- 2 Stepper Motors
- Stepper Motor Drivers
- Custom 3D-Printed Components
- Marble Collection Bins

## How It Works

1. A marble enters the gravity-fed holding area.
2. The TEPT5600 sensor measures reflected light from the marble.
3. The Arduino determines the marble's color using calibrated sensor thresholds.
4. The sorting arm rotates to the appropriate destination bin.
5. The release mechanism pushes the marble from the holding area.
6. The marble falls into the selected collection bin.
7. The system resets and waits for the next marble.

## Skills Demonstrated

- Embedded Systems Programming
- Arduino Development
- Sensor Integration
- Stepper Motor Control
- Calibration and Testing
- Mechanical Design
- Automation Systems
- Hardware/Software Integration

## Challenges

- Calibrating the TEPT5600 sensor for reliable color detection
- Synchronizing multiple stepper motors
- Designing a reliable marble release mechanism
- Ensuring consistent sorting accuracy under varying lighting conditions

## Future Improvements

- Support for additional marble colors
- LCD or OLED status display
- Increased sorting speed
- Enhanced color classification algorithms
- Computer vision-based detection system

## Media


## Authors
- Eric Nguyen
- Adam Nguyen
- John Reade Corr
- Dejan Gardner

