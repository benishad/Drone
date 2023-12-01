// 중계1번 esp32 드론
//라디오 통신으로 받은 값을 처리하여 중계2번 esp32로 uart전송한다.
#include <SPI.h>
#include <RF24.h>

HardwareSerial esp32_Serial(0);

#define RXp1 3    //esp32 uart통신을 위한 핀
#define TXp1 1    //esp32 uart통신을 위한 핀
 
const uint64_t pipeIn = 0xABCD1234567890EFLL;  //주소값 설정

char buf[10]; //저장은 3자리 정수만을 저장하기에 여유롭게 10으로 설정

RF24 radio(4, 5); // GPIO18 for CE, GPIO5 for CSN

struct MyData 
{
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
  byte AUX1;
  byte AUX2;
  byte AUX3;
  byte AUX4;
  byte AUX5;
  byte AUX6;
};

MyData receivedData;
MyData lastData;  // 마지막으로 받은 데이터를 저장할 구조체

bool communicationActive = true;  // 통신 활성화 상태
unsigned long lastCommunicationTime = 0;
unsigned long communicationTimeout = 5000;  // 통신 타임아웃 시간 (5초)

// 안전한 착지를 위한 Roll, Pitch, Yaw 값을 대신할 safe값
const byte safeData = 128;
const byte safeAUX = 000;
const byte safeAUX2 = 001;

void setup() 
{
  esp32_Serial.begin(115200, SERIAL_8N1, RXp1, TXp1);    //메인드론 esp32로 보내기 위한 시리얼 
  radio.begin();
  //radio.setDataRate(RF24_250KBPS);
  // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  //radio.setPALevel(RF24_PA_HIGH);
  //거리가 가까운 순으로 RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX 등으로 설정
  radio.openReadingPipe(1, pipeIn); // Set the same address used in the transmitter
  radio.startListening(); //수신모드
  esp32_Serial.print("중계1 - start!!");
}

void loop() 
{
  if(radio.available()>0) 
  {
    radio.read(&receivedData, sizeof(MyData));
    lastCommunicationTime = millis();  // 통신이 성공한 경우 타임아웃 타이머 재설정

    //esp32로 보내는 uart통신 부분
    esp32_Serial.print("A");
    sprintf(buf, "%03d", receivedData.throttle);
    esp32_Serial.print(buf);
    esp32_Serial.print("B");
    sprintf(buf, "%03d", receivedData.yaw);
    esp32_Serial.print(buf);
    esp32_Serial.print("C");
    sprintf(buf, "%03d", receivedData.pitch);
    esp32_Serial.print(buf);
    esp32_Serial.print("D");
    sprintf(buf, "%03d", receivedData.roll);
    esp32_Serial.print(buf);
    esp32_Serial.print("E");
    sprintf(buf, "%03d", receivedData.AUX1);
    esp32_Serial.print(buf);
    esp32_Serial.print("F");
    sprintf(buf, "%03d", receivedData.AUX2);
    esp32_Serial.print(buf);
    esp32_Serial.print("G");
    sprintf(buf, "%03d", receivedData.AUX3);
    esp32_Serial.print(buf);
    esp32_Serial.print("H");
    sprintf(buf, "%03d", receivedData.AUX4);
    esp32_Serial.print(buf);
    esp32_Serial.print("I");
    sprintf(buf, "%03d", receivedData.AUX5);
    esp32_Serial.print(buf);
    esp32_Serial.print("J");
    sprintf(buf, "%03d", receivedData.AUX6);
    esp32_Serial.print(buf);
    esp32_Serial.println("K");
    

    // 통신이 활성화된 상태로 설정
    communicationActive = true;
    delay(10);
  }
  else 
  {
    // 통신이 끊겼을 때의 동작
    if (communicationActive && (millis() - lastCommunicationTime >= communicationTimeout)) 
    {
      communicationActive = false;  // 통신 끊김 상태로 설정
      // 스로틀 값을 천천히 감소시켜 안전한 착지 구현
      for (int throttleValue = receivedData.throttle; throttleValue >= 0; throttleValue -= 2) 
      {
        if (radio.available()>0) {//라디오 통신이 복구됬는지 판단
          break;
        }
        esp32_Serial.print("A");
        sprintf(buf, "%03d", throttleValue);
        esp32_Serial.print(buf);
        esp32_Serial.print("B");
        esp32_Serial.print(safeData);  // 안전한 Yaw 값
        esp32_Serial.print("C");
        esp32_Serial.print(safeData);  // 안전한 Pitch 값
        esp32_Serial.print("D");
        esp32_Serial.print(safeData);  // 안전한 Roll 값
        esp32_Serial.print("E");
        sprintf(buf, "%03d", safeAUX);  //AUX1
        esp32_Serial.print(buf);
        esp32_Serial.print("F");
        sprintf(buf, "%03d", safeAUX2); //AUX2
        esp32_Serial.print(buf);
        esp32_Serial.print("G");
        sprintf(buf, "%03d", safeAUX);  //AUX3
        esp32_Serial.print(buf);
        esp32_Serial.print("H");
        sprintf(buf, "%03d", safeAUX);  //AUX4
        esp32_Serial.print(buf);
        esp32_Serial.print("I");
        sprintf(buf, "%03d", safeAUX);  //AUX5
        esp32_Serial.print(buf);
        esp32_Serial.print("J");
        sprintf(buf, "%03d", safeAUX);  //AUX6
        esp32_Serial.print(buf);
        esp32_Serial.println("K");
        delay(750);  // 100ms 간격으로 스로틀 값을 감소시킴
      }
    }
  }
}
