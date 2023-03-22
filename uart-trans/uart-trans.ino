//중계 1
#include <HardwareSerial.h>
#include <SPI.h>
#include "RF24.h"

HardwareSerial Unit1(1);

//읽기------------------------------------------------------------------

int read_joystick_x_1 = 0; // 조이스틱 x의 값을 변수 선언
int read_joystick_y_1 = 0; // 조이스틱 y의 값을 변수 선언
int read_joystick_x_2 = 0;
int read_joystick_y_2 = 0;

//로그------------------------------------------------------------------

int logJoystickValueX_1 = 0;
int logJoystickValueY_1 = 0;
int logJoystickValueX_2 = 0;
int logJoystickValueY_2 = 0;

char buf[30];

int message[4];

// 0번과 1번으로 송수신을 결정
// 수신 아두이노는 0으로, 송신 아두이노는 1로 설정하고 컴파일

RF24 radio(12,14); // 7번핀 CE, 8번핀 CSN으로 SPI통신 설정

// pipe adresses를 설정하기 위한 값
// 하나의 Rx가 6개까지의 Tx와 통신가능
// 5 byte의 문자열로 주소값 설정가능
byte addresses[6] = "12345";

void setup() {
  Serial.begin(115200);
  Unit1.begin(115200, SERIAL_8N1, 17,16);
  radio.begin(); // nRF24L01모듈 초기화

  // 전원 공급 관련 문제가 발생하지 않도록 PA레벨을 LOW로 설정, RF24_PA_MAX가 기본값
  // RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
  // NRF24L01: -18dBm, -12dBm,-6dBM, and 0dBm
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0,addresses); // 데이터를 받을 주소 설정
  radio.startListening(); // 읽는 pipe 주소의 data Listening 시작
}
void loop() {
  if(radio.available())
  {
    radio.read(message, sizeof(message));
    
    READJOYSTICK();
    WRITEPRINT();
    LOG();
    
    delay(10);
  }
}

void READJOYSTICK(){
  read_joystick_x_1 = message[0];
  read_joystick_y_1 = message[1];
  read_joystick_x_2 = message[2];
  read_joystick_y_2 = message[3];

  logJoystickValueX_1 = read_joystick_x_1;
  logJoystickValueY_1 = read_joystick_y_1;
  logJoystickValueX_2 = read_joystick_x_2;
  logJoystickValueY_2 = read_joystick_y_2;
}

void WRITEPRINT(){
  Unit1.print("A");
  sprintf(buf, "%04d", read_joystick_x_1);
  Unit1.print(buf);
  Unit1.print("B");
  sprintf(buf, "%04d", read_joystick_y_1);
  Unit1.print(buf);
  Unit1.print("C");
  sprintf(buf, "%04d", read_joystick_x_2);
  Unit1.print(buf);
  Unit1.print("D");
  sprintf(buf, "%04d", read_joystick_y_2);
  Unit1.print(buf);
  Unit1.println("E");
}

void LOG(){
  sprintf(buf, "%04d", logJoystickValueX_1);
  Serial.print(buf);
  Serial.print("A");
  sprintf(buf, "%04d", logJoystickValueY_1);
  Serial.print(buf);
  Serial.print("A");
  sprintf(buf, "%04d", logJoystickValueX_2);
  Serial.print(buf);
  Serial.print("A");
  sprintf(buf, "%04d", logJoystickValueY_2);
  Serial.print(buf);
  Serial.println("A");
}