// 중계1번 esp32 드론
//라디오 통신으로 받은 값을 처리하여 중계2번 esp32로 uart전송한다.
#include <SPI.h>
#include <RF24.h>

#define RXp1 3    //esp32 uart통신을 위한 핀
#define TXp1 1    //esp32 uart통신을 위한 핀
#define RXp2 16   // 아두이노 나노 uart 통신을 위한 핀
#define TXp2 17   // 아두이노 나노 uart 통신을 위한 핀

//long long pipeIn = 0x1324ABCDEFLL;   //주소값 설정
const uint64_t pipeIn = 0xABCD1234567890EFLL;

char buf[24]; //
char nanobuf[18];

RF24 radio(4, 5); // GPIO18 for CE, GPIO5 for CSN

struct MyData {
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
  byte AUX1;
  byte AUX2;
  byte AUX3;
  byte AUX4;
};

MyData receivedData;

struct NanoData {
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
  byte AUX1;
  byte AUX2;

};

NanoData transData;

void setup() {
  Serial1.begin(115200, SERIAL_8N1, RXp1, TXp1);    //메인드론 esp32로 보내기 위한 시리얼 
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);    // 아두이노 나노와 uart 통신을 위한 시리얼
  radio.begin();
  //radio.setDataRate(RF24_250KBPS);
  // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  //radio.setPALevel(RF24_PA_HIGH);
  //거리가 가까운 순으로 RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX 등으로 설정
  radio.openReadingPipe(1, pipeIn); // Set the same address used in the transmitter
  radio.startListening();
}

void loop() { 
  if(radio.available()) 
  {
    radio.read(&receivedData, sizeof(MyData));

    /*
    Serial.print("중계기 1 => Throttle: ");
    Serial.print(receivedData.throttle);
    Serial.print("  Yaw: ");
    Serial.print(receivedData.yaw);
    Serial.print("  Pitch: ");
    Serial.print(receivedData.pitch);
    Serial.print("  Roll: ");
    Serial.print(receivedData.roll);
    Serial.print("  AUX1: ");
    Serial.print(receivedData.AUX1);
    Serial.print("  AUX2: ");
    Serial.println(receivedData.AUX2);
    */

    
    //esp32로 보내는 uart통신 부분
    Serial1.print("A");
    sprintf(buf, "%03d", receivedData.throttle);
    Serial1.print(buf);
    Serial1.print("B");
    sprintf(buf, "%03d", receivedData.yaw);
    Serial1.print(buf);
    Serial1.print("C");
    sprintf(buf, "%03d", receivedData.pitch);
    Serial1.print(buf);
    Serial1.print("D");
    sprintf(buf, "%03d", receivedData.roll);
    Serial1.print(buf);
    Serial1.print("E");
    sprintf(buf, "%03d", receivedData.AUX1);
    Serial1.print(buf);
    Serial1.print("F");
    sprintf(buf, "%03d", receivedData.AUX2);
    Serial1.print(buf);
    Serial1.print("G");
    sprintf(buf, "%03d", receivedData.AUX3);
    Serial1.print(buf);
    Serial1.print("H");
    sprintf(buf, "%03d", receivedData.AUX4);
    Serial1.print(buf);
    Serial1.println("I");

     // 아두이노 나노로 보내는 uart 통신 부분
    Serial2.print("G");
    sprintf(nanobuf, "%03d", transData.throttle);
    Serial2.print(nanobuf);
    Serial2.print("F");
    sprintf(nanobuf, "%03d", transData.yaw);
    Serial2.print(nanobuf);
    Serial2.print("E");
    sprintf(nanobuf, "%03d", transData.pitch);
    Serial2.print(nanobuf);
    Serial2.print("D");
    sprintf(nanobuf, "%03d", transData.roll);
    Serial2.print(nanobuf);
    Serial2.print("C");
    sprintf(nanobuf, "%03d", transData.AUX1);
    Serial2.print(nanobuf);
    Serial2.print("B");
    sprintf(nanobuf, "%03d", transData.AUX2);
    Serial2.print(nanobuf);
    Serial2.println("A");
    
    delay(10);
  }
}
