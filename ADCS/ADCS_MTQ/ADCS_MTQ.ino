#define MTQ1_SLEEP PG6
#define MTQ1_DIR PG7
#define MTQ1_PWM PC7
#define MTQ2_SLEEP PD0
#define MTQ2_DIR PD1
#define MTQ2_PWM PC8
#define MTQ3_SLEEP PE12
#define MTQ3_DIR PE13
#define MTQ3_PWM PC6

#define MTQ_SLEEP LOW
#define MTQ_ACTIVE HIGH

void setup() {
  Serial.setTx(PD8);
  Serial.setRx(PD9);
  Serial.begin(115200);
  pinMode(MTQ1_SLEEP, OUTPUT);
  pinMode(MTQ1_DIR, OUTPUT);
  pinMode(MTQ1_PWM, OUTPUT);
  pinMode(MTQ2_SLEEP, OUTPUT);
  pinMode(MTQ2_DIR, OUTPUT);
  pinMode(MTQ2_PWM, OUTPUT);
  pinMode(MTQ3_SLEEP, OUTPUT);
  pinMode(MTQ3_DIR, OUTPUT);
  pinMode(MTQ3_PWM, OUTPUT);

  digitalWrite(MTQ1_SLEEP, MTQ_ACTIVE);
  digitalWrite(MTQ2_SLEEP, MTQ_ACTIVE);
  digitalWrite(MTQ3_SLEEP, MTQ_ACTIVE);

  digitalWrite(MTQ1_DIR, HIGH);
  digitalWrite(MTQ2_DIR, HIGH);
  digitalWrite(MTQ3_DIR, HIGH);



  analogWriteFrequency(10000);
  analogWriteResolution(12);
}

void loop() {
  analogWrite(MTQ1_PWM, 1024);
  analogWrite(MTQ2_PWM, 1024);
  analogWrite(MTQ3_PWM, 1024);
  // put your main code here, to run repeatedly:
}
