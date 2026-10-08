#include <RadioLib.h>
#include <SPI.h>
#define RADIOLIB_DEBUG

SPIClass RADIO_SPI(PA7, PA6, PA5, -1);
SX1278 radio = new Module(PA4, PF2, PE2, -1, RADIO_SPI);

volatile bool receivedFlag = false;

void setFlag(void) {
  receivedFlag = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.setTx(PD8);
  Serial.setRx(PD9);
  RADIO_SPI.begin();
  Serial.begin(115200);
  int state = radio.beginFSK(435);
  // radio.setDataShaping(RADIOLIB_SHAPING_0_5);
  // radio.setFrequencyDeviation(1.2);
  radio.setOutputPower(8);
  uint8_t syncWord[] = { 0x01, 0x23, 0x45, 0x67,
                         0x89, 0xAB, 0xCD, 0xEF };
  state = radio.setSyncWord(syncWord, 8);
  delay(2000);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // Bind hardware transceiver event to ISR flag
  radio.setDio0Action(setFlag, RISING);

  // Trigger background reception
  int state_rx = radio.startReceive();
  if (state_rx == RADIOLIB_ERR_NONE) {
    Serial.println(F("[SYSTEM] RF Interrupt enabled. Listening..."));
  } else {
    Serial.print(F("[ERROR] Failed to start receive. Code: "));
    Serial.println(state_rx);
  }
}

int count = 0;

void loop() {
  if (receivedFlag) {
    receivedFlag = false; // Reset flag

    String rxString;
    int state = radio.readData(rxString);

    if (state == RADIOLIB_ERR_NONE) {
      Serial.print(F("[RX] Data Received: "));
      Serial.println(rxString);
    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
      Serial.println(F("[WARNING] RF CRC Mismatch! Bad signal received."));
    } else {
      Serial.print(F("[ERROR] Receive failed, code: "));
      Serial.println(state);
    }

    // Reactivate non-blocking hardware receiver state
    radio.startReceive();
    receivedFlag = false;
  }

  Serial.print(F("[SX1278] Transmitting packet ... "));

  // you can transmit C-string or Arduino string up to
  // 255 characters long
  String str = "Hello World! #" + String(count++);
  int state = radio.transmit(str);

  // you can also transmit byte array up to 256 bytes long
  /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    int state = radio.transmit(byteArr, 8);
  */

  if (state == RADIOLIB_ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F(" success!"));

    // print measured data rate
    Serial.print(F("[SX1278] Datarate:\t"));
    Serial.print(radio.getDataRate());
    Serial.println(F(" bps"));

  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    // the supplied packet was longer than 256 bytes
    Serial.println(F("too long!"));

  } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
    // timeout occurred while transmitting packet
    Serial.println(F("timeout!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  // Re-enable receive mode & clear flag (transmit triggers the DIO0 interrupt)
  radio.startReceive();
  receivedFlag = false;

  // wait for a second before transmitting again
  delay(1000);
}
