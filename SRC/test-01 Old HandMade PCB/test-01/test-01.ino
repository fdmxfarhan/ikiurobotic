void setup() {
  pinMode(PA15, OUTPUT);
  pinMode(PB3, OUTPUT);
  pinMode(PB4, OUTPUT);
  pinMode(PB5, OUTPUT);
  pinMode(PB6, OUTPUT);
  pinMode(PB7, OUTPUT);
  pinMode(PB8, OUTPUT);
  pinMode(PB9, OUTPUT);
  pinMode(PB12, OUTPUT);
  pinMode(PB13, OUTPUT);
  pinMode(PB14, OUTPUT);
  pinMode(PB15, OUTPUT);
  
  
}

void loop() {
  
  digitalWrite(PB13, 0); //R1 (back)
  digitalWrite(PB12, 0); //R1 (forward)
  
  
  digitalWrite(PB7 , 0); //l1 (back)
  digitalWrite(PB6 , 0); //L1 (forward)
  
  
  digitalWrite(PB4 , 0); //l2 (back)
  digitalWrite(PA15, 1); //L1 (back)
  
  digitalWrite(PB9 , 0); //R2 (back)
  digitalWrite(PB5 , 0); //R2 (forward)
  
  
}
