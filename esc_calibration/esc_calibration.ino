#include <Servo.h>

#define MAX_SIGNAL 2000
#define MIN_SIGNAL 1000
#define MOTOR_PIN 9
int DELAY = 1000;

Servo motor;

void setup() {
  Serial.begin(9600);
  Serial.println("ESC캘리...");
  Serial.println(" ");
  delay(1500);
  Serial.println("프로그램 시작..");
  delay(1000);
  Serial.println("이 프로그램은 ESC를 조정합니다.");

  motor.attach(MOTOR_PIN);

  Serial.print("최대 출력 작성: (");
  Serial.print(MAX_SIGNAL);
  Serial.print(" 값이 최대 값)");
  Serial.print("\n");
  Serial.println("전원을 키세요, 2초 후 아무 키나 누르세요.");
  motor.writeMicroseconds(MAX_SIGNAL);

  // Wait for input
  while (!Serial.available());
  Serial.read();

  // Send min output
  Serial.println("\n");
  Serial.println("\n");
  Serial.print("최소값을 작성: (");
  Serial.print(MIN_SIGNAL);
  Serial.print(" 값이 최소 값)");
  Serial.print("\n");
  motor.writeMicroseconds(MIN_SIGNAL);
  Serial.println("ESC가 조정되었습니다.");
  Serial.println("----");
  Serial.println("1000 ~ 2000 값을 작성하고 ENTER 키를 누르세요");
  Serial.println("모터가 회전합니다.");
  Serial.println("1000 = 모터 정지 / 2000 = 최대 출력");

}

void loop() {
  if (Serial.available() > 0)
  {
    int DELAY = Serial.parseInt();
    if (DELAY > 999)
    {
      motor.writeMicroseconds(DELAY);
      float SPEED = (DELAY-1000)/10;
      Serial.print("\n");
      Serial.println("모터 속도:");
      Serial.print("  ");
      Serial.print(SPEED);
      Serial.print("%"); 
    }     
  }
} 
