#include <Wire.h>
#include "PCF85063TP.h"
const PinMap PinMap_I2C_SDA[] = {
  { PB_9, I2C1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_PULLUP, GPIO_AF4_I2C1) },
  { NC, NP, 0 }
};

// Override weak default SCL pin mapping with internal pull-up
const PinMap PinMap_I2C_SCL[] = {
  { PB_8, I2C1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_PULLUP, GPIO_AF4_I2C1) },
  { NC, NP, 0 }
};
PCD85063TP rtc;//define a object of PCD85063TP class
void setup()
{
  Wire.setSCL(PB8);
  Wire.setSDA(PB9);
  Wire.begin();
  Serial.setTx(PD8);
  Serial.setRx(PD9);
  Serial.begin(115200);
  rtc.begin();
 /*
  rtc.stopClock();
  rtc.fillByYMD(2016,7,5);//Jan 19,2013
  rtc.fillByHMS(14,10,00);//15:28 30"
  rtc.fillDayOfWeek(TUE);//Saturday
  rtc.setTime();//write time to the RTC chip
  rtc.startClock();

*/
  // rtc.setcalibration(1, 32767);  // Setting offset by rtc frequency
  uint8_t ret = rtc.calibratBySeconds(0, -0.000041);
  Serial.print("offset value: ");
  Serial.print("0x");
  Serial.println(ret, HEX);
 
}
void loop()
{
  printTime();
  delay(1000);
}
/*Function: Display time on the serial monitor*/
void printTime()
{
  rtc.getTime();
  Serial.print(rtc.hour, DEC);
  Serial.print(":");
  Serial.print(rtc.minute, DEC);
  Serial.print(":");
  Serial.print(rtc.second, DEC);
  Serial.print("  ");
  Serial.print(rtc.month, DEC);
  Serial.print("/");
  Serial.print(rtc.dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(rtc.year+2000, DEC);
  Serial.print(" ");
  Serial.print(rtc.dayOfMonth);
  Serial.print("*");
  switch (rtc.dayOfWeek)// Friendly printout the weekday
  {
    case MON:
      Serial.print("MON");
      break;
    case TUE:
      Serial.print("TUE");
      break;
    case WED:
      Serial.print("WED");
      break;
    case THU:
      Serial.print("THU");
      break;
    case FRI:
      Serial.print("FRI");
      break;
    case SAT:
      Serial.print("SAT");
      break;
    case SUN:
      Serial.print("SUN");
      break;
  }
  Serial.println(" ");
}