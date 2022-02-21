//조종기 1
#include <SPI.h>
#include "RF24.h"
int sw = 5; // 스위치 D6에 연결하고 변수 선언
int read_sw = 0; // 스위치 읽은 값 변수 선언

// 0번과 1번으로 송수신을 결정
// 수신 아두이노는 0으로, 송신 아두이노는 1로 설정하고 컴파일

RF24 radio(12,14); // 7번핀 CE, 8번핀 CSN으로 SPI통신 설정

// pipe adresses를 설정하기 위한 값
// 하나의 Rx가 6개까지의 Tx와 통신가능
// 5 byte의 문자열로 주소값 설정가능
byte addresses[6] = "12345";

int message[2];

int x = 0;
int y = 0;

void setup()
{
 Serial.begin(115200); // 통신속도 9600bps로 시리얼 통신 시작
 radio.begin(); // nRF24L01모듈 초기화
 pinMode(sw, INPUT); // 스위치 내부풀업 입력모드로 설정

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
  read_sw=digitalRead(sw);
  if(read_sw==1){
    Serial.println("****************ON****************");
    x = 1;
    y = 1;
  }
  else{
    Serial.println("OFF");
    x = 2;
    y = 2;
  }

  message[0] = x;
  message[1] = y;
  radio.write(message, sizeof(message)); // 해당 텍스트를 송신
  delay(100);//원래는100줬음
}
