//esp32 드론 시뮬레이터 수신기
//라디오 통신으로 받은 값을 처리하여 유나티로 시리얼 전송
#include <SPI.h>
#include <RF24.h>

const uint64_t pipeIn = 0xABCD1234567890EFLL;   //주소값 설정

char buf[10]; //  

RF24 radio(4, 5); //CE - 4번, CSN - 5번

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

boolean SData = false;

void setup() 
{
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, pipeIn);
  radio.startListening(); //수신 모드
}

void loop() 
{ 
  if(radio.available()) 
  {
    radio.read(&receivedData, sizeof(MyData));
//    test_data();
    serial_trans(receivedData);
    delay(10);
    SData = true;
  }
  else
  {
    if(SData == false)
    {
      resetData();
      for(int i = 0; i < 10; i++)
      {
        serial_trans(receivedData);
        test_data();
        if(radio.available())
        {
          break;
        }
      }
      delay(10);
    }
    
  }
}

void test_data()
{
  Serial.print("test - Throttle: ");
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
  Serial.print(receivedData.AUX4);
  Serial.print("  AUX5: ");
  Serial.print(receivedData.AUX5);
  Serial.print("  AUX6: ");
  Serial.println(receivedData.AUX6);
}

void serial_trans(MyData sendData)
{
  Serial.print("A");
  sprintf(buf, "%03d", sendData.throttle);
  Serial.print(buf);
  Serial.print("B");
  sprintf(buf, "%03d", sendData.yaw);
  Serial.print(buf);
  Serial.print("C");
  sprintf(buf, "%03d", sendData.pitch);
  Serial.print(buf);
  Serial.print("D");
  sprintf(buf, "%03d", sendData.roll);
  Serial.print(buf);
  Serial.print("E");
  sprintf(buf, "%03d", sendData.AUX1);
  Serial.print(buf);
  Serial.print("F");
  sprintf(buf, "%03d", sendData.AUX2);
  Serial.print(buf);
  Serial.print("G");
  sprintf(buf, "%03d", sendData.AUX3);
  Serial.print(buf);
  Serial.print("H");
  sprintf(buf, "%03d", sendData.AUX4);
  Serial.print(buf);
  Serial.print("I");
  sprintf(buf, "%03d", sendData.AUX5);
  Serial.print(buf);
  Serial.print("J");
  sprintf(buf, "%03d", sendData.AUX6);
  Serial.print(buf);
  Serial.println("K");
}

void resetData() 
{
  receivedData.throttle = 0;
  receivedData.yaw = 127;
  receivedData.pitch = 127;
  receivedData.roll = 127;
  receivedData.AUX1 = 0;
  receivedData.AUX2 = 0;
  receivedData.AUX3 = 0;
  receivedData.AUX4 = 0;
  receivedData.AUX5 = 0;
  receivedData.AUX6 = 0;
}
