int dataPin = 9;    // к выводу 14 регистра SD
int clockPin = 11;  // к выводу 11 регистра (SH_CP)
int latchPin = 12;  // к выводу 12 регистра (ST_CP)

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(latchPin, LOW);
  Serial.begin(9600);
}

// 0b10000000 - none
// BUTTON control
// 0b01000000 - QB switch mode
// 0b00100000 - QC switch light
// 0b00010000 - QD off\on
// USB control
// 0b00001000 - QE
// 0b00000100 - QF
// 0b00000010 - QG
// 0b00000001 - none

void loop() {
  delay(2000);

  lamp_mode(1, 8);


  delay(2000);
}


void lamp_mode( int mode, int brightness ){

  if(brightness > 8){
    brightness = 8; 
  }else if(brightness < 1){
    brightness = 1;
  }

  if(mode > 3){
    mode = 3;  
  }else if(mode < 1){
    mode = 1;  
  }
  
// включаю лампу
  digitalWrite(latchPin, LOW); // начинаем передачу данных
  shiftOut(dataPin, clockPin, LSBFIRST, 0b00010000);
  digitalWrite(latchPin, HIGH); // прекращаем передачу данных
  delay(50);
  digitalWrite(latchPin, LOW); // начинаем передачу данных
  shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);
  digitalWrite(latchPin, HIGH); // прекращаем передачу данных

// устанавливаю яркость
  int need_press_button_brightness = 8 - brightness;

  for(int i=1; i<need_press_button_brightness ; i++){
    digitalWrite(latchPin, LOW); // начинаем передачу данных
    shiftOut(dataPin, clockPin, LSBFIRST, 0b00100000);
    digitalWrite(latchPin, HIGH); // прекращаем передачу данных
    delay(50);
    digitalWrite(latchPin, LOW); // начинаем передачу данных
    shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);
    digitalWrite(latchPin, HIGH); // прекращаем передачу данных
    delay(50);
  }

//  устанавливаю режим
  for(int i=1; i<mode ; i++){
    digitalWrite(latchPin, LOW); // начинаем передачу данных
    shiftOut(dataPin, clockPin, LSBFIRST, 0b01000000);
    digitalWrite(latchPin, HIGH); // прекращаем передачу данных
    delay(50);
    digitalWrite(latchPin, LOW); // начинаем передачу данных
    shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);
    digitalWrite(latchPin, HIGH); // прекращаем передачу данных
    delay(50);
  }

}
