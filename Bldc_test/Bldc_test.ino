#include <Servo.h>

Servo BLDC;

void setup() {
  Serial.begin(115200);
  BLDC.attach(13);
  BLDC.writeMicroseconds(1200);//시작 값
}

void loop() {
  int val;
  val = analogRead(34);
  val = map(val, 0, 1023, 1000, 1500);
  BLDC.write(val);
  BLDC.writeMicroseconds(val);
  Serial.println(val);
  // put your main code here, to run repeatedly:

}
