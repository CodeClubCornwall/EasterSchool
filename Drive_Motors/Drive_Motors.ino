#include <NewPing.h>

/*
  Robot Awesome
  
  Makes awesome happen
 */
const float REVS_PER_METRE = 518.00;

#define leftMotorFeedback 2
#define leftMotorEnable 9
#define leftMotorIN1 8
#define leftMotorIN2 7

#define rightMotorFeedback 3
#define rightMotorEnable 10
#define rightMotorIN4 11
#define rightMotorIN3 12

#define FORWARD_TRIGGER_PIN  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define FORWARD_ECHO_PIN     5  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define FORWARD_MAX_DISTANCE 20 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.


#define sensorPinLeft A0    // select the input pin for the potentiometer
#define sensorPinRight A2
#define sensorPinMiddle A1

NewPing forwardSonar(FORWARD_TRIGGER_PIN, FORWARD_ECHO_PIN, FORWARD_MAX_DISTANCE); // NewPing setup of pins and maximum distance.

boolean interruptStopped = false; 
boolean shouldMove = false;

int runs = 15;

volatile int revolutions = 0;
volatile int revolutionsLeft = 0;
volatile int revolutionsRight = 0;


int sensorValueLeft = 0;
int sensorValueMiddle = 0;
int sensorValueRight = 0;// variable to store the value coming from the sensor

float leftAdjust;
float rightAdjust;
float middleAdjust;

float leftCalibrated;
float middleCalibrated;
float rightCalibrated;

float leftAdjustPercent;
float middleAdjustPercent;
float rightAdjustPercent;



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
  
  digitalWrite(sensorPinLeft, HIGH);
  digitalWrite(sensorPinMiddle, HIGH);
  digitalWrite(sensorPinRight, HIGH);

  calibrateSensors();

}

// the loop routine runs over and over again forever:
void loop() {
  resetAll();
  
  while (runs > 0) {
    followLight();
    //driveForward_cms(100);    
    //turnHardLeft(8.2);
    runs--;
  }
  
}

void processLeftMotorInterrupt() {
  revolutions--;
  revolutionsLeft--;
    if (!shouldMove) {
    return;
  }
  if (interruptStopped) {
    digitalWrite(rightMotorEnable, HIGH);
    digitalWrite(leftMotorEnable, HIGH); 
  } else {
    digitalWrite(leftMotorEnable, LOW);
    digitalWrite(rightMotorEnable, HIGH);
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
    digitalWrite(leftMotorEnable, HIGH);
    digitalWrite(rightMotorEnable, HIGH);
  } else {
    digitalWrite(rightMotorEnable, LOW);
    digitalWrite(leftMotorEnable, HIGH);
  }
  interruptStopped = !interruptStopped;

}

void followLight()
{
  driveForward_cms(3);
  
  enableBoth();
  while (!facingLight()) {
    if (brighterRight()) {
      turnRight();
    } else {
      turnLeft();
    }
  }
  resetAll();
  
}

boolean facingLight()
{
  sensorValueLeft = analogRead(sensorPinLeft);
  sensorValueMiddle = analogRead(sensorPinMiddle);
  sensorValueRight = analogRead(sensorPinRight);  

  leftCalibrated = sensorValueLeft / leftAdjustPercent;
  middleCalibrated = sensorValueMiddle / middleAdjustPercent;
  rightCalibrated = sensorValueRight / rightAdjustPercent;

  return (middleCalibrated <= leftCalibrated && middleCalibrated <= rightCalibrated);
}

boolean brighterRight()
{
  sensorValueLeft = analogRead(sensorPinLeft);
  sensorValueMiddle = analogRead(sensorPinMiddle);
  sensorValueRight = analogRead(sensorPinRight);  

  leftCalibrated = sensorValueLeft / leftAdjustPercent;
  middleCalibrated = sensorValueMiddle / middleAdjustPercent;
  rightCalibrated = sensorValueRight / rightAdjustPercent;

  float rightCalibratedPlus = rightCalibrated + (rightCalibrated*0.1);

  return (rightCalibratedPlus < leftCalibrated && rightCalibratedPlus < middleCalibrated);
}

void driveForward_cms(float cm){
  
  revolutions = (REVS_PER_METRE * (cm / 100) *2);
    
  leftForward();
  rightForward();
  shouldMove = true;
  enableBoth();
  
  while (revolutions > 0) {
    stopIfObstacleCloserThan_cm(10);
  }  
  
  shouldMove = false;
  resetAll();
}

void stopIfObstacleCloserThan_cm(float cm){
  unsigned int uS = forwardSonar.ping();
  unsigned int distance = uS / US_ROUNDTRIP_CM;
  if(distance>0 && distance<=cm)
  {
    revolutions=0;    
    resetAll();
  }
}

void driveBackward_seconds(int time){
  leftBack();
  rightBack();
  shouldMove = true;
  enableBoth();
  
  delay(time);
  shouldMove = false;
  resetAll();
}

void turnHardLeft(float cm) {
  revolutionsLeft = (REVS_PER_METRE * (cm / 100.00));
  revolutionsRight = revolutionsLeft;
  leftBack();
  rightForward();
  enableBoth();
  
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

  }
  
  resetAll();

}

void turnHardLeft90() {
  leftBack();
  rightForward();

  enableBoth();
  delay(1250);
  resetAll();
}
void turnHardRight90() {
  leftForward();
  rightBack();

  enableBoth();
  delay(1250);
  resetAll();
}
void turnLeft() {
  rightForward();
  leftBack(); 
}
void turnRight() {
  leftForward();
  rightBack(); 
}

void rightBack() {
  digitalWrite(rightMotorIN4, HIGH);
  digitalWrite(rightMotorIN3, LOW);
}
void rightForward() {
  digitalWrite(rightMotorIN3, HIGH);
  digitalWrite(rightMotorIN4, LOW);
}
void stopRight() {
  digitalWrite(rightMotorEnable, LOW);
  digitalWrite(rightMotorIN3, LOW);
  digitalWrite(rightMotorIN4, LOW);
}
void enableRight() {
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
  digitalWrite(leftMotorIN1, LOW);
  digitalWrite(leftMotorIN2, LOW);
}

void enableLeft() {
  digitalWrite(leftMotorEnable, HIGH);
}

void resetAll() {
   stopLeft();
   stopRight();
   delay(1000);
}

void enableBoth() {
  enableLeft();
  enableRight();
}

void calibrateSensors() {
  float leftTotal =0;
  float middleTotal = 0;
  float rightTotal=0; 
  float combinedTotal = 0;
 
  for (int i=20; i--; i>0) {
    leftTotal += analogRead(sensorPinLeft);
    middleTotal += analogRead(sensorPinMiddle);
    rightTotal += analogRead(sensorPinRight);
  }

  leftAdjust = leftTotal/20.0;
  middleAdjust = middleTotal/20.0;
  rightAdjust = rightTotal/20.0;


  combinedTotal = leftTotal + middleTotal + rightTotal;


  leftAdjustPercent = leftTotal / combinedTotal;
  middleAdjustPercent = middleTotal / combinedTotal;
  rightAdjustPercent = rightTotal / combinedTotal;

}



