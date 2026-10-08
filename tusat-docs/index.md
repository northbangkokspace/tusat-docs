# TUSat Documents
## Welcome to TUSat Documents
This document will explain a brief overview of TUSat subsystem operation and functions

The satellite consists of 4 main subsystems:

1. [**OBC + Commu**](subsystems/obc-commu.md): On-Board Computer with SD card, flash memory, temperature sensor, and communication module (Radio RFM98PW).
2. [**ADCS**](subsystems/adcs.md): Attitude Determination and Control System featuring 3-axis MTQ with PWM and direction control. Each MTQ's current and voltage are monitored through an INA226 sensor. It also includes an external gyro/accelerometer, a magnetometer board, and a GPS.
3. [**EPS**](subsystems/eps.md): Electrical Power System, which is pre-programmed and can be queried for temperature data.
4. [**Payload**](subsystems/payload.md): Payload subsystem utilizing a Raspberry Pi Zero and a LoRa module.

### Overall System Architecture

```mermaid
flowchart TB
    subgraph TUSat["TUSat Subsystems"]
        direction TB
        
        OBC["<b>OBC + Commu</b>
        (STM32F429ZI)
        - Radio Module
        - SD Card & Flash
        - Temp Sensors"]

        ADCS["<b>ADCS</b>
        (STM32F429ZI)
        - 3-Axis MTQ & INA226
        - Gyro/Accel & Magneto
        - GPS"]

        EPS["<b>EPS</b>
        - Pre-programmed
        - Temp Data Query"]

        Payload["<b>Payload</b>
        (Raspberry Pi Zero)
        - LoRa Module"]
        
        BUS(("<b>PC104 Bus</b>"))
    end

    %% Physical Bus Connections
    OBC <==>|UART, CAN| BUS
    ADCS <==>|UART, CAN| BUS
    Payload <==>|UART, CAN| BUS
    EPS <==>|UART| BUS

    %% Logical Connections (via PC104)
    OBC -.->|OBC TX/RX| Payload
    OBC -.->|BUS OBC TX/RX| ADCS
    OBC -.->|EPS TX/RX| EPS

    %% Links to subsystem pages
    click OBC href "subsystems/obc-commu/" "Go to OBC + Commu documentation"
    click ADCS href "subsystems/adcs/" "Go to ADCS documentation"
    click EPS href "subsystems/eps/" "Go to EPS documentation"
    click Payload href "subsystems/payload/" "Go to Payload documentation"
```