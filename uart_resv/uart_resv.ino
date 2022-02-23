//중계 2               
#include <HardwareSerial.h>
#include <SPI.h>
#include "RF24.h"

HardwareSerial Unit2(1);

RF24 radio(12,14); // 7번핀 CE, 8번핀 CSN으로 SPI통신 설정
byte addresses[6] = "abcde";

int read_joystick_x =0; // 조이스틱 x의 값을 변수 선언
int read_joystick_y =0; // 조이스틱 y의 값을 변수 선언
int joystickButtonValue =0; // 조이스틱 읽은 값 변수 선언
int buttonOneValue = 0; // 스위치 읽은 값 변수 선언
int buttonTwoValue = 0;

int logJoystickValueX = 0;
int logJoystickValueY = 0;
int logJoystickValueButton = 0;
int logButtonOneValue = 0;
int logButtonTwoValue = 0;

int message[5];

void setup() {
  Serial.begin(115200);
  Unit2.begin(115200, SERIAL_8N1, 17,16);
  radio.begin(); // nRF24L01모듈 초기화
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses); // 데이터를 받을 주소 설정
  radio.stopListening(); // 읽는 pipe 주소의 data Listening 시작
}
void loop() {
 
  if(Unit2.available())
  {
    String inString = Unit2.readStringUntil('\n');
    read_joystick_x = inString.substring(1, 5).toInt();
    read_joystick_y = inString.substring(6, 10).toInt();
    joystickButtonValue = inString.substring(11, 12).toInt();
    buttonOneValue = inString.substring(13, 14).toInt();
    buttonTwoValue = inString.substring(15, 16).toInt();
    
    LOG();
    
    message[0] = read_joystick_x;
    message[1] = read_joystick_y;
    message[2] = joystickButtonValue;
    message[3] = buttonOneValue;
    message[4] = buttonTwoValue;
    radio.write(message, sizeof(message)); // 해당 텍스트를 송신
    delay(20);
  }
}

void LOG(){
  logJoystickValueX = read_joystick_x;
  logJoystickValueY = read_joystick_y;
  logJoystickValueButton = joystickButtonValue;
  logButtonOneValue = buttonOneValue;
  logButtonTwoValue = buttonTwoValue;
  
  Serial.print(logJoystickValueX);
  Serial.print(logJoystickValueY);
  Serial.print(logJoystickValueButton);
  Serial.print(logButtonOneValue);
  Serial.println(logButtonTwoValue);
}
