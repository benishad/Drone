#include <HardwareSerial.h>
HardwareSerial Unit1(1);

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(BaudRate, Data Protocol, Txd pin,Rxd pin);
  Serial.begin(512000);
  Unit1.begin(512000, SERIAL_8N1, 17,16);

}

void loop() {
  // put your main code here, to run repeatedly:
  while(Unit1.available()){
    Unit1.write("Hello");
    delay(1500);
  }
  
}
