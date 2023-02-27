String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
long int cnt = 0;
void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  motor(0,0,0,0);
  inputString.reserve(200);
}

void loop() {
  int l1, l2, r2, r1,d;
  if (stringComplete) {
    if(inputString[0] == 'M'){
      l1  = (inputString[2] - 48) %10 *100;
      l1 += (inputString[3] - 48) %10 *10;
      l1 += (inputString[4] - 48) %10;
      l2  = (inputString[6] - 48) %10 *100;
      l2 += (inputString[7] - 48) %10 *10;
      l2 += (inputString[8] - 48) %10;
      r2  = (inputString[10] - 48) %10 *100;
      r2 += (inputString[11] - 48) %10 *10;
      r2 += (inputString[12] - 48) %10;
      r1  = (inputString[14] - 48) %10 *100;
      r1 += (inputString[15] - 48) %10 *10;
      r1 += (inputString[16] - 48) %10;
      d  = (inputString[17] - 48) %10 *100;
      d += (inputString[18] - 48) %10 *10;
      d += (inputString[19] - 48) %10;
      if(inputString[1] == '-')  l1 = -l1;
      if(inputString[5] == '-')  l2 = -l2;
      if(inputString[9] == '-')  r2 = -r2;
      if(inputString[13] == '-') r1 = -r1;
      motor(l1,l2,r2,r1);
      if(d != 0)  {
        d /= 3;
        cnt = 0;
        while(cnt != d){
          while(analogRead(A0) > 200);
          cnt++;
          if(cnt >= d) break;
          while(analogRead(A0) <= 200);
          cnt++;
        }
        motor(0,0,0,0);
      }
      Serial.print('N');
    }
    inputString = "";
    stringComplete = false;
  }
}
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
void motor(int ml1, int ml2, int mr2, int mr1){
  if(ml1 > 255)   ml1 = 255;
  if(ml2 > 255)   ml2 = 255;
  if(mr1 > 255)   mr1 = 255;
  if(mr2 > 255)   mr2 = 255;
  if(ml1 < -255)   ml1 = -255;
  if(ml2 < -255)   ml2 = -255;
  if(mr1 < -255)   mr1 = -255;
  if(mr2 < -255)   mr2 = -255;
  // ml1
  if(ml1 > 0){
    digitalWrite(8, 0);
    analogWrite(9, ml1);
  }else{
    digitalWrite(8, 1);
    analogWrite(9, 255 + ml1);
  }
  // ml2
  if(ml2 > 0){
    digitalWrite(7, 0);
    analogWrite(6, ml2);
      
  }else{
    digitalWrite(7, 1);
    analogWrite(6, 255 + ml2);
  }
  // mr2
  if(mr2 > 0){
    digitalWrite(4, 0);
    analogWrite(5, mr2);
  }else{
    digitalWrite(4, 1);
    analogWrite(5, 255 + mr2);
  }
  // mr1
  if(mr1 > 0){
    digitalWrite(2, 0);
    analogWrite(3, mr1);
  }else{
    digitalWrite(2, 1);
    analogWrite(3, 255 + mr1);
  }
  
}
