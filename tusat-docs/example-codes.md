# OBC, ADCS and Ground Station Arduino Code Example
## Software Used
<!-- <figure align="center" style="display : flex; flex-direction: row;justify-content: space-evenly; align-items: center; ">
    <img src="https://images-eds-ssl.xboxlive.com/image?url=4rt9.lXDC4H_93laV1_eHM0OYfiFeMI2p9MWie0CvL99U4GA1gf6_kayTt_kBblFwHwo8BW8JXlqfnYxKPmmBR1mpxxSHRjAeQKNY.14SpGglSw9Q_T17Vj6iz5SLNOCf__D3E7g2HXS_dGjwUnSQiy8CMy2I7RwP_CyVWBj74g-&format=source" alt="arduino-ide">
    <img src="https://framerusercontent.com/images/nga8AjlWVtdS8EfppGaOXtfk6X0.png?scale-down-to=512&width=1205&height=213" alt="arduino-ide" ><img>
    <img src="https://www.st.com/bin/ecommerce/api/image.PF265487.en.feature-description-include-personalized-no-cpn-large.jpg" ><img>
</figure> -->

- [Arduino IDE](https://www.arduino.cc/en/software/) : Tested on version **2.3.6**
- [Serial Studio](https://github.com/Serial-Studio/Serial-Studio/releases/tag/v3.1.3) : Recommend version **3.1.3** and older (still free to use)
- [STM32 CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)
- [STM32 Arduino Core](https://github.com/stm32duino/Arduino_Core_STM32/wiki)

## Arduino Library used

This repository contains example Arduino sketches designed to demonstrate how to use various hardware modules together, including SD storage, radio communication, and real-time clock functionality.
To ensure the examples compile and run correctly on your board, you must install several external libraries.
The list below provides the required libraries along with a brief explanation of what each one does and why it is needed for the example projects.
> These library can be installed from the Arduino IDE's library manager

<figure align="center">
  <img src="/images/image.png" alt="sdfat">
  <figcaption><a href="https://github.com/adafruit/SdFat">SdFat - Adafruit Fork</a></figcaption>
</figure>

SdFat is an advanced SD card file system library that provides fast and reliable read/write access to SD cards using SPI.
It is more efficient and stable than the default SD library, especially when working with large files or continuous data logging.
This project uses SdFat to store sensor data and system logs on the SD card.

<figure align="center">
  <img src="/images/image-1.png" alt="sdfat">
  <figcaption><a href="https://github.com/jgromes/RadioLib">RadioLib</a></figcaption>
</figure>
RadioLib is a versatile radio communication library that supports a wide range of wireless modules, including LoRa, FSK, and other RF transceivers.
It handles packet encoding, modulation settings, and radio control, making it easy to implement long-range or low-power wireless communication.
This project uses RadioLib to manage communication with the LoRa module.

<figure align="center">
  <img src="/images/image-2.png" alt="sdfat">
  <figcaption><a href="https://github.com/Seeed-Studio/Grove_High_Precision_RTC_PCF85063TP">Grove - High Precision RTC</a></figcaption>
</figure>
Grove High Precision RTC (PCF85063TP) is a real-time clock library used to keep accurate time even when the microcontroller is powered off.
It provides timestamping for files, logs, and events, ensuring the system always maintains correct date and time information.
This project uses the RTC to timestamp sensor readings and system activity.

## Example Codes
### OBC : On-board Computer Example Codes
- [SD Card Example](OBC/OBC_SD_Card/OBC_SD_Card.ino)
- [I2C Scan Example](OBC/OBC_I2C_Scan/OBC_I2C_Scan.ino)
- [SPI Flash Example](OBC/OBC_Flash/OBC_Flash.ino)
- [OBC EPS Example](OBC/OBC_Poll_EPS/OBC_Poll_EPS.ino)
- [Realtime Clock Example](OBC/OBC_RTC/OBC_RTC.ino)
- [Communication Example](OBC/OBC_Radio_Transmit_Receive/OBC_Radio_Transmit_Receive.ino)
- [Temperature Sensor Example](OBC/OBC_Temperature/OBC_Temperature.ino)
### ADCS : Attitude control and determination Example Codes
- [I2C Scan Example](ADCS/ADCS_I2C_Scan/ADCS_I2C_Scan.ino)
- [SPI Flash Example](ADCS/ADCS_Flash/ADCS_Flash.ino)
- [MTQ Control Example](ADCS/ADCS_MTQ/ADCS_MTQ.ino)
### Ground Station
- [Ground Station Communication Example](GS/GS_TX_RX/GS_TX_RX.ino)

