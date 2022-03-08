//드론 1
#include <Servo.h>
#include <SPI.h>
#include "RF24.h"

Servo BLDC1;   // 직교좌표 사분면 느낌으로 배치
Servo BLDC2;
Servo BLDC3;
//Servo BLDC4;

int LED_BLUE = 5;
int LED_GREEN = 4;
int LED_WHITE = 17;

int bldcOne = 13;
int bldcTwo = 26;
int bldcThree = 25;
//int bldcFour = 23;

int message[6];

int read_joystick_x =0; // 조이스틱 x의 값을 변수 선언
int read_joystick_y =0; // 조이스틱 y의 값을 변수 선언
int joystickButtonValue =0; // 조이스틱 읽은 값 변수 선언

int buttonOneValue = 0; // 스위치1 읽은 값 변수 선언
int buttonTwoValue = 0; // 스위치2 읽은 값 변수 선언

int bldcOneValue = 0; // 1번째 bldc모터 변수선언
int bldcTwoValue = 0; // 2번째 bldc모터 변수선언
int bldcThreeValue = 0; // 3번째 bldc모터 변수선언
//int bldcFourValue = 0; // 4번째 bldc모터 변수선언

int logJoystickValueX = 0; // 조이스틱 x값의 로그를 저장할 변수선언
int logJoystickValueY = 0; // 조이스틱 y값의 로그를 저장할 변수선언
int logJoystickValueButton = 0; // 조이스틱 버튼의 값을 저장할 변수선언

int logButtonOneValue = 0; // 스위치1의 로그를 저장할 변수선언
int logButtonTwoValue = 0; // 스위치2의 로그를 저장할 변수선언

int logbldcOneValue = 0; // bldc모터1의 로그를 저장할 변수선언
int logbldcTwoValue = 0; // bldc모터2의 로그를 저장할 변수선언
int logbldcThreeValue = 0; // bldc모터3의 로그를 저장할 변수선언
//int logbldcFourValue = 0; // bldc모터4의 로그를 저장할 변수선언

RF24 radio(12,14); // 7번핀 CE, 8번핀 CSN으로 SPI통신 설정
byte addresses[6] = "12345";

void setup() 
{
  Serial.begin(115200); // 시리얼속도 115200으로시작
  radio.begin(); //rf통신 시작
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0,addresses); // 데이터를 받을 주소 설정
  radio.startListening(); // 읽는 pipe 주소의 data Listening 시작
  
  BLDC1.attach(bldcOne); 
  BLDC2.attach(bldcTwo);
  BLDC3.attach(bldcThree);
  //BLDC4.attach(bldcFour);
  BLDC1.writeMicroseconds(1200); //시작 값
  BLDC2.writeMicroseconds(1200); //시작 값
  BLDC3.writeMicroseconds(1200); //시작 값
  //BLDC4.writeMicroseconds(1200); //시작 값
  
  pinMode(LED_BLUE, OUTPUT); // LED 출력으로 선언
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
  bldcTwoValue = message[5];
  bldcThreeValue = message[5];
  //bldcFourValue = message[5];
  
  logbldcOneValue = bldcOneValue;
  logbldcTwoValue = bldcTwoValue;
  logbldcThreeValue = bldcThreeValue;
  //logbldcFourValue = bldcFourValue;
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
  BLDC2.write(bldcTwoValue);
  BLDC3.write(bldcThreeValue);
  //BLDC4.write(bldcFourValue);
  
  BLDC1.writeMicroseconds(bldcOneValue);
  BLDC2.writeMicroseconds(bldcTwoValue);
  BLDC3.writeMicroseconds(bldcThreeValue);
  //BLDC4.writeMicroseconds(bldcFourValue);
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
}
