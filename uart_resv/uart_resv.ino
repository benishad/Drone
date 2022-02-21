//중계 2
/*
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);                       //LCD 주소 값은 0x27이며 가로 16칸,세로 2칸 임

char data;

void setup() {
  Serial.begin(9600);
  lcd.init();                                             //LCD 초기화
  lcd.backlight();
}
void loop() {
  if(Serial.available())
  {
    data = Serial.read();
  }
  if(data == 'a')
  {
    lcd.setCursor(0, 0);
    lcd.print("Data IN");
  }
  else if(data =='b')
  {
    lcd.setCursor(0, 0);
    lcd.print("NONE");
  }
}
*/

//---------------------------------중계b 강화~
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include "RF24.h"
RF24 radio(7,8); // 7번핀 CE, 8번핀 CSN으로 SPI통신 설정
byte addresses[6] = "abcde";

LiquidCrystal_I2C lcd(0x27, 16, 2);                       //LCD 주소 값은 0x27이며 가로 16칸,세로 2칸 임

char data;

int message[2];
int x = 0;
int y = 0;
int sw=6; // 스위치 D6에 연결하고 변수 선언
int read_sw=0; // 스위치 읽은 값 변수 선언

void setup() {
  Serial.begin(115200);
  lcd.init();                                             //LCD 초기화
  lcd.backlight();
  radio.begin(); // nRF24L01모듈 초기화
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses); // 데이터를 받을 주소 설정
  radio.stopListening(); // 읽는 pipe 주소의 data Listening 시작
  pinMode(sw, INPUT_PULLUP); // 스위치 내부풀업 입력모드로 설정
}
void loop() {
  read_sw=digitalRead(6);
  
  if(Serial.available())
  {
    data = Serial.read();
    //Serial.println("0");
    if(data == 'a')
  {
    Serial.println("1");
    x = 2;
    y = 2;
    lcd.setCursor(0, 0);
    lcd.print("Data IN");
  }
  else if(data =='b')
  {
    Serial.println("0");
    x = 1;
    y = 1;
    lcd.setCursor(0, 0);
    lcd.print("NONE");
  }

  message[0] = x;
  message[1] = y;
  radio.write(message, sizeof(message)); // 해당 텍스트를 송신
  delay(50);//원래는100주고 시작했음
  }
 
  
 /*
  if(read_sw==1){
    x = 1;
    y = 1;
  }
  else{
    x = 2;
    y = 2;
  }
  */ 
}
