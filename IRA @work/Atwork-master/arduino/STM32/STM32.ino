String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
long int cnt = 0;
void setup() {
  Serial.begin(115200);
  pinMode(PA15, OUTPUT);
  pinMode(PB3, OUTPUT);
  pinMode(PB4, OUTPUT);
  pinMode(PB5, OUTPUT);
  pinMode(PB6, OUTPUT);
  pinMode(PB7, OUTPUT);
  pinMode(PB8, OUTPUT);
  pinMode(PB9, OUTPUT);
  pinMode(PA11, OUTPUT);
  
//  motor(0,0,0,0);
  inputString.reserve(200);
}

void loop() {
  digitalWrite(PA15, 0);
  digitalWrite(PB3, 0);
  digitalWrite(PB4, 0);
  digitalWrite(PB5, 0);
  digitalWrite(PB6, 0);
  digitalWrite(PB7, 0);
  digitalWrite(PB8, 0);
  digitalWrite(PB9, 0);
  
  
//  motor(50,50,50,50);
//  int l1, l2, r2, r1,d;
//  if (stringComplete) {
//    if(inputString[0] == 'M'){
//      l1  = (inputString[2] - 48) %10 *100;
//      l1 += (inputString[3] - 48) %10 *10;
//      l1 += (inputString[4] - 48) %10;
//      l2  = (inputString[6] - 48) %10 *100;
//      l2 += (inputString[7] - 48) %10 *10;
//      l2 += (inputString[8] - 48) %10;
//      r2  = (inputString[10] - 48) %10 *100;
//      r2 += (inputString[11] - 48) %10 *10;
//      r2 += (inputString[12] - 48) %10;
//      r1  = (inputString[14] - 48) %10 *100;
//      r1 += (inputString[15] - 48) %10 *10;
//      r1 += (inputString[16] - 48) %10;
//      d  = (inputString[17] - 48) %10 *100;
//      d += (inputString[18] - 48) %10 *10;
//      d += (inputString[19] - 48) %10;
//      if(inputString[1] == '-')  l1 = -l1;
//      if(inputString[5] == '-')  l2 = -l2;
//      if(inputString[9] == '-')  r2 = -r2;
//      if(inputString[13] == '-') r1 = -r1;
//      motor(l1,l2,r2,r1);
//      if(d != 0)  {
//        d /= 3;
//        cnt = 0;
//        while(cnt != d){
//          while(analogRead(A0) > 200);
//          cnt++;
//          if(cnt >= d) break;
//          while(analogRead(A0) <= 200);
//          cnt++;
//        }
//        motor(0,0,0,0);
//      }
//      Serial.print('N');
//    }
//    inputString = "";
//    stringComplete = false;
//  }
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
  ml1 *= 65535/255;
  ml2 *= 65535/255;
  mr1 *= 65535/255;
  mr2 *= 65535/255;
  
  // ml1
  if(ml1 > 0){
    digitalWrite(PA15, 0);
    pwmWrite(PB6, ml1);
  }else{
    digitalWrite(PA15, 1);
    pwmWrite(PB6, 255 + ml1);
  }
//  // ml2
//  if(ml2 > 0){
//    digitalWrite(PB3, 0);
//    pwmWrite(PB7, ml2);
//      
//  }else{
//    digitalWrite(PB3, 1);
//    pwmWrite(PB7, 255 + ml2);
//  }
//  // mr2
//  if(mr2 > 0){
//    digitalWrite(PB4, 0);
//    pwmWrite(PB8, mr2);
//  }else{
//    digitalWrite(PB4, 1);
//    pwmWrite(PB8, 255 + mr2);
//  }
//  // mr1
//  if(mr1 > 0){
//    digitalWrite(PB5, 0);
//    pwmWrite(PB9, mr1);
//  }else{
//    digitalWrite(PB5, 1);
//    pwmWrite(PB9, 255 + mr1);
//  }
//  
}
