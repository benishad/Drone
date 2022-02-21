#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);//처음 7,8

const byte address[6] = "00001";

int button = 4;
boolean button_state = 0;

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  pinMode(button, INPUT_PULLUP);
}

void loop() {
  button_state = digitalRead(button);
  if(button_state == HIGH){
    const char text[] = "Hello Button";
    radio.write(&text, sizeof(text));
  }
  else{
    const char text[] = "Bye Button";
    radio.write(&text, sizeof(text));
  }
  radio.write(&button_state, sizeof(button_state));
  delay(1000);
}
