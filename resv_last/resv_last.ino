//드론 1
#include <Servo.h>
#include <SPI.h>
#include "RF24.h"

Servo servo;
int servo_pin = 9;
int message[2];
RF24 radio(7,8); // 7번핀 CE, 8번핀 CSN으로 SPI통신 설정
byte addresses[6] = "abcde";

void setup() 
{
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0,addresses); // 데이터를 받을 주소 설정
  radio.startListening(); // 읽는 pipe 주소의 data Listening 시작
  servo.attach(servo_pin);
}
void loop() 
{
  if(radio.available())
  {
    radio.read(message, sizeof(message));
    
    if(message[0] == 2)
    {
      servo.write(0); // 서보모터 최고속도
    }
    else
    {
      servo.write(90); //서보모터 멈춤
    }    
    Serial.print(message[0]);
    Serial.print(", ");
    Serial.println(message[1]);
  }
}
