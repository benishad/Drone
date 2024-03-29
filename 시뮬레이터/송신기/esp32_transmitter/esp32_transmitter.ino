//조종기 ESP32
//드론 시뮬레이터를 위한 드론 조종장치 중 조종기 코드
#include <SPI.h>
#include <RF24.h>

const uint64_t pipeOut = 0xABCD1234567890EFLL;   //주소값 설정

RF24 radio(4, 5); // 4번 ce 5번 csn

struct MyData {   //전송할 데이터 구조체 생성
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
  data.AUX6 = 1;
}

void setup()
{
  Serial.begin(9600);
  radio.begin();
  //radio.setAutoAck(false);
  //radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
  radio.stopListening(); // 송신 모드
  resetData();

  pinMode(33, INPUT);
  pinMode(32, INPUT);
  pinMode(25, INPUT);
  pinMode(26, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
}

/**************************************************/
//조이스틱에 중간 값이 조이스틱마다 저항 값에 차이가 있어서 그 값을 맞춰주는 함수
int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);
  if (val < middle)
    val = map(val, lower, middle, 0, 128);
  else
    val = map(val, middle, upper, 128, 255);
  return (reverse ? 255 - val : val);
}

void loop()
{
  data.throttle = mapJoystickValues(analogRead(34), 230, 1880, 4095, false);
  data.yaw      = mapJoystickValues(analogRead(14), 5, 2095, 4095, true);
  data.pitch    = mapJoystickValues(analogRead(15), 128, 1745, 4095, true);
  data.roll     = mapJoystickValues(analogRead(35), 0, 2030, 4095, false);
  data.AUX1     = digitalRead(32);
  data.AUX2     = digitalRead(33);
  data.AUX3     = digitalRead(25);
  data.AUX4     = digitalRead(26);
  data.AUX5     = digitalRead(12);
  data.AUX6     = digitalRead(13);

  radio.write(&data, sizeof(MyData));
  
//  TEST_OUT();
  SPRINT();

  //delay(100); // 일정 시간 간격으로 송신 (500ms = 0.5초)
}

void SPRINT(){
  //   송신하는 값을 시리얼 모니터에 출력
  Serial.print("조종기 값 -->  ");
  Serial.print("Throttle: ");
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

void TEST_OUT(){
  //직접 돌려보면서 최소 중간 최대 값을 찾기
  Serial.print("조종기 값 -->  ");
  Serial.print("Throttle: ");
  Serial.print(analogRead(34));
  Serial.print("  Yaw: ");
  Serial.print(analogRead(14));
  Serial.print("  Pitch: ");
  Serial.print(analogRead(15));
  Serial.print("  Roll: ");
  Serial.println(analogRead(35));
}
