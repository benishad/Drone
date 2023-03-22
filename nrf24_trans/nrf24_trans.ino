//조종기 1
#include <SPI.h>
#include "RF24.h"
//조이스틱 두개*********************************************************
int joystickX_1 = 4;    //좌
int joystickY_1 = 0;

int joystickX_2 = 2;    //우
int joystickY_2 = 15;

//읽기 *****************************************************************
int read_joystick_x_1 =0; // 조이스틱 x의 저항값을 받아 줄 수 있는 변수 선언
int read_joystick_y_1 =0; // 조이스틱 y의 저항값을 받아 줄 수 있는 변수 선언
int read_joystick_x_2 =0;
int read_joystick_y_2 =0;
//로그******************************************************************
/*
int logJoystickValueX_1 = 0;
int logJoystickValueY_1 = 0;
int logJoystickValueX_2 = 0;
int logJoystickValueY_2 = 0;
*/


// 맵핑******************************************************************
int MapLogJoysticValueX_1 = 0;
int MapLogJoysticValueY_1 = 0;
int MapLogJoysticValueX_2 = 0;
int MapLogJoysticValueY_2 = 0;


// 0번과 1번으로 송수신을 결정
// 수신 아두이노는 0으로, 송신 아두이노는 1로 설정하고 컴파일

RF24 radio(12,14); // 7번핀 CE, 8번핀 CSN으로 SPI통신 설정

// pipe adresses를 설정하기 위한 값
// 하나의 Rx가 6개까지의 Tx와 통신가능
// 5 byte의 문자열로 주소값 설정가능
byte addresses[6] = "12345";

int message[4];

void setup()
{
 Serial.begin(115200); // 통신속도 9600bps로 시리얼 통신 시작
 radio.begin(); // nRF24L01모듈 초기화

 // 전원 공급 관련 문제가 발생하지 않도록 PA레벨을 LOW로 설정, RF24_PA_MAX가 기본값
 // RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
 // NRF24L01: -18dBm, -12dBm,-6dBM, and 0dBm
 radio.setPALevel(RF24_PA_LOW);

 // 송신기 설정
  radio.openWritingPipe(addresses); // 데이터를 보낼 주소 설정
  radio.stopListening(); // Listening을 멈춤
}

void loop()
{
  //int A = read_joystick_x; 나중에 로그 만들때 사용할 기록을 다시 저장하는 변수 예시
  JOYSTICK();
  LOG();
  radio.write(message, sizeof(message)); // 해당 텍스트를 송신
  delay(100); //원래는100줬음
}

void JOYSTICK(){
  read_joystick_x_1 = analogRead(joystickX_1);
  int map_read_joystick_x_1 = map(read_joystick_x_1,0,4095,999,0);///4095를 999로 변경

  read_joystick_y_1 = analogRead(joystickY_1);
  int map_read_joystick_y_1 = map(read_joystick_y_1,0,4095,999,0);

  read_joystick_x_2 = analogRead(joystickX_2);
  int map_read_joystick_x_2 = map(read_joystick_x_2,4095,0,999,0);

  read_joystick_y_2 = analogRead(joystickY_2);
  int map_read_joystick_y_2 = map(read_joystick_y_2,0,4095,999,0);

/*
  message[0] = read_joystick_x_1;
  message[1] = read_joystick_y_1;
  message[2] = read_joystick_x_2;
  message[3] = read_joystick_y_2;
*/
  message[0] = map_read_joystick_x_1;
  message[1] = map_read_joystick_y_1;
  message[2] = map_read_joystick_x_2;
  message[3] = map_read_joystick_y_2;
  
/*

  logJoystickValueX_1 = read_joystick_x_1;
  logJoystickValueY_1 = read_joystick_y_1;
  logJoystickValueX_2 = read_joystick_x_2;
  logJoystickValueY_2 = read_joystick_y_2;
*/

  //맵핑************************************************************
  MapLogJoysticValueX_1 = map_read_joystick_x_1;
  MapLogJoysticValueY_1 = map_read_joystick_y_1;
  MapLogJoysticValueX_2 = map_read_joystick_x_2;
  MapLogJoysticValueY_2 = map_read_joystick_y_2;
}

void LOG(){
  Serial.print("LEFT = ");
  Serial.print("X : ");
  Serial.print(MapLogJoysticValueX_1);
  Serial.print(" , ");
  Serial.print("Y : ");
  Serial.print(MapLogJoysticValueY_1);
  Serial.print(" | ");
  Serial.print("RIGHT = ");
  Serial.print("X : ");
  Serial.print(MapLogJoysticValueX_2);
  Serial.print(" , ");
  Serial.print("Y : ");
  Serial.println(MapLogJoysticValueY_2);
}
