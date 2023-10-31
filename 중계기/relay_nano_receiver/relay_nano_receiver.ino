// 중계기 나노 ppm생성기
//esp32로 시리얼 통신을 받은 값을 ppm신호를 발생시켜 fc(멀티위)에 전송한다.
//아래 코드는 멀티위 ppm신호 예제와 esp32코드를 혼합하여 제작한 코드이다.
//
////////////////////// PPM CONFIGURATION//////////////////////////
#define channel_number 6  //set the number of channels
#define sigPin 2  //set PPM signal output pin on the arduino
#define PPM_FrLen 27000  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PulseLen 400  //set the pulse length
//////////////////////////////////////////////////////////////////

int ppm[channel_number];
unsigned long lastRecvTime = 0;

struct MyData {
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
  byte AUX1;
  byte AUX2;
};

struct String_Data {
  String throttle;
  String yaw;
  String pitch;
  String roll;
  String AUX1;
  String AUX2;
};

MyData data;
String_Data test_data;



void resetData() 
{
  // 'safe' values to use when no radio input is detected
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  data.AUX1 = 0;
  data.AUX2= 0;
  
  setPPMValuesFromData();
}


void setupPPM() {
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, 0);  //set the PPM signal pin to the default state (off)

  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;

  OCR1A = 100;  // compare match register (not very important, sets the timeout for the first interrupt)
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();
}




void setup() {
  Serial.begin(9600);   //esp32 Serial2에서 사용한 보드레이트와 같게 맞춤

  resetData();
  setupPPM();
  
}

void loop() 
{
  if(Serial.available())
  {
    
    String value = Serial.readStringUntil('\n'); //전송 받은 값을 String으로 저장
    //Serial.println(value);
    if(value.length() == 26)//정확한 값을 받기 위한 처리
    {
      test_data.throttle = value.substring(1, 4);    //받은 스트링 값을 분할 후 인티져로 변환하여 저장
      test_data.yaw = value.substring(5, 8);
      test_data.pitch = value.substring(9, 12);
      test_data.roll = value.substring(13, 16);
      test_data.AUX1 = value.substring(17, 20);
      test_data.AUX2 = value.substring(21, 24);

      data.throttle = test_data.throttle.toInt();
      data.yaw = test_data.yaw.toInt();
      data.pitch = test_data.pitch.toInt();
      data.roll = test_data.roll.toInt();
      data.AUX1 = test_data.AUX1.toInt();
      data.AUX2 = test_data.AUX2.toInt();

      setPPMValuesFromData();

      
      Serial.print("중계기 나노 => Throttle: ");
      Serial.print(ppm[0]);
      Serial.print("  Yaw: ");
      Serial.print(ppm[1]);
      Serial.print("  Pitch: ");
      Serial.print(ppm[2]);
      Serial.print("  Roll: ");
      Serial.print(ppm[3]);
      Serial.print("  AUX1: ");
      Serial.print(ppm[4]);
      Serial.print("  AUX2: ");
      Serial.println(ppm[5]);
      
      
      Serial.print("data => Throttle: ");
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
      Serial.println(data.AUX2);
    
    }
  }
}


void setPPMValuesFromData()
{
  ppm[0] = map(data.throttle, 0, 255, 1000, 2000);
  ppm[1] = map(data.yaw,      0, 255, 1000, 2000);
  ppm[2] = map(data.pitch,    0, 255, 1000, 2000);
  ppm[3] = map(data.roll,     0, 255, 1000, 2000);
  ppm[4] = map(data.AUX1,     0, 1, 1000, 2000);
  ppm[5] = map(data.AUX2,     0, 1, 1000, 2000);  
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++







/**************************************************/

//#error Delete this line befor you cahnge the value (clockMultiplier) below
#define clockMultiplier 2 // set this to 2 if you are using a 16MHz arduino, leave as 1 for an 8MHz arduino

ISR(TIMER1_COMPA_vect){
  static boolean state = true;

  TCNT1 = 0;

  if ( state ) {
    //end pulse
    PORTD = PORTD & ~B00000100; // turn pin 2 off. Could also use: digitalWrite(sigPin,0)
    OCR1A = PPM_PulseLen * clockMultiplier;
    state = false;
  }
  else {
    //start pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;

    PORTD = PORTD | B00000100; // turn pin 2 on. Could also use: digitalWrite(sigPin,1)
    state = true;

    if(cur_chan_numb >= channel_number) {
      cur_chan_numb = 0;
      calc_rest += PPM_PulseLen;
      OCR1A = (PPM_FrLen - calc_rest) * clockMultiplier;
      calc_rest = 0;
    }
    else {
      OCR1A = (ppm[cur_chan_numb] - PPM_PulseLen) * clockMultiplier;
      calc_rest += ppm[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}
