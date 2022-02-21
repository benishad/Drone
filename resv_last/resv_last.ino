//드론 1
#include <Servo.h>
#include <SPI.h>
#include "RF24.h"

Servo servo;
int LED_BLUE = 5;
int LED_GREEN = 4;
int servo_pin = 13;
int message[4];

int read_joystick_x =0; // 조이스틱 x의 값을 변수 선언
int read_joystick_y =0; // 조이스틱 y의 값을 변수 선언
int joystickButtonValue =0; // 조이스틱 읽은 값 변수 선언
int buttonValue = 0; // 스위치 읽은 값 변수 선언

int logJoystickValueX = 0;
int logJoystickValueY = 0;
int logJoystickValueButton = 0;
int logButtonValue = 0;

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

    READJOYSTICK();
    READBUTTON();
    LEDBLUE();
    LEDGREEN();
    LOG();
  }
}

void READJOYSTICK(){
  read_joystick_x = message[0];
  read_joystick_y = message[1];
  joystickButtonValue = message[2];

  logJoystickValueX = read_joystick_x;
  logJoystickValueY = read_joystick_y;
  logJoystickValueButton = joystickButtonValue;  
}

void READBUTTON(){
  buttonValue = message[3];

  logButtonValue = buttonValue;
}

void LEDBLUE(){
  if(joystickButtonValue == 0)
    {
      digitalWrite(LED_BLUE , HIGH);
    }
    else
      digitalWrite(LED_BLUE , LOW);
}

void LEDGREEN(){
  if(buttonValue == 1){
      digitalWrite(LED_GREEN , HIGH);
    }
    else
      digitalWrite(LED_GREEN , LOW);
}

void LOG(){
  Serial.print(logJoystickValueX);
  Serial.print(",");
  Serial.print(logJoystickValueY);
  Serial.print(",");
  Serial.print(logJoystickValueButton);
  Serial.print(",");
  Serial.println(logButtonValue);
}
