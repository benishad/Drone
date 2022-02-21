#include <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // 7,8 SPI 버스에 nRF24L01 라디오를 설정하기 위해 CE, CSN 선언.

const byte address[6] = "00001"; //주소값을 5가지 문자열로 변경할 수 있으며, 송신기과 수신기가 동일한 주소로 해야됨.

boolean button_state = 0;

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    radio.read(&button_state, sizeof(button_state));
    if(button_state == HIGH){
      Serial.println(text);
    }
    else{
      Serial.println(text);
    }
  }
  delay(10);
}
