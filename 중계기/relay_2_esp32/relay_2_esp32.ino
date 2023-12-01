//중계2번 esp32 드론
//중계1번에서 받은 값을 나노로 보내며 다시 라디오 통신으로 메인 ESP32로 보내준다.
#include <SPI.h>
#include <RF24.h>

HardwareSerial nano_Serial(2);

#define RXp1 3    //esp32 uart통신을 위한 핀
#define TXp1 1    //esp32 uart통신을 위한 핀
#define RXp2 16   // 아두이노 나노 uart 통신을 위한 핀
#define TXp2 17   // 아두이노 나노 uart 통신을 위한 핀

const uint64_t pipeOut = 0x5A1B2C3D4E5F6A7BLL;  //주소값 설정

char buf[10];

RF24 radio(4, 5); // GPIO18 for CE, GPIO5 for CSN , 4번 ce 5번 csn

struct MyData //전송할 데이터 구조체 생성
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

MyData data;
MyData lastData;  // 마지막으로 받은 데이터를 저장할 구조체

struct TestValue  // 스트링 타입 데이터를 파싱하기 위한..
{
  String test_throttle;
  String test_yaw;
  String test_pitch;
  String test_roll;
  String test_AUX1;
  String test_AUX2;
  String test_AUX3;
  String test_AUX4;
  String test_AUX5;
  String test_AUX6;
};

TestValue test;

void resetData() 
{
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  data.AUX1 = 0;
  data.AUX2 = 1;
  data.AUX3 = 0;
  data.AUX4 = 0;
  data.AUX5 = 0;
  data.AUX6 = 0;
}

//-----------------------------------------------------------------------
//--------------------------SETUP----------------------------------------
//-----------------------------------------------------------------------
void setup()
{
  Serial1.begin(9600, SERIAL_8N1, RXp2, TXp2); //나노와 통신하는 시리얼
  Serial2.begin(115200, SERIAL_8N1, RXp1, TXp1); // 중계1과 통신하는 시리얼 
  radio.begin();
  radio.openWritingPipe(pipeOut);
  radio.setDataRate(RF24_2MBPS);  // 데이터 전송 속도 설정
  // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  radio.setPALevel(RF24_PA_HIGH);
  //거리가 가까운 순으로 RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX 등으로 설정
  radio.stopListening(); // 송신모드
  resetData();
}

//-----------------------------------------------------------------------
//--------------------------LOOP----------------------------------------
//-----------------------------------------------------------------------
void loop() {
  if(Serial2.available()>0)
  {   
    String inString = Serial2.readStringUntil('\n');

     // 데이터 길이 확인
    if(inString.length() == 42)  
    {
      //스트링 값을 분할 후 저장
      test.test_throttle = inString.substring(1, 4);
      test.test_yaw = inString.substring(5, 8);
      test.test_pitch = inString.substring(9, 12);
      test.test_roll = inString.substring(13, 16);
      test.test_AUX1 = inString.substring(17, 20);
      test.test_AUX2 = inString.substring(21, 24);
      test.test_AUX3 = inString.substring(25, 28);
      test.test_AUX4 = inString.substring(29, 32);
      test.test_AUX5 = inString.substring(33, 36);
      test.test_AUX6 = inString.substring(37, 40);
      
      //받은 후 인티져로 변환하여 저장
      data.throttle = test.test_throttle.toInt();  
      data.yaw = test.test_yaw.toInt();
      data.pitch = test.test_pitch.toInt();
      data.roll = test.test_roll.toInt();
      data.AUX1 = test.test_AUX1.toInt();
      data.AUX2 = test.test_AUX2.toInt();
      data.AUX3 = test.test_AUX3.toInt();
      data.AUX4 = test.test_AUX4.toInt();
      data.AUX5 = test.test_AUX5.toInt();
      data.AUX6 = test.test_AUX6.toInt();

      //==============================================================================
      //=====================중계 드론 개별 정지==========================================

      if(data.AUX3 == 1) 
      {
        lastData.yaw = 127; // 중간값으로 설정
        lastData.pitch = 127;
        lastData.roll = 127;
        lastData.AUX1 = 0;
        lastData.AUX2 = 1;
        lastData.AUX5 = 0;
        lastData.AUX6 = 1;
        nano(lastData);
        serial_print(lastData); //모니터 값 출력(확인용)
      } 
      else 
      {
        lastData = data;  // 통신 데이터를 lastData에 저장
        nano(data);
        serial_print(data); //모니터 값 출력(확인용)6
      }
    
      //==============================================================================
      //==============================================================================
     
      radio.write(&data, sizeof(MyData)); //라디오 통신, 메인드론으로 전송
    }
    else
//      Serial2.println("error");

    delay(10);
  }
}

//-----------------------------------------------------------------------------
//--------------------------중계 나노로 보내는 데이터 전송 함수---------------------
//-----------------------------------------------------------------------------
void nano(MyData sendData)
{
  Serial1.print("A");
  sprintf(buf, "%03d", sendData.throttle);
  Serial1.print(buf);
  Serial1.print("B");
  sprintf(buf, "%03d", sendData.yaw);
  Serial1.print(buf);
  Serial1.print("C");
  sprintf(buf, "%03d", sendData.pitch);
  Serial1.print(buf);
  Serial1.print("D");
  sprintf(buf, "%03d", sendData.roll);
  Serial1.print(buf);
  Serial1.print("E");
  sprintf(buf, "%03d", sendData.AUX1);
  Serial1.print(buf);
  Serial1.print("F");
  sprintf(buf, "%03d", sendData.AUX2);
  Serial1.print(buf);
  Serial1.println("G");
  
  delay(10);
}

//-----------------------------------------------------------------------
//-------------------------들어오는 데이터 맵핑값 출력함수--------------------
//-----------------------------------------------------------------------
void serial_print(MyData sendData)
{
  Serial2.print("중계기 2 => ");
  Serial2.print("Throttle: ");
  Serial2.print(sendData.throttle);
  Serial2.print("  Yaw: ");
  Serial2.print(sendData.yaw);
  Serial2.print("  Pitch: ");
  Serial2.print(sendData.pitch);
  Serial2.print("  Roll: ");
  Serial2.print(sendData.roll);
  Serial2.print("  AUX1: ");
  Serial2.print(sendData.AUX1);
  Serial2.print("  AUX2: ");
  Serial2.print(sendData.AUX2);
  Serial2.print("  AUX3: ");
  Serial2.print(sendData.AUX3); 
  Serial2.print("  AUX4: ");
  Serial2.print(sendData.AUX4);
  Serial2.print("  AUX5: ");
  Serial2.print(sendData.AUX5); 
  Serial2.print("  AUX6: ");
  Serial2.println(sendData.AUX6); 
}
