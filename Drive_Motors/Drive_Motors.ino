/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int leftMotorFwd = 5;
int rightMotorFwd = 4;
int leftMotorBkwd = 7;
int rightMotorBkwd = 6;
int i = 0;
int runs = 4;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(leftMotorFwd, OUTPUT);
  pinMode(rightMotorFwd, OUTPUT);  
    pinMode(leftMotorBkwd, OUTPUT);
  pinMode(rightMotorBkwd, OUTPUT);  

}

// the loop routine runs over and over again forever:
void loop() {
  while (i<runs) {
    driveForward();
    turnHardLeft90();               // wait for a second    
    i++;
  }
}

void driveForward(){
  digitalWrite(leftMotorFwd, HIGH);   // turn the left Motor On (HIGH is the voltage level)   
  digitalWrite(rightMotorFwd, HIGH);   // turn the right Motor On (HIGH is the voltage level)  
  digitalWrite(leftMotorBkwd, LOW);   // turn the left Motor On (HIGH is the voltage level)   
  digitalWrite(rightMotorBkwd, LOW);   // turn the right Motor On (HIGH is the voltage level)  
  delay(3000);
  stopMotors();
}

void driveBackward(){
  digitalWrite(leftMotorBkwd, HIGH);   // turn the left Motor On (HIGH is the voltage level)   
  digitalWrite(rightMotorBkwd, HIGH);   // turn the right Motor On (HIGH is the voltage level)  
  digitalWrite(leftMotorFwd, LOW);   // turn the left Motor On (HIGH is the voltage level)   
  digitalWrite(rightMotorFwd, LOW);   // turn the right Motor On (HIGH is the voltage level)  

}

void turnLeft90(){
  digitalWrite(leftMotorFwd, LOW);   // turn the left Motor On (HIGH is the voltage level)   
  digitalWrite(rightMotorFwd, HIGH);   // turn the right Motor On (HIGH is the voltage level)   
  digitalWrite(leftMotorBkwd, LOW);   // turn the left Motor On (HIGH is the voltage level)   
  digitalWrite(rightMotorBkwd, LOW);   // turn the right Motor On (HIGH is the voltage level) 
  delay(2000);
  stopMotors(); 

}

void turnHardLeft90(){
  digitalWrite(leftMotorFwd, LOW);   // turn the left Motor On (HIGH is the voltage level)   
  digitalWrite(rightMotorFwd, HIGH);   // turn the right Motor On (HIGH is the voltage level)   
  digitalWrite(leftMotorBkwd, HIGH);   // turn the left Motor On (HIGH is the voltage level)   
  digitalWrite(rightMotorBkwd, LOW);   // turn the right Motor On (HIGH is the voltage level)  

  delay(1250);
  stopMotors();
}

void stopMotors(){
   digitalWrite(leftMotorFwd, LOW);   // turn the left Motor On (HIGH is the voltage level)   
   digitalWrite(rightMotorFwd, LOW);   // turn the right Motor On (HIGH is the voltage level)   
   digitalWrite(leftMotorBkwd, LOW);   // turn the left Motor On (HIGH is the voltage level)   
   digitalWrite(rightMotorBkwd, LOW);   // turn the right Motor On (HIGH is the voltage level)   

}


