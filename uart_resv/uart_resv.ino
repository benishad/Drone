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
#include <HardwareSerial.h>
#include <SPI.h>
#include "RF24.h"

HardwareSerial Unit2(1);

RF24 radio(12,14); // 7번핀 CE, 8번핀 CSN으로 SPI통신 설정
byte addresses[6] = "abcde";

char data;

int message[4];
int v = 0;
int x = 0;
int y = 0;
int z = 0;

void setup() {
  Serial.begin(115200);
  Unit2.begin(115200, SERIAL_8N1, 17,16);
  radio.begin(); // nRF24L01모듈 초기화
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses); // 데이터를 받을 주소 설정
  radio.stopListening(); // 읽는 pipe 주소의 data Listening 시작
}
void loop() {
 
  if(Unit2.available())
  {
    data = Unit2.read();
    //Serial.println("0");
    if(data == 'a')
  {
    Serial.println("X");
    x = 1;
    y = 0;
    z = 0;
    v = 0;
  }
  else if(data =='b')
  {
    Serial.println("Y");
    x = 0;
    y = 1;
    z = 0;
    v = 0;
  }
  else if(data =='c'){
    Serial.println("Z");
    x = 0;
    y = 0;
    z = 1;
    v = 0;
  }
  else if(data =='d'){
    Serial.println("V");
    x = 0;
    y = 0;
    z = 0;
    v = 1;
  }

  message[0] = x;
  message[1] = y;
  message[2] = z;
  message[3] = v;
  radio.write(message, sizeof(message)); // 해당 텍스트를 송신
  delay(5);
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
