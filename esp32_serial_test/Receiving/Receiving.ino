#include <HardwareSerial.h>
HardwareSerial Unit2(1);

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(BaudRate, Data Protocol, Txd pin,Rxd pin);
  Serial.begin(115200);
  Unit2.begin(115200, SERIAL_8N1, 17,16);

}

void loop() {
  // put your main code here, to run repeatedly:
  while(Unit2.available()){
    char RxdChar=Unit2.read();
    Serial.println(RxdChar);
    
  }
  
}
