int dataPin = 9;    // к выводу 14 регистра SD
int clockPin = 11;  // к выводу 11 регистра (SH_CP)
int latchPin = 12;  // к выводу 12 регистра (ST_CP)

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(latchPin, LOW);
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
  digitalWrite(latchPin, LOW); // начинаем передачу данных
  shiftOut(dataPin, clockPin, LSBFIRST, 0b00010000);
  digitalWrite(latchPin, HIGH); // прекращаем передачу данных
  delay(2000);
  digitalWrite(latchPin, LOW); // начинаем передачу данных
  shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);
  digitalWrite(latchPin, HIGH); // прекращаем передачу данных
  delay(2000);
}
