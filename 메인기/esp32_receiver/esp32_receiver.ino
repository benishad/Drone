//////////////////////////////////////////////////////////////////////////////////
//메인 드론
//중계통신으로 받은 값으로 작동하는 메인 드론 ESP32.
//받은 값을 처리하여 다시 나노로 전송하는 역할을 한다.
#include <SPI.h>
#include <RF24.h>

#define RXp2 16   //uart통신을 위한 핀
#define TXp2 17   //uart통신을 위한 핀

//long long pipeIn = 0x1234ABCDEFLL;   //주소값 설정
const uint64_t pipeIn = 0x5A1B2C3D4E5F6A7BLL;

char buf[10]; //  

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
const byte safeAUX5 = 000;
const byte safeAUX6 = 001;

struct MyData {
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
MyData lastData;  // 마지막으로 받은 데이터를 저장할 변수

const int charge = 14;
const int fire = 12;

void setup() 
{
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);    //Atmega와 uart통신을 위한 시리얼
  radio.begin();
  radio.openReadingPipe(1, pipeIn); // Set the same address used in the transmitter
  radio.setDataRate(RF24_2MBPS);  // 데이터 전송 속도 설정
  // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  //radio.setPALevel(RF24_PA_HIGH);
  //거리가 가까운 순으로 RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX 등으로 설정
  radio.startListening(); //수신모드

  pinMode(charge, OUTPUT);
  pinMode(fire, OUTPUT);
}

void loop() 
{
  if (radio.available()) 
  {
    radio.read(&receivedData, sizeof(MyData));
    lastCommunicationTime = millis();  // 통신이 성공한 경우 타임아웃 타이머 재설정

    //============================================================
    //=====================메인 드론 개별 정지========================

    if(receivedData.AUX4 == 1) 
    {
      lastData.yaw = 127; // 중간값으로 설정
      lastData.pitch = 127;
      lastData.roll = 127;
      lastData.AUX1 = 0;
      lastData.AUX2 = 1;
      lastData.AUX5 = 0;
      lastData.AUX6 = 1;
      sendToNano(lastData);
      serial_print(lastData);
    } 
    else 
    {
      lastData = receivedData;  // 통신 데이터를 lastData에 저장
      sendToNano(receivedData);
      serial_print(receivedData);
    }
    
    //==============================================================
    //==============================================================

    //============================================================
    //===================== 코일건 발사 코드 ========================
    if(receivedData.AUX5 == 1)
    {
      Serial.println("- 충전 -");
      digitalWrite(charge, HIGH);
    }
    else if(receivedData.AUX5 == 0)
    {
      Serial.println("- 충전 꺼짐 -");
      digitalWrite(charge, LOW);
    }
    
    if(receivedData.AUX6 == 0)
    {
      digitalWrite(fire, HIGH);
      Serial.println("발사");
    }
    else if(receivedData.AUX6 == 1)
    {
      digitalWrite(fire, LOW);
      Serial.println("발사꺼짐");
    }
    //==============================================================
    //==============================================================
    
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
        delay(1000);  // 100ms 간격으로 스로틀 값을 감소시킴
      }
    }
  }
}

void sendToNano(MyData data) 
{
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
void serial_print(MyData data)
{
  Serial.print("메인기 =>  Throttle: ");
  Serial.print(data.throttle);
  Serial.print("  Yaw: ");
  Serial.print(data.yaw);
  Serial.print("  Pitch: ");
  Serial.print(data.pitch);
  Serial.print("  Roll: ");
  Serial.print(data.roll);
  Serial.print("  AUX1: ");
  Serial.print(data.AUX1);
  Serial.print("  AUX2: ");
  Serial.print(data.AUX2);
  Serial.print("  AUX3: ");
  Serial.print(data.AUX3);
  Serial.print("  AUX4: ");
  Serial.print(data.AUX4);
  Serial.print("  AUX5: ");
  Serial.print(data.AUX5);
  Serial.print("  AUX6: ");
  Serial.println(data.AUX6);
}
