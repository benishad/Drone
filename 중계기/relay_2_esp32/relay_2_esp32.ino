//중계2번 esp32 드론
//중계1번에서 받은 값을 다시 라디오 통신으로 메인 ESP32로 보내준다.
#include <SPI.h>
#include <RF24.h>
#define RXp1 3    //esp32 uart통신을 위한 핀
#define TXp1 1    //esp32 uart통신을 위한 핀

//long long pipeOut =  0x1234ABCDEFLL;   // 여기는 주소값이 바껴야 혼선이 없음
const uint64_t pipeOut = 0x5A1B2C3D4E5F6A7BLL;

String buffer = "";

RF24 radio(4, 5); // GPIO18 for CE, GPIO5 for CSN , 4번 ce 5번 csn

struct MyData {   //전송할 데이터 구조체 생성
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
  byte AUX1;
  byte AUX2;
  byte AUX3;
  byte AUX4;
};

struct TestValue
{
  String test_throttle;
  String test_yaw;
  String test_pitch;
  String test_roll;
  String test_AUX1;
  String test_AUX2;
  String test_AUX3;
  String test_AUX4;
};


MyData data;

TestValue test;

void resetData() 
{
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  data.AUX1 = 0;
  data.AUX2 = 0;
  data.AUX3 = 0;
  data.AUX4 = 0;
}

void setup()
{
  Serial2.begin(115200, SERIAL_8N1, RXp1, TXp1);
  radio.begin();
  radio.openWritingPipe(pipeOut);
  radio.setDataRate(RF24_2MBPS);  // 데이터 전송 속도 설정
  // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  radio.setPALevel(RF24_PA_HIGH);
  //거리가 가까운 순으로 RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX 등으로 설정
  radio.stopListening(); // Listening을 멈춤
  resetData();
  
}

void loop() {
  serial_event();
  
  delay(10);
}

void serial_event() {
  if(Serial2.available()>0)
  {
    String inString = Serial2.readStringUntil('\n');

     // 데이터 길이 확인
    if(inString.length() == 34)
    {
      test.test_throttle = inString.substring(1, 4);    //받은 스트링 값을 분할 후 인티져로 변환하여 저장
      test.test_yaw = inString.substring(5, 8);
      test.test_pitch = inString.substring(9, 12);
      test.test_roll = inString.substring(13, 16);
      test.test_AUX1 = inString.substring(17, 20);
      test.test_AUX2 = inString.substring(21, 24);
      test.test_AUX3 = inString.substring(25, 28);
      test.test_AUX4 = inString.substring(29, 32);
      
      data.throttle = test.test_throttle.toInt();    //받은 스트링 값을 분할 후 인티져로 변환하여 저장
      data.yaw = test.test_yaw.toInt();
      data.pitch = test.test_pitch.toInt();
      data.roll = test.test_roll.toInt();
      data.AUX1 = test.test_AUX1.toInt();
      data.AUX2 = test.test_AUX2.toInt();
      data.AUX3 = test.test_AUX3.toInt();
      data.AUX4 = test.test_AUX4.toInt();
      
      serial_print();
      radio.write(&data, sizeof(MyData));
    }
    else
      Serial2.println("error");
  }
}

void serial_print()
{
  Serial2.print("중계기 2 => ");
  Serial2.print("Throttle: ");
  Serial2.print(data.throttle);
  Serial2.print("  Yaw: ");
  Serial2.print(data.yaw);
  Serial2.print("  Pitch: ");
  Serial2.print(data.pitch);
  Serial2.print("  Roll: ");
  Serial2.print(data.roll);
  Serial2.print("  AUX1: ");
  Serial2.print(data.AUX1);
  Serial2.print("  AUX2: ");
  Serial2.print(data.AUX2);
  Serial2.print("  AUX3: ");
  Serial2.print(data.AUX3); 
  Serial2.print("  AUX4: ");
  Serial2.println(data.AUX4); 
}
