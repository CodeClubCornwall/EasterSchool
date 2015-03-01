/*
  Robot Awesome
  
  Makes awesome happen
 */
const float REVS_PER_METRE = 518;

#define leftMotorFeedback 2
#define leftMotorEnable 9
#define leftMotorIN1 8
#define leftMotorIN2 7

#define rightMotorFeedback 3
#define rightMotorEnable 10
#define rightMotorIN4 11
#define rightMotorIN3 12

boolean interruptStopped = false; 
boolean shouldMove = false;

int runs = 1;

int revolutions = 0;
int revolutionsLeft = 0;
int revolutionsRight = 0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(leftMotorIN1, OUTPUT);
  pinMode(leftMotorIN2, OUTPUT);
  pinMode(rightMotorIN3, OUTPUT);
  pinMode(rightMotorIN4, OUTPUT);
  pinMode(leftMotorEnable, OUTPUT);
  pinMode(rightMotorEnable, OUTPUT);
  digitalWrite(rightMotorFeedback, HIGH);
  digitalWrite(leftMotorFeedback, HIGH);
  attachInterrupt(0, processLeftMotorInterrupt, RISING);
  attachInterrupt(1, processRightMotorInterrupt, RISING);

}

// the loop routine runs over and over again forever:
void loop() {
  resetAll();
  runs = 4;
  while (runs > 0) {
    driveForward_cms(100);    
    turnHardLeft(9);
    runs--;
  }
  turnHardLeft(36);
}

void processLeftMotorInterrupt() {
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
void processRightMotorInterrupt() {
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

void driveForward_cms(float cm){
  
  revolutions = (REVS_PER_METRE * (cm / 100) *2);
    
  leftForward();
  rightForward();
  shouldMove = true;
  startBoth();
  
  while (revolutions > 0) {

  }  
  
  shouldMove = false;
  resetAll();
}

void driveBackward_seconds(int time){
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
  digitalWrite(rightMotorIN4, HIGH);
  digitalWrite(rightMotorIN3, LOW);
}
void rightBack() {
  digitalWrite(rightMotorIN3, HIGH);
  digitalWrite(rightMotorIN4, LOW);
}
void stopRight() {
  digitalWrite(rightMotorEnable, LOW);
}
void startRight() {
  digitalWrite(rightMotorEnable, HIGH);
}

void leftForward() {
  digitalWrite(leftMotorIN1, HIGH);
  digitalWrite(leftMotorIN2, LOW);
}
void leftBack() {
  digitalWrite(leftMotorIN2, HIGH);
  digitalWrite(leftMotorIN1, LOW);
}
void stopLeft() {
  digitalWrite(leftMotorEnable, LOW);
}
void startLeft() {
  digitalWrite(leftMotorEnable, HIGH);
}
void resetAll() {
  stopBoth();
  digitalWrite(leftMotorIN1, LOW);
  digitalWrite(leftMotorIN2, LOW);
  digitalWrite(rightMotorIN3, LOW);
  digitalWrite(rightMotorIN4, LOW);
}

void stopBoth() {
   stopLeft();
   stopRight();
}
void startBoth() {
  startLeft();
  startRight();
}



