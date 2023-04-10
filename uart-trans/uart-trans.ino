//중계 1
// !!!!!!!!!!!!!!!!!! 현재 MPU6050()함수 안 302번째 줄에 constrain으로 속도 제한 걸어둠!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define MOTOR_SPEED_LIMIT 1200
#define TURN_MAX_SPEED 1000
#define TURN_MIN_SPEED 200
#include <HardwareSerial.h>                            //nrf 쓰기위한 헤더파일
#include <SPI.h>
#include "RF24.h"
//--------------------------------------------------------------------------
#include <Wire.h>                                      // mpu-6050쓰기위한 헤더파일
#include "BluetoothSerial.h"
#include <ESP32_Servo.h>
//---------------------------------------------------------------------------

HardwareSerial Unit1(1);

//읽기------------------------------------------------------------------

int read_joystick_x_1 = 0; // 조이스틱 x의 값을 변수 선언
int read_joystick_y_1 = 0; // 조이스틱 y의 값을 변수 선언
int read_joystick_x_2 = 0;
int read_joystick_y_2 = 0;

//로그------------------------------------------------------------------

int logJoystickValueX_1 = 0;
int logJoystickValueY_1 = 0;
int logJoystickValueX_2 = 0;
int logJoystickValueY_2 = 0;

char buf[35];

int message[4];

// 0번과 1번으로 송수신을 결정
// 수신 아두이노는 0으로, 송신 아두이노는 1로 설정하고 컴파일

RF24 radio(12,14); // 7번핀 CE, 8번핀 CSN으로 SPI통신 설정

// pipe adresses를 설정하기 위한 값
// 하나의 Rx가 6개까지의 Tx와 통신가능
// 5 byte의 문자열로 주소값 설정가능
byte addresses[6] = "12345";

//모터선언 -----------------------------------------------------------------
/*
const int MOTOR_A_val = 15;                           // 모터의 핀 배치
const int MOTOR_B_val = 2;
const int MOTOR_C_val = 26;
const int MOTOR_D_val = 27;
*/
const int MOTOR_A_val = 15;                           // 모터의 핀 배치
const int MOTOR_B_val = 2;
const int MOTOR_C_val = 27;
const int MOTOR_D_val = 26;


Servo MOTOR_A;
Servo MOTOR_B;
Servo MOTOR_C;
Servo MOTOR_D;

BluetoothSerial SerialBT;

int throttle=700;  //시작 높이 0


//-------------------------------------------------------------------------
// 모터변수

double speedA ; // 1사분면 모터
double speedB ; // 2사분면 모터
double speedC ; // 3사분면 모터
double speedD ; // 4사분면 모터


//-------------------------------------------------------------------------

void setup() {
// nrf설정 부분 ------------------------------------------------------------------------------------
  Serial.begin(115200);
  Unit1.begin(115200, SERIAL_8N1, 17,16);
  radio.begin(); // nRF24L01모듈 초기화
// 전원 공급 관련 문제가 발생하지 않도록 PA레벨을 LOW로 설정, RF24_PA_MAX가 기본값
// RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
// NRF24L01: -18dBm, -12dBm,-6dBM, and 0dBm
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0,addresses); // 데이터를 받을 주소 설정
  radio.startListening(); // 읽는 pipe 주소의 data Listening 시작
//------------------------------------------------------------------------------------------------  
// mpu6050과 종속되어야함  --------------------------------------------------------------------------
  Wire.begin();                           // i2c통신 활성화
  Wire.setClock(400000);                  // i2c통신속도 설정 함수, 속도는 100000,400000밖에 없음

  Wire.beginTransmission(0x68);           // 슬레이브역할을 맡고있는 6050의 레지스터인 0x68(주소값)과 통신을 시작한다는 의미
  Wire.write(0x6b);                                   
  Wire.write(0x0);
  Wire.endTransmission(true);
//------------------------------------------------------------------------------------------------
  MOTOR_A.attach(MOTOR_A_val);
  MOTOR_B.attach(MOTOR_B_val);
  MOTOR_C.attach(MOTOR_C_val);
  MOTOR_D.attach(MOTOR_D_val);      
  MOTOR_A.writeMicroseconds(throttle);
  MOTOR_B.writeMicroseconds(throttle);
  MOTOR_C.writeMicroseconds(throttle);
  MOTOR_D.writeMicroseconds(throttle);
}

