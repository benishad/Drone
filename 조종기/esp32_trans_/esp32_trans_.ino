//조종기 ESP32
//조종기에는 조이스틱 두개와 토글 스위치 5개, 푸쉬 버튼 1개가 설치됨
#include <SPI.h>
#include <RF24.h>

const uint64_t pipeOut = 0xABCD1234567890EFLL;  //주소값 설정

RF24 radio(4, 5); // 4번 ce 5번 csn

//조이스틱 핀 할당
const int THR = 34;
const int YAW = 14;
const int PITCH = 15;
const int ROLL = 35;
//각종 버튼 핀 할당
const int SW1 = 32;
const int SW2 = 33;
const int SW3 = 25;
const int SW4 = 26;
const int SW5 = 12;
const int SW6 = 13;

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
  data.AUX2 = 0;
  data.AUX3 = 0;
  data.AUX4 = 0;
  data.AUX5 = 0;
  data.AUX6 = 0;
}

void setup()
{
  Serial.begin(115200);
  radio.begin();
  //radio.setAutoAck(false);
  //radio.setDataRate(RF24_250KBPS);
  // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  //radio.setPALevel(RF24_PA_HIGH);
  //거리가 가까운 순으로 RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX 등으로 설정
  radio.openWritingPipe(pipeOut);
  radio.stopListening(); // 송신모드
  resetData();

  pinMode(SW1, INPUT);    //angle 모드
  pinMode(SW2, INPUT);    //horizon 모드
  pinMode(SW3, INPUT);    //중계 단일 호버링 모드
  pinMode(SW4, INPUT);    //메인 단일 호버링 모드
  //응용 핀
  pinMode(SW5, INPUT);    //코일건 충전
  pinMode(SW6, INPUT);    //코일건 발사
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
  data.throttle = mapJoystickValues(analogRead(THR), 260, 1890, 4030, false);
  data.yaw      = mapJoystickValues(analogRead(YAW), 0, 2077, 4095, true);
  data.pitch    = mapJoystickValues(analogRead(PITCH), 1179, 1776, 4095, true);
  data.roll     = mapJoystickValues(analogRead(ROLL), 0, 2061, 4095, false);
  data.AUX1     = digitalRead(SW1);
  data.AUX2     = digitalRead(SW2);
  data.AUX3     = digitalRead(SW3);
  data.AUX4     = digitalRead(SW4);
  data.AUX5     = digitalRead(SW5);
  data.AUX6     = digitalRead(SW6);

  radio.write(&data, sizeof(MyData));

  //   송신하는 값을 시리얼 모니터에 출력
  Serial.print("조종기 => ");
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

////   조이스틱 테스트 코드, 최소 최대 중간 값을 확인하고 수정
//  Serial.print("조종기 => ");
//  Serial.print("Throttle: ");
//  Serial.print(analogRead(THR));
//  Serial.print("  Yaw: ");
//  Serial.print(analogRead(YAW));
//  Serial.print("  Pitch: ");
//  Serial.print(analogRead(PITCH));
//  Serial.print("  Roll: ");
//  Serial.println(analogRead(ROLL));


  //delay(100); // 일정 시간 간격으로 송신 (500ms = 0.5초)
}
