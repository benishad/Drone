//메인 드론
//중계통신으로 받은 값으로 작동하는 메인 드론 ESP32.
//받은 값을 처리하여 다시 나노로 전송하는 역할을 한다.
#include <SPI.h>
#include <RF24.h>

#define RXp2 16   //uart통신을 위한 핀
#define TXp2 17   //uart통신을 위한 핀

//long long pipeIn = 0x1234ABCDEFLL;   //주소값 설정
const uint64_t pipeIn = 0x5A1B2C3D4E5F6A7BLL;

char buf[30]; //  

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

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2);    //Atmega와 uart통신을 위한 시리얼
  radio.begin();
  radio.openReadingPipe(1, pipeIn); // Set the same address used in the transmitter
  radio.setDataRate(RF24_2MBPS);  // 데이터 전송 속도 설정
  // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  //radio.setPALevel(RF24_PA_HIGH);
  //거리가 가까운 순으로 RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX 등으로 설정
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    radio.read(&receivedData, sizeof(MyData));

    Serial.print("메인기 =>  Throttle: ");
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
    Serial.print(receivedData.AUX2);
    Serial.print("  AUX3: ");
    Serial.print(receivedData.AUX3);
    Serial.print("  AUX4: ");
    Serial.println(receivedData.AUX4);

    Serial2.print("A");
    sprintf(buf, "%03d", receivedData.throttle);
    Serial2.print(buf);
    Serial2.print("B");
    sprintf(buf, "%03d", receivedData.yaw);
    Serial2.print(buf);
    Serial2.print("C");
    sprintf(buf, "%03d", receivedData.pitch);
    Serial2.print(buf);
    Serial2.print("D");
    sprintf(buf, "%03d", receivedData.roll);
    Serial2.print(buf);
    Serial2.print("E");
    sprintf(buf, "%03d", receivedData.AUX1);
    Serial2.print(buf);
    Serial2.print("F");
    sprintf(buf, "%03d", receivedData.AUX2);
    Serial2.print(buf);
    Serial2.println("G");

    delay(10);
  }
}
