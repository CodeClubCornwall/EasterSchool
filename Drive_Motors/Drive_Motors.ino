/*
  Robot Awesome
  
  Makes awesome happen
 */
int leftMotorEnable = 5;
int leftMotorFwd = 6;
int leftMotorBkwd = 7;

int rightMotorEnable = 8;
int rightMotorFwd = 9;
int rightMotorBkwd = 10;

boolean interruptStopped = false; 
boolean shouldMove = false;

int i = 0;
int runs = 20;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(leftMotorFwd, OUTPUT);
  pinMode(rightMotorFwd, OUTPUT);
  pinMode(leftMotorBkwd, OUTPUT);
  pinMode(rightMotorBkwd, OUTPUT);
  pinMode(leftMotorEnable, OUTPUT);
  pinMode(rightMotorEnable, OUTPUT);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  attachInterrupt(0, leftMotorInterrupt, RISING);
  attachInterrupt(1, rightMotorInterrupt, RISING);

}

// the loop routine runs over and over again forever:
void loop() {
  resetAll();
  while (i<runs) {
    driveForward(1000);
    i++;
  }
}

void leftMotorInterrupt() {
  if (!shouldMove) {
    return;
  }
  if (interruptStopped) {
    digitalWrite(leftMotorEnable, HIGH);
  } else {
    digitalWrite(rightMotorEnable, LOW);
  }
  interruptStopped = !interruptStopped;
}
void rightMotorInterrupt() {
  if (!shouldMove) {
    return;
  }
  if (interruptStopped) {
    digitalWrite(rightMotorEnable, HIGH);
  } else {
    digitalWrite(leftMotorEnable, LOW);
  }
  interruptStopped = !interruptStopped;
}

void driveForward(int time){
  leftForward();
  rightForward();
  startBoth();
  
  delay(time);
  resetAll();
}

void driveBackward(int time){
  leftBack();
  rightBack();
  startBoth();
  
  delay(time);
  resetAll();
}

void turnHardLeft90() {
  leftBack();
  rightForward();

  startBoth();
  delay(1250);
  resetAll();
}
void turnHardRight90() {
  leftForward();
  rightBack();

  startBoth();
  delay(1250);
  resetAll();
}

void rightForward() {
  digitalWrite(rightMotorFwd, HIGH);
  digitalWrite(rightMotorBkwd, LOW);
}
void rightBack() {
  digitalWrite(rightMotorBkwd, HIGH);
  digitalWrite(rightMotorFwd, LOW);
}
void stopRight() {
  digitalWrite(rightMotorEnable, LOW);
}
void startRight() {
  digitalWrite(rightMotorEnable, HIGH);
}

void leftForward() {
  digitalWrite(leftMotorFwd, HIGH);
  digitalWrite(leftMotorBkwd, LOW);
}
void leftBack() {
  digitalWrite(leftMotorBkwd, HIGH);
  digitalWrite(leftMotorFwd, LOW);
}
void stopLeft() {
  digitalWrite(leftMotorEnable, LOW);
}
void startLeft() {
  digitalWrite(leftMotorEnable, HIGH);
}
void resetAll() {
  stopBoth();
  digitalWrite(leftMotorFwd, LOW);
  digitalWrite(leftMotorBkwd, LOW);
  digitalWrite(rightMotorFwd, LOW);
  digitalWrite(rightMotorBkwd, LOW);
}

void stopBoth() {
   shouldMove = false;
   stopLeft();
   stopRight();
}
void startBoth() {
  shouldMove = true;
  startLeft();
  startRight();
}