void loop() {
  
  MPU6050();
  if(radio.available())
  {
    radio.read(message, sizeof(message));
    
    READJOYSTICK();
    WRITEPRINT();
    MOTORLOG();
    //LOG();
    
      
    
  }
  
  MOTOR_A.writeMicroseconds(speedA);
  MOTOR_B.writeMicroseconds(speedB);
  MOTOR_C.writeMicroseconds(speedC);
  MOTOR_D.writeMicroseconds(speedD);
}

void READJOYSTICK(){
  read_joystick_x_1 = message[0];
  read_joystick_y_1 = message[1];
  read_joystick_x_2 = message[2];
  read_joystick_y_2 = message[3];

  logJoystickValueX_1 = read_joystick_x_1;
  logJoystickValueY_1 = read_joystick_y_1;
  logJoystickValueX_2 = read_joystick_x_2;
  logJoystickValueY_2 = read_joystick_y_2;
}

void WRITEPRINT(){
  Unit1.print("A");
  sprintf(buf, "%04d", read_joystick_x_1);
  Unit1.print(buf);
  Unit1.print("B");
  sprintf(buf, "%04d", read_joystick_y_1);
  Unit1.print(buf);
  Unit1.print("C");
  sprintf(buf, "%04d", read_joystick_x_2);
  Unit1.print(buf);
  Unit1.print("D");
  sprintf(buf, "%04d", read_joystick_y_2);
  Unit1.println("E");
}

