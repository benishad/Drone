//중계2번 esp32 드론
//중계1번에서 받은 값을 다시 라디오 통신으로 메인 ESP32로 보내준다.
#include <SPI.h>
#include <RF24.h>
#define RXp1 3    //esp32 uart통신을 위한 핀
#define TXp1 1    //esp32 uart통신을 위한 핀

long long pipeOut =  0x1234ABCDEFLL;   // 여기는 주소값이 바껴야 혼선이 없음

String buffer = "";

RF24 radio(4, 5); // GPIO18 for CE, GPIO5 for CSN , 4번 ce 5번 csn

struct MyData {   //전송할 데이터 구조체 생성
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
  byte AUX1;
  byte AUX2;
};

MyData data;

void resetData() 
{
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  data.AUX1 = 0;
  data.AUX2 = 0;
}

void setup()
{
  Serial2.begin(115200, SERIAL_8N1, RXp1, TXp1);
  radio.begin();
  radio.openWritingPipe(pipeOut);
  radio.setDataRate(RF24_2MBPS);  // 데이터 전송 속도 설정
  radio.setPALevel(RF24_PA_HIGH);
  //거리가 가까운 순으로 RF24_PA_MIN 
  // RF24_PA_LOW / RF24_PA_HIGH
  // RF24_PA_MAX 등으로 설정할 수 있다.
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
    if(inString.length() == 26)
    {
      data.throttle = inString.substring(1, 4).toInt();    //받은 스트링 값을 분할 후 인티져로 변환하여 저장
      data.yaw = inString.substring(5, 8).toInt();
      data.pitch = inString.substring(9, 12).toInt();
      data.roll = inString.substring(13, 16).toInt();
      data.AUX1 = inString.substring(17, 20).toInt();
      data.AUX2 = inString.substring(21, 24).toInt();
      serial_print();
      radio.write(&data, sizeof(MyData));
    }
    else
      Serial2.println("error");
  }
}

void serial_print()
{ 
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
    Serial2.println(data.AUX2); 
}
