//중계 1
#include <HardwareSerial.h>
#include <SPI.h>
#include "RF24.h"

HardwareSerial Unit1(1);

char DATA;
int message[2];

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
    if(message[0] == 2){
      Unit1.write('a');
    }
    else{
      Unit1.write('b');
    }
    Serial.print(message[0]);
    Serial.print(", ");
    Serial.println(message[1]);
    delay(10);
  }
}
