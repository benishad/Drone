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

bool communicationActive = true;  // 통신 활성화 상태
unsigned long lastCommunicationTime = 0;
unsigned long communicationTimeout = 5000;  // 통신 타임아웃 시간 (5초)

// 안전한 착지를 위한 Roll, Pitch, Yaw 값
const byte safeRoll = 128;
const byte safePitch = 128;
const byte safeYaw = 128;
const byte safeAUX1 = 000;
const byte safeAUX2 = 001;
const byte safeAUX3 = 000;
const byte safeAUX4 = 000;

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

MyData lastData;  // 마지막으로 받은 데이터를 저장할 변수

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
    lastCommunicationTime = millis();  // 통신이 성공한 경우 타임아웃 타이머 재설정

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

    if(receivedData.AUX4 == 1) // AUX4 값이 1이면 마지막 데이터 저장
    {
      lastData = receivedData;
      sendToNano(lastData);  // 나노로 마지막 데이터 전송
    }
    else if(receivedData.AUX4 == 0) // AUX4 값이 0이면 현재 데이터를 나노로 전송
    {
      sendToNano(receivedData);
    }

    communicationActive = true;
    delay(10);
  }
  //안전모드 동작부분
  else 
  {
    // 통신이 끊겼을 때의 동작
    if (communicationActive && (millis() - lastCommunicationTime >= communicationTimeout)) 
    {
      communicationActive = false;  // 통신 끊김 상태로 설정
      // 스로틀 값을 천천히 감소시켜 안전한 착지 모사
      for (int throttleValue = receivedData.throttle; throttleValue >= 0; throttleValue -= 2) 
      {
        Serial2.print("A");
        sprintf(buf, "%03d", throttleValue);
        Serial2.print(buf);
        Serial2.print("B");
        Serial2.print(safeYaw);  // 안전한 Yaw 값
        Serial2.print("C");
        Serial2.print(safePitch);  // 안전한 Pitch 값
        Serial2.print("D");
        Serial2.print(safeRoll);  // 안전한 Roll 값
        Serial2.print("E");
        sprintf(buf, "%03d", safeAUX1);
        Serial2.print(buf);
        Serial2.print("F");
        sprintf(buf, "%03d", safeAUX2);
        Serial2.print(buf);
        Serial2.println("G");
        delay(750);  // 100ms 간격으로 스로틀 값을 감소시킴
      }
    }
  }
}

void sendToNano(MyData data) {
    Serial2.print("A");
    sprintf(buf, "%03d", data.throttle);
    Serial2.print(buf);
    Serial2.print("B");
    sprintf(buf, "%03d", data.yaw);
    Serial2.print(buf);
    Serial2.print("C");
    sprintf(buf, "%03d", data.pitch);
    Serial2.print(buf);
    Serial2.print("D");
    sprintf(buf, "%03d", data.roll);
    Serial2.print(buf);
    Serial2.print("E");
    sprintf(buf, "%03d", data.AUX1);
    Serial2.print(buf);
    Serial2.print("F");
    sprintf(buf, "%03d", data.AUX2);
    Serial2.print(buf);
    Serial2.println("G");
}
