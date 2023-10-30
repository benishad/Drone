//조종기 ESP32
#include <SPI.h>
#include <RF24.h>

//const uint64_t pipeOut = 0xE8E8F0F0E1LL;
//long long pipeOut = 0x1324ABCDEFLL;   //주소값 설정
const uint64_t pipeOut = 0xABCD1234567890EFLL;


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
  radio.stopListening(); // Listening을 멈춤
  resetData();

  pinMode(33, INPUT);
  pinMode(32, INPUT);
  pinMode(25, INPUT);
  pinMode(26, INPUT);
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
  data.throttle = mapJoystickValues(analogRead(34), 50, 1850, 3800, true);
  data.yaw      = mapJoystickValues(analogRead(14), 5, 2080, 4095, true);
  data.pitch    = mapJoystickValues(analogRead(15), 80, 1750, 4095, true);
  data.roll     = mapJoystickValues(analogRead(35), 0, 2060, 4095, false);
  data.AUX1     = digitalRead(32);
  data.AUX2     = digitalRead(33);
  data.AUX3     = digitalRead(25);
  data.AUX4     = digitalRead(26);

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
  Serial.println(data.AUX4);

  //delay(100); // 일정 시간 간격으로 송신 (500ms = 0.5초)
}
