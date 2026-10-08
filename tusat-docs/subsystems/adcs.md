# Attitude Determination and Control System (ADCS)

## Overview
The ADCS subsystem determines the satellite's orientation (attitude) in space and applies control torques to stabilize it or point it in a desired direction (e.g., pointing antennas at Earth or solar panels at the sun).

## Core Components
- **Microcontroller (MCU)**: STM32F429ZI

## Sensors (Attitude Determination)

- **Gyroscope & Accelerometer (MPU6050)**:
  - **Duty**: Measures the satellite's angular velocity and linear acceleration.
  - **Interface/Protocol**: I2C (Address: `0x69`).
- **Magnetometer (LIS2MDL)**:
  - **Duty**: Measures the Earth's magnetic field vector to help determine the satellite's orientation relative to Earth.
  - **Interface/Protocol**: I2C (Address: `0x1E`).
- **GPS**:
  - **Duty**: Provides precise position, velocity, and time data.
  - **Interface/Protocol**: UART (TX/RX).
- **Temperature Sensor (STTS751)**:
  - **Duty**: Monitors ADCS board temperature.
  - **Interface/Protocol**: I2C (Address: `0x39`).

## Actuators (Attitude Control)

- **Magnetic Torquers (MTQ 1-3)**:
  - **Duty**: Coils that generate a magnetic dipole moment, interacting with the Earth's magnetic field to produce torque and rotate the satellite.
  - **Control Method**: Driven by **DRV8829** stepper/DC motor drivers. Controlled via **PWM** (for strength) and **DIR** (direction/polarity) pins from the MCU.
- **Voltage/Current Sensors (INA226 x3)**:
  - **Duty**: Monitors the power consumption and health of each MTQ.
  - **Interface/Protocol**: I2C (Addresses: `0x40`, `0x41`, `0x44`).

## Internal Communication & Storage
- **SPI Flash (W25Q128)**: For storing configuration and control logs via SPI.
- **CAN Transceiver (SN65HVD230)**: Connects to the PC104 CAN bus.
- **PC104 Bus**: Communicates with the OBC via UART and CAN.

## Architecture Diagram

```mermaid
flowchart TD
    MCU[STM32F429ZI MCU]
    
    subgraph Sensors I2C Bus
        IMU[MPU6050 Gyro/Accel 0x69]
        Mag[LIS2MDL Magneto 0x1E]
        Temp[STTS751 Temp 0x39]
        INA1[INA226 MTQ1 0x40]
        INA2[INA226 MTQ2 0x41]
        INA3[INA226 MTQ3 0x44]
    end
    
    subgraph Actuators
        Driver1[DRV8829] --> MTQ1[MTQ 1]
        Driver2[DRV8829] --> MTQ2[MTQ 2]
        Driver3[DRV8829] --> MTQ3[MTQ 3]
    end
    
    GPS[GPS Module]
    PC104((PC104 Bus))
    
    MCU <-->|I2C| Sensors_I2C_Bus
    MCU -->|PWM, DIR| Actuators
    MCU <-->|UART| GPS
    MCU <-->|UART, CAN| PC104
```

### Detailed Pinout Diagram
![ADCS Diagram](../diagram/TUSat%20Block%20Diagram.drawio#2)
