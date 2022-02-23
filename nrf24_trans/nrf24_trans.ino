//조종기 1
#include <SPI.h>
#include "RF24.h"
int buttonOne = 5; // 스위치 D6에 연결하고 변수 선언
int buttonTwo = 17;
int joystickButton = 4; //스위치 D4에 연결하고 변수선언
int joystickX = 2;
int joystickY = 15;

int buttonOneValue = 0; // 스위치 읽은 값 변수 선언
int buttonTwoValue = 0;
int joystickButtonValue =0; // 스위치 읽은 값 변수 선언
int read_joystick_x =0; // 조이스틱 x의 저항값을 받아 줄 수 있는 변수 선언
int read_joystick_y =0; // 조이스틱 y의 저항값을 받아 줄 수 있는 변수 선언

int logJoystickValueX = 0;
int logJoystickValueY = 0;
int logJoystickValueButton = 0;
int logButtonOneValue = 0;
int logButtonTwoValue = 0;

// 0번과 1번으로 송수신을 결정
// 수신 아두이노는 0으로, 송신 아두이노는 1로 설정하고 컴파일

RF24 radio(12,14); // 7번핀 CE, 8번핀 CSN으로 SPI통신 설정

// pipe adresses를 설정하기 위한 값
// 하나의 Rx가 6개까지의 Tx와 통신가능
// 5 byte의 문자열로 주소값 설정가능
byte addresses[6] = "12345";

int message[5];

void setup()
{
 Serial.begin(115200); // 통신속도 9600bps로 시리얼 통신 시작
 radio.begin(); // nRF24L01모듈 초기화
 pinMode(buttonOne, INPUT); // 스위치 내부풀업 입력모드로 설정
 pinMode(buttonTwo, INPUT);
 pinMode(joystickButton, INPUT_PULLUP); // 조이스틱 스위치 내부풀업 모드로 설정

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
  BUTTON();
  LOG();
  
  radio.write(message, sizeof(message)); // 해당 텍스트를 송신
  delay(100);//원래는100줬음
}

void JOYSTICK(){
  read_joystick_x = analogRead(joystickX);
  read_joystick_y = analogRead(joystickY);
  joystickButtonValue = digitalRead(joystickButton);

  message[0] = read_joystick_x;
  message[1] = read_joystick_y;
  message[2] = joystickButtonValue;

  logJoystickValueX = read_joystick_x;
  logJoystickValueY = read_joystick_y;
  logJoystickValueButton = joystickButtonValue;
}

void BUTTON(){
  buttonOneValue = digitalRead(buttonOne);
  buttonTwoValue = digitalRead(buttonTwo);
  
  message[3] = buttonOneValue;
  message[4] = buttonTwoValue;

  logButtonOneValue = buttonOneValue;
  logButtonTwoValue = buttonTwoValue;
}

void LOG(){
  Serial.print(logJoystickValueX);
  Serial.print(" , ");
  Serial.print(logJoystickValueY);
  Serial.print(" , ");
  Serial.print(logJoystickValueButton);
  Serial.print(" , ");
  Serial.print(logButtonOneValue);
  Serial.print(" , ");
  Serial.println(logButtonTwoValue);
}
