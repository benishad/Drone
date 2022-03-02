//드론 1
#include "BluetoothSerial.h"
#include <Servo.h>
#include <SPI.h>
#include "RF24.h"

BluetoothSerial SerialBT;

Servo BLDC1;

int LED_BLUE = 5;
int LED_GREEN = 4;
int LED_WHITE = 17;
int bldcOne = 13;

int message[6];

int read_joystick_x =0; // 조이스틱 x의 값을 변수 선언
int read_joystick_y =0; // 조이스틱 y의 값을 변수 선언
int joystickButtonValue =0; // 조이스틱 읽은 값 변수 선언
int buttonOneValue = 0; // 스위치 읽은 값 변수 선언
int buttonTwoValue = 0;
int bldcOneValue = 0;

int logJoystickValueX = 0;
int logJoystickValueY = 0;
int logJoystickValueButton = 0;
int logButtonOneValue = 0;
int logButtonTwoValue = 0;
int logbldcOneValue = 0;

RF24 radio(12,14); // 7번핀 CE, 8번핀 CSN으로 SPI통신 설정
byte addresses[6] = "abcde";

void setup() 
{
  Serial.begin(115200);
  SerialBT.begin("ESP_DRONE_32"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0,addresses); // 데이터를 받을 주소 설정
  radio.startListening(); // 읽는 pipe 주소의 data Listening 시작
  BLDC1.attach(bldcOne);
  BLDC1.writeMicroseconds(1200); //시작 값
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_WHITE, OUTPUT);
}
void loop() 
{
  if(radio.available())
  {
    radio.read(message, sizeof(message));

    READJOYSTICK();
    READBUTTON();
    READPOTEN();
    LEDBLUE();
    LEDGREEN();
    LEDWHITE();
    BLDCCONTROL();
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
  buttonOneValue = message[3];
  buttonTwoValue = message[4];

  logButtonOneValue = buttonOneValue;
  logButtonTwoValue = buttonTwoValue;
}

void READPOTEN(){
  bldcOneValue = message[5];

  logbldcOneValue = bldcOneValue;
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
  if(buttonOneValue == 1){
      digitalWrite(LED_GREEN , HIGH);
    }
    else
      digitalWrite(LED_GREEN , LOW);
}

void LEDWHITE(){
  if(buttonTwoValue == 1){
      digitalWrite(LED_WHITE , HIGH);
    }
    else
      digitalWrite(LED_WHITE , LOW);
}

void BLDCCONTROL(){
  BLDC1.write(bldcOneValue);
  BLDC1.writeMicroseconds(bldcOneValue);
}

void LOG(){
  Serial.print(logbldcOneValue);
  Serial.print(",");
  Serial.print(logJoystickValueX);
  Serial.print(",");
  Serial.print(logJoystickValueY);
  Serial.print(",");
  Serial.print(logJoystickValueButton);
  Serial.print(",");
  Serial.print(logButtonOneValue);
  Serial.print(",");
  Serial.println(logButtonTwoValue);

  SerialBT.print(logbldcOneValue);
  SerialBT.print(",");
  SerialBT.print(logJoystickValueX);
  SerialBT.print(",");
  SerialBT.print(logJoystickValueY);
  SerialBT.print(",");
  SerialBT.print(logJoystickValueButton);
  SerialBT.print(",");
  SerialBT.print(logButtonOneValue);
  SerialBT.print(",");
  SerialBT.println(logButtonTwoValue);
  
}
