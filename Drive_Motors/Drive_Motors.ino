/*
  Robot Awesome
  
  Makes awesome happen
 */
const float REVS_PER_METRE = 518;

int leftMotorEnable = 5;
int leftMotorFwd = 6;
int leftMotorBkwd = 7;

int rightMotorEnable = 8;
int rightMotorFwd = 9;
int rightMotorBkwd = 10;

boolean interruptStopped = false; 
boolean shouldMove = false;

int runs = 1;

int revolutions = 0;
int revolutionsLeft = 0;
int revolutionsRight = 0;

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
  while (runs > 0) {
//driveForward(100);
    turnHardLeft(14);
    delay(5000);
    turnHardLeft(13);
    delay(5000);
    turnHardLeft(12);
    delay(5000);
    turnHardLeft(11);
    delay(5000);
    turnHardLeft(10);
    delay(5000);
    runs--;
  }
}

void leftMotorInterrupt() {
  revolutions--;
  revolutionsLeft--;
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
  revolutions--;
  revolutionsRight--;
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

void driveForward(float cm){
  
  revolutions = (REVS_PER_METRE * (cm / 100) *2);
  while (revolutions > 0) {
    leftForward();
    rightForward();
    shouldMove = true;
    startBoth();
  }  
  
  shouldMove = false;
  resetAll();
}

void driveBackward(int time){
  leftBack();
  rightBack();
  shouldMove = true;
  startBoth();
  
  delay(time);
  shouldMove = false;
  resetAll();
}

void turnHardLeft(float cm) {
  revolutionsLeft = (REVS_PER_METRE * (cm / 100));
  revolutionsRight = revolutionsLeft;
  while (revolutionsLeft > 0 || revolutionsRight > 0) {
    if (revolutionsLeft > 0) {
      leftBack();
    } else {
      stopLeft();
    }
    if (revolutionsRight > 0) {
      rightForward();
    } else {
      stopRight();
    }
    
    startBoth();
  }
  
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
   stopLeft();
   stopRight();
}
void startBoth() {
  startLeft();
  startRight();
}



