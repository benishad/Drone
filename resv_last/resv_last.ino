//드론 1
#include <Servo.h>
#include <SPI.h>
#include "RF24.h"

Servo servo;
int LED_BLUE = 5;
int LED_GREEN = 4;
int servo_pin = 13;
int message[4];
RF24 radio(12,14); // 7번핀 CE, 8번핀 CSN으로 SPI통신 설정
byte addresses[6] = "abcde";

void setup() 
{
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0,addresses); // 데이터를 받을 주소 설정
  radio.startListening(); // 읽는 pipe 주소의 data Listening 시작
  servo.attach(servo_pin);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}
void loop() 
{
  if(radio.available())
  {
    radio.read(message, sizeof(message));
    
    if(message[0] == 1)
    {
      digitalWrite(LED_BLUE , HIGH);
      digitalWrite(LED_GREEN , HIGH);
    }
    else if(message[1] == 1)
    {
      digitalWrite(LED_BLUE , LOW);
      digitalWrite(LED_GREEN , LOW);
    }
    else if(message[2] == 1){
      digitalWrite(LED_BLUE , LOW);
      digitalWrite(LED_GREEN , HIGH);
      //servo.write(0);
    }
    else if(message[3] == 1){
      digitalWrite(LED_BLUE , HIGH);
      digitalWrite(LED_GREEN , LOW);
       //servo.write(90);
    }
    Serial.print(message[0]);
    Serial.print(",");
    Serial.print(message[1]);
    Serial.print(",");
    Serial.print(message[2]);
    Serial.print(",");
    Serial.println(message[2]);
  }
}
