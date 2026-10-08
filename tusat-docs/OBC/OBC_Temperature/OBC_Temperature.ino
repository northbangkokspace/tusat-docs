#include <Wire.h>

/*######################## Pin Pull-Up Enable ##########################*/
const PinMap PinMap_I2C_SDA[] = {
  { PB_9, I2C1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_PULLUP, GPIO_AF4_I2C1) },
  { NC,   NP,   0 }
};

const PinMap PinMap_I2C_SCL[] = {
  { PB_8, I2C1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_PULLUP, GPIO_AF4_I2C1) },
  { NC,   NP,   0 }
};
/*######################## Pin Pull-Up Enable ##########################*/

#define SENSOR_A 0x39   // First STTS751 sensor
#define SENSOR_B 0x48   // Second STTS751 sensor

#define REG_TEMP_MSB 0x00
#define REG_TEMP_LSB 0x02

void setup() {
  Wire.setSCL(PB8);
  Wire.setSDA(PB9);
  
  Serial.setTx(PD8);
  Serial.setRx(PD9);

  Wire.begin();
  Serial.begin(115200);
  Serial.println("STTS751 Dual Sensor Reader");
}

void loop() {

  float tempA = readTemperature(SENSOR_A);
  float tempB = readTemperature(SENSOR_B);

  Serial.print("Sensor A (0x39): ");
  Serial.print(tempA);
  Serial.println(" C");

  Serial.print("Sensor B (0x48): ");
  Serial.print(tempB);
  Serial.println(" C");

  Serial.println();
  delay(500);
}

// ----------------------------------------
// Read temperature from STTS751 sensor
// ----------------------------------------
float readTemperature(uint8_t address) {
  // Read MSB (integer part)
  Wire.beginTransmission(address);
  Wire.write(REG_TEMP_MSB);
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  int msb = Wire.read();

  // Read LSB (fraction part)
  Wire.beginTransmission(address);
  Wire.write(REG_TEMP_LSB);
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  int lsb = Wire.read();

  // Convert to temperature
  int8_t intPart = (int8_t)msb;
  float fracPart = ((lsb >> 5) & 0x07) * 0.125;

  return intPart + fracPart;
}