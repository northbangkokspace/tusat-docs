#include "string.h"
#define FEND 0xC0
#define FESC 0xDB
#define TNC_DATA 0x00
#define TFEND 0xDC
#define TFESC 0xDD

typedef enum {
  EPS_CMD_NONE = 0x00,
  EPS_GET_INA226 = 0x01,
  EPS_GET_ADM1177 = 0x02,
  EPS_GET_OUTPUT = 0x03,
  EPS_GET_TEMP_BAT = 0x04,
  EPS_SET_OUTPUT = 0x05,
  EPS_GET_PARAM = 0xFF,
  EPS_SENSOR_INIT = 0xFE

} EpsDriverType_t;

typedef struct vi_sen {
  int mV;
  int mA;
  int state;
} EpsViSensor_t;

// Result buffer length (you can adjust this)
#define MAX_KISS_DATA_LEN 100
HardwareSerial epsSerial(PE0, PE1);
void setup() {
  Serial.setTx(PD8);
  Serial.setRx(PD9);
  epsSerial.begin(115200);
  Serial.begin(115200);
  // put your setup code here, to run once:
}

void loop() {
  EpsViSensor_t in_sen = eps_get_input_sensor(0);
  EpsViSensor_t out_sen = eps_get_output_sensor(0);
  int state = eps_get_output_state(0);
  float temp = eps_get_temperature(0);

  Serial.printf("Vin=%d mV, Iin=%d mA, Vout=%d mV, Iout=%d mA, State=%d, Temp=%.2f C\r\n",
                in_sen.mV, in_sen.mA, out_sen.mV, out_sen.mA, state, temp);

  delay(1000);
}


static bool eps_send_and_receive(uint8_t *tx, int tx_len,
                                 uint8_t *cmd_out,
                                 uint8_t *data_out, int *data_len) {
  uint8_t recv[120];
  int recvLen = 0;

  // Send request
  epsSerial.write(tx, tx_len);

  // Small wait for response
  delay(10);

  // Read response
  unsigned long t0 = millis();
  while ((millis() - t0) < 100) {  // timeout ~50ms
    while (epsSerial.available()) {
      if (recvLen < sizeof(recv)) {
        recv[recvLen++] = epsSerial.read();
      }
    }
  }

  if (recvLen == 0) return false;

  // Decode
  return kiss_rx(recv, recvLen, cmd_out, data_out, data_len);
}


EpsViSensor_t eps_get_input_sensor(uint8_t channel) {
  uint8_t tx[10];
  int len = 0;
  uint8_t cmd;
  uint8_t data[100];
  int dataLen;

  tx[len++] = FEND;
  tx[len++] = TNC_DATA;
  tx[len++] = EPS_GET_INA226;
  tx[len++] = channel;
  tx[len++] = FEND;

  EpsViSensor_t temporary;
  if (!eps_send_and_receive(tx, len, &cmd, data, &dataLen)) {
    temporary.state = -1;
    return temporary;
  }

  if (dataLen < 4) {
    temporary.state = -1;
    return temporary;
  }

  uint16_t mv_raw = (data[1] << 8) | data[0];
  uint16_t ma_raw = (data[3] << 8) | data[2];

  temporary.mV = (int)mv_raw;
  temporary.mA = (int)ma_raw;
  return temporary;
}

EpsViSensor_t eps_get_output_sensor(uint8_t channel) {
  uint8_t tx[20];
  int len = 0;
  uint8_t cmd;
  uint8_t data[100];
  int dataLen;

  tx[len++] = FEND;
  tx[len++] = TNC_DATA;
  tx[len++] = EPS_GET_ADM1177;
  tx[len++] = channel;
  tx[len++] = FEND;

  EpsViSensor_t temporary;
  if (!eps_send_and_receive(tx, len, &cmd, data, &dataLen)) {
    temporary.state = -1;
    return temporary;
  }

  if (dataLen < 4) {
    temporary.state = -1;
    return temporary;
  }

  int mv_raw = (data[1] << 8) | data[0];
  int ma_raw = (data[3] << 8) | data[2];

  temporary.mV = (int)mv_raw;
  temporary.mA = (int)ma_raw;
  return temporary;
}

int eps_get_output_state(uint8_t channel) {
  uint8_t tx[10];
  int len = 0;
  uint8_t cmd;
  uint8_t data[10];
  int dataLen;

  tx[len++] = FEND;
  tx[len++] = TNC_DATA;
  tx[len++] = EPS_GET_OUTPUT;
  tx[len++] = channel;
  tx[len++] = FEND;

  if (!eps_send_and_receive(tx, len, &cmd, data, &dataLen)) {
    return -1;
  }

  if (dataLen < 1) return -1;

  return data[0];  // 0 = off, 1 = on
}

float eps_get_temperature(uint8_t channel) {
  uint8_t tx[10];
  int len = 0;
  uint8_t cmd;
  uint8_t data[10];
  int dataLen;

  tx[len++] = FEND;
  tx[len++] = TNC_DATA;
  tx[len++] = EPS_GET_TEMP_BAT;
  tx[len++] = channel;
  tx[len++] = FEND;

  if (!eps_send_and_receive(tx, len, &cmd, data, &dataLen)) {
    return -999.0f;
  }

  if (dataLen < 2) return -999.0f;
  float temp;
  memcpy(&temp, data, 4);
  return temp;
}


int eps_set(uint8_t channel, uint8_t state) {
  uint8_t data[10];
  int reqLen = 0;
  data[reqLen++] = FEND;
  data[reqLen++] = TNC_DATA;
  data[reqLen++] = EPS_SET_OUTPUT;
  data[reqLen++] = channel;
  data[reqLen++] = state;
  data[reqLen++] = FEND;
  epsSerial.write(data, reqLen);
  return reqLen;
}


// Decode a KISS frame into cmd and data
bool kiss_rx(byte *kiss_frame, int frame_len, byte *cmd_out, byte *data_out, int *data_len_out) {
  if (frame_len < 4) {
    // Too short to be valid (FEND, TNC, CMD, FEND)
    return false;
  }

  // Check start and end
  if (kiss_frame[0] != FEND || kiss_frame[frame_len - 1] != FEND) {
    return false;
  }

  // Strip FEND bytes
  int i = 1;  // Start after first FEND
  int end = frame_len - 1;

  if ((end - i) < 2) {
    return false;  // Not enough for command and data
  }

  // TNC_DATA is kiss_frame[i] — ignored
  *cmd_out = kiss_frame[i + 1];  // Command is second byte after FEND
  byte *raw_data = &kiss_frame[i + 2];
  int raw_len = end - (i + 2);

  // Unescape
  int di = 0;  // Index for data_out
  for (int j = 0; j < raw_len; j++) {
    if (raw_data[j] == FESC) {
      j++;
      if (j >= raw_len) {
        return false;  // Incomplete escape sequence
      }
      if (raw_data[j] == TFEND) {
        data_out[di++] = FEND;
      } else if (raw_data[j] == TFESC) {
        data_out[di++] = FESC;
      } else {
        return false;  // Invalid escape
      }
    } else {
      data_out[di++] = raw_data[j];
    }

    if (di >= MAX_KISS_DATA_LEN) {
      return false;  // Overflow
    }
  }

  *data_len_out = di;
  return true;
}