void LOG(){
  sprintf(buf, "%04d", logJoystickValueX_1);
  Serial.print(buf);
  Serial.print("A");
  sprintf(buf, "%04d", logJoystickValueY_1);
  Serial.print(buf);
  Serial.print("A");
  sprintf(buf, "%04d", logJoystickValueX_2);
  Serial.print(buf);
  Serial.print("A");
  sprintf(buf, "%04d", logJoystickValueY_2);
  Serial.print(buf);
  Serial.println("A");
}
void MOTORLOG(){
  
  Serial.print("current value : ");
  Serial.print(throttle); 
  Serial.print("Y1 : ");
  Serial.println(logJoystickValueY_1);
  Serial.printf(" | A = %6.1f" , speedA); //1사분면모터분배속도
  Serial.printf(" | B = %6.1f" , speedB); //2사분면모터분배속도
  Serial.printf(" | C = %6.1f" , speedC); //3사분면모터분배속도
  Serial.printf(" | D = %6.1f" , speedD); //4사분면모터분배속도
  Serial.println();
  
}
void MPU6050(){
  Wire.beginTransmission(0x68);
  Wire.write(0x3b);
  Wire.endTransmission(false);
  Wire.requestFrom((uint16_t)0x68,(uint8_t)14,true);

  int16_t AcXH =Wire.read();
  int16_t AcXL =Wire.read();
  int16_t AcYH =Wire.read();
  int16_t AcYL =Wire.read();
  int16_t AcZH =Wire.read();
  int16_t AcZL =Wire.read();
  int16_t TepH =Wire.read();
  int16_t TepL =Wire.read();
  int16_t GyXH =Wire.read();
  int16_t GyXL =Wire.read();
  int16_t GyYH =Wire.read();
  int16_t GyYL =Wire.read();
  int16_t GyZH =Wire.read();
  int16_t GyZL =Wire.read();   //센서가 읽어서 레지스터에 저장된 값들을 읽어와 각각 변수에 저장하는 문장들

  int16_t AcX = AcXH <<8 |AcXL;
  int16_t AcY = AcYH <<8 |AcYL;
  int16_t AcZ = AcZH <<8 |AcZL;
  int16_t GyX = GyXH <<8 |GyXL;
  int16_t GyY = GyYH <<8 |GyYL;
  int16_t GyZ = GyZH <<8 |GyZL;

  //MPU-6050보정하는 부분 시작
  static int32_t AcXSum =0, AcYSum =0, AcZSum=0; // 가속도센서의 각 축값들 1000번씩 더해서 저장할 변수선언. static활용하여 루프문 빠져나가도 변수값 유지
  static int32_t GyXSum =0, GyYSum =0, GyZSum=0; // 자이로센서                          "
  static double AcXOff =0, AcYOff =0, AcZOff=0;  
  static double GyXOff =0, GyYOff =0, GyZOff=0;  // 가속도, 자이로 센서의 평균값들 저장해 놓는 변수선언. 평균때문에 소수점 나올수 있어서 double로 선언

  static int cnt_sample =1000;  // 센서들의 평군값을 구하기 위해 센서값을 읽어올 횟수를 저장할 변수 선언 1000번 더해서 1000으로 초기화
  if(cnt_sample>0)
  {
    AcXSum += AcX; AcYSum +=AcY; AcZSum +=AcZ;
    GyXSum += GyX; GyYSum +=GyY; GyZSum +=GyZ; // 각축 센서들의 값을 계속 더해준다
    cnt_sample --;
    if(cnt_sample ==0)                        
    {
      AcXOff = AcXSum/1000.0;
      AcYOff = AcYSum/1000.0;
      AcZOff = AcZSum/1000.0;
      GyXOff = GyXSum/1000.0;
      GyYOff = GyYSum/1000.0;
      GyZOff = GyZSum/1000.0;  // 1000번 더한 값들을 1000으로 나누어 평균값을 Off변수들에 저장하기
    }
    delay(1);  
    return; // 각축의 평균값들을 구하지 못하면 67번째줄 실행X
  }

  double AcXD = AcX - AcXOff;  
  double AcYD = AcY - AcYOff;
  double AcZD = AcZ - AcZOff + 16384;

  double GyXD = GyX - GyXOff;
  double GyYD = GyY - GyYOff;
  double GyZD = GyZ - GyZOff;  // 각 센서 측정값에서 센서 오차값을 뺌으로서 센서의 오차를 0으로 만들어줌
  //MPU-6050보정하는 부분 끝

  //주기시간 계산하는 부분 시작
  static unsigned long t_prev =0;  // 바로 이전에 센서를 읽은 시간을 저장
  unsigned long t_now = micros();  // micros()함수를 호출해 센서값을 측정한 현재시간을 변수에 저장
  double dt =(t_now - t_prev)/1000000.0;  // 현재값에서 이전값을 빼고 1000000.0으로 나누어 초단위로 변환후 dt에 저장
  t_prev = t_now; // 현재시간이 이전시간으로 되고 두줄위에있는 센서값에서 현재시간을 다시 추출
  //주기시간: 0.500ms정도 나옴
  //주기시간 계산하는 부분 끝


  //자이로센서가 읽은 값들로 각도표시하는 부분 시작
  const float GYROXYZ_TO_DEGREES_PER_SEC = 131;  // ->따로 설명
  double GyXR = GyXD / GYROXYZ_TO_DEGREES_PER_SEC;
  double GyYR = GyYD / GYROXYZ_TO_DEGREES_PER_SEC;
  double GyZR = GyZD / GYROXYZ_TO_DEGREES_PER_SEC;  // 보정 자이로 값을 넣어 131로 나눈다음 각축의 변수에 저장한다.

  static double gyAngleX = 0.0, gyAngleY = 0.0, gyAngleZ =0.0; // 각도를 저장할 변수, static으로 넣어서 루프문 빠져나가도 값이 유지
  gyAngleX += GyXR*dt; 
  gyAngleY += GyYR*dt;
  gyAngleZ += GyZR*dt; //현재의 각도는 = 이전각도 + (각속도 x 시간)
  //자이로센서가 읽은 값들로 각도표시하는 부분 끝
  

  //가속도센서가 읽은 값들로 각도표시하는 부분 시작
  const float RADIANS_TO_DEGREES = 180/3.14159;  
  double AcYZD = sqrt(pow(AcY,2) + pow(AcZ,2));
  double AcXZD = sqrt(pow(AcX,2) + pow(AcZ,2));
  double acAngleY = atan(-AcXD/AcYZD)*RADIANS_TO_DEGREES;
  double acAngleX = atan(AcYD/AcXZD)*RADIANS_TO_DEGREES;
  double acAngleZ = 0;
  //가속도센서가 읽은 값들로 각도표시하는 부분 끝 

  //상보필터(상호보완필터) 시작
  const double ALPHA = 0.96;  // 가속도 센서와 자이로센서의 값을 얼마나 반영할지의 비율. 자유롭게 조정가능
  static double cmAngleX = 0.0,cmAngleY = 0.0, cmAngleZ =0.0; //상보필터를 적용한 값들을 저장할 변수 선언
  cmAngleX = ALPHA*(cmAngleX+GyXR*dt)+(1.0-ALPHA)*acAngleX; 
  cmAngleY = ALPHA*(cmAngleY+GyYR*dt)+(1.0-ALPHA)*acAngleY; //상호보완필터의 핵심...문장... 이름은 거창하지만 코드는 한줄이누
  cmAngleZ = gyAngleZ; // z값은 자이로센서만 사용해서 오차가 발생할 수밖에 없을듯... 가속도센서로는 못구함 구하려면 지자기센서 들어가있는 MPU9250을 써야해

  static double tAngleX = 0.0, tAngleY =0.0, tAngleZ =0.0;  //드론의 목표 각도를 저장할 변수 선언
  double eAngleX = tAngleX-cmAngleX; 
  double eAngleY = tAngleY-cmAngleY;
  double eAngleZ = tAngleZ-cmAngleZ;  // 목표각도와 현재각도를 빼서 오차 각도를 구한다.

  double kp = 1.5;  // 비례항의 계수값을 1로 설정. 힘이 더필요하다면 이걸 조정하면 됨.
  // ------------------------------------ 이중 PID -----------------------------------------
  static double tRateX = 0.0, tRateY =0.0, tRateZ = 0.0;
  tRateX = kp*eAngleX;
  tRateY = kp*eAngleY;
  tRateZ = kp*eAngleZ;

  double eRateX = tRateX - GyXR;
  double eRateY = tRateY - GyYR;
  double eRateZ = tRateZ - GyZR;
  if(throttle ==0) eRateX = eRateY = eRateZ = 0.0;
  
  double kd = 2;  // 미분항의 계수값을 1로 설정
  double BalX = kp * eRateX; // 좌우균형
  double BalY = kp * eRateY; // 전후균형
  double BalZ = kp * eRateZ; // 수평좌우균형

  double Kri = 1.0;
  double ResRateX = 0.0, ResRateY = 0.0, ResRateZ = 0.0;
  ResRateX += Kri * eRateX * dt;
  ResRateY += Kri * eRateY * dt;
  ResRateZ += Kri * eRateZ * dt;
  if(throttle ==0) ResRateX = ResRateY = ResRateZ =0.0;
  
  BalX += ResRateX; 
  BalY += ResRateY;
  BalZ += ResRateZ; // -가 붙은 이유는 목표값에 혼선을 주지않기 위해 -를 붙힌것임
  if(throttle ==0) BalX = BalY = BalZ = 0.0;
//----------------------------------드론 좌우 기울기 조정-----------------------------
  if(logJoystickValueX_2 >=TURN_MAX_SPEED)
  {
    tAngleX = -10.0;
  }
  else if(logJoystickValueX_2 <=TURN_MIN_SPEED)
  {
    tAngleX = 10.0;
  }
  else if ((logJoystickValueX_2 <TURN_MAX_SPEED)&&(logJoystickValueX_2 >TURN_MIN_SPEED))
  {
    tAngleX = 0.0;
  }
//--------------------------------------------------------------------------------
//----------------------------------드론 앞뒤 기울기 조정-----------------------------
  if(logJoystickValueY_2 >=TURN_MAX_SPEED)
  {
    tAngleY = -10.0;
  }
  else if(logJoystickValueY_2 <=TURN_MIN_SPEED)
  {
    tAngleY = 10.0;
  }
  else if ((logJoystickValueY_2 <TURN_MAX_SPEED)&&(logJoystickValueY_2 >TURN_MIN_SPEED))
  {
    tAngleY = 0.0;
  }
//--------------------------------------------------------------------------------
//----------------------------------드론 좌우 방향 조정-----------------------------
  if(logJoystickValueX_1 >=TURN_MAX_SPEED)
  {
    tAngleZ = -10.0;
  }
  else if(logJoystickValueX_1 <=TURN_MIN_SPEED)
  {
    tAngleZ = 10.0;
  }
  else if ((logJoystickValueX_1 <TURN_MAX_SPEED)&&(logJoystickValueX_1 >TURN_MIN_SPEED))
  {
    tAngleZ = 0.0;
  }
//--------------------------------------------------------------------------------
  double ki = 1.0;
  double ResX = 0.0, ResY = 0.0, ResZ = 0.0;
  ResX += ki*eAngleX * dt;
  ResY += ki*eAngleY * dt;
  ResZ += ki*eAngleZ * dt;
  if(throttle == 0) ResX = ResY = ResZ =0.0;
  BalX += ResX;
  BalY += ResY;
  BalZ += ResZ;

  speedA = throttle + BalX - BalY + BalZ; // 1사분면 모터
  speedB = throttle - BalX - BalY - BalZ; // 2사분면 모터
  speedC = throttle - BalX + BalY + BalZ; // 3사분면 모터
  speedD = throttle + BalX + BalY - BalZ; // 4사분면 모터
  throttle = constrain(logJoystickValueY_1,700,MOTOR_SPEED_LIMIT); //현재 안정성 때문에 constrain함수로 값 제한 걸어둠
}
