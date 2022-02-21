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
