
// ******************************************************
//
// Ensure the board is set to Arduino Nano with ATmega328
//
// ******************************************************

#define Left_Motor_Feedback 2
#define Right_Motor_Feedback 3
#define Ultrasonic_Fwd_Trigger 4
#define Ultrasonic_Fwd_Echo 5
#define Servo_Pin 6
#define Left_Motor_IN2 7
#define Left_Motor_IN1 8
#define Left_Motor_Enable 9
#define Right_Motor_Enable 10
#define Right_Motor_IN4 11
#define Right_Motor_IN3 12
#define Ultrasonic_Down_Trigger 13
#define Light_Sensor_Left_PUP 14
#define Light_Sensor_Middle_PUP 15
#define Light_Sensor_Right_PUP 16
#define Ultrasonic_Down_Echo 17

#define Light_Sensor_Left 0
#define Light_Sensor_Middle 1
#define Light_Sensor_Right 2

#define Compass_Address 0x1E  // I2C address of the compass IC (HMC5883)
#define MPU_Address 0x68 // I2C address of the MPU-6050

#define MAX_TEST_DISTANCE 200 // maximum ultrasonic sensor distance in test 5 & 6
#define MAX_DISTANCE 15 // For Story 10 (chassis 1)
#define MAX_DROP 15 // For Story 11 (chassis 1)
#define REVS_PER_METRE 518 // 518 hall switch clicks per meter
#define LIGHT_TRIGGER_VAL 200  // For Story 12

//  Constants to calibrate based on:
//  1. The floor surface
//  2. The battery used
//  3. The speed used for PWM
//  4. the robot chassis

#define NINETY_DEG_TURN 75 // number of hall sensor clicks in 90 deg
#define PWM_SPEED 200 // use a number between 100 and 255

#include <Wire.h>
#include <Servo.h>
#include <NewPing.h>

char keypress=' ';

volatile unsigned int rpmLeftCounter=0;    // max = 65535 = 126.515 meters
volatile unsigned int rpmRightCounter=0; 

void setup() {
  Wire.begin();
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(Compass_Address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
  Wire.beginTransmission(MPU_Address);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission();

  digitalWrite(Light_Sensor_Left_PUP,HIGH);
  digitalWrite(Light_Sensor_Middle_PUP,HIGH);
  digitalWrite(Light_Sensor_Right_PUP,HIGH);
  digitalWrite(Left_Motor_Feedback, HIGH);  // Enable pull up resistors for left  motor sensor input
  digitalWrite(Right_Motor_Feedback, HIGH);  // Enable pull up resistors for right motor sensor input
  brakesOn(); 
  Serial.begin(115200);
  displayMenu();
  Serial.flush();
}

void loop() {
  checkForKeyPress();
}

void displayMenu() {
  Serial.println("");
  Serial.println("Test Menu");
  Serial.println("");
  Serial.println("1. I2C Bus Scan");
  Serial.println("2. Gyro & ACC Test");
  Serial.println("3. Compass Test");
  Serial.println("4. Servo Test - Disables motor enable PWM");
  Serial.println("5. Ultrasonic Forward Test");
  Serial.println("6. Ultrasonic Down Test");
  Serial.println("7. Light Detector Test");
  Serial.println("8. Motor RPM Test"); 
  Serial.println("");
  Serial.println("f. Slow forward   F. Fast forward");
  Serial.println("b. Slow reverse   B. Fast reverse");
  Serial.println("l. Slow left      L. Fast left");
  Serial.println("r. Slow right     R. Fast right");
  Serial.println("");  
  Serial.println("S. Story 4        T. Story 5");
  Serial.println("U. Story 6        V. Story 7"); 
  Serial.println("W. Story 10       X. Story 11");
  Serial.println("Y. Story 12       Z. Story 13");
  Serial.println("");
  Serial.println("SPACE to exit test and return to menu");
  Serial.println("");
  keypress='#';
}

void checkForKeyPress() {
  if(Serial.available()>0) {
    keypress=Serial.read();
    Serial.flush();
  }
  switch(keypress) {
  case '1':
    I2CBusScan();
    break;
  case '2':
    MPU6050Test();
    break;
  case '3':
    compassTest();
    break;
  case '4':
    servoTest();
    break;
  case '5':
    sonicForwardTest();
    break;
  case '6':
    sonicDownTest();
    break;
  case '7':
    LDRTest();
    break;
  case '8':
    RPMTest();
    break;
  case 'f':
    forward(100);
    break;
  case 'F':
    forward(255);
    break;
  case 'b':
    reverse(100);
    break;
  case 'B':
    reverse(255);
    break;
  case 'l':
    left(100);
    break;
  case 'L':
    left(255);
    break;
  case 'r':
    right(100);
    break;
  case 'R':
    right(255);
    break;
  case 's':
  case 'S':
    story4();  
    keypress='#';
    break;
  case 't':
  case 'T':
    story5();
    keypress='#';
    break;
  case 'u':
  case 'U':
    story6();
    keypress='#';
    break; 
  case 'v':
  case 'V':
    story7();
    keypress='#';
    break; 
  case 'w':
  case 'W':
    story10();
    keypress='#';
    break;
  case 'x':
  case 'X':
    story11();
    keypress='#';
    break; 
  case 'y':
  case 'Y':
    story12();
    keypress='#';
    break; 
  case 'z':
  case 'Z':
    story13();
    keypress='#';
    break;     
  case ' ':
  case 'q':
  case 'Q':
    brakesOn();
    displayMenu();
  }
}  

void I2CBusScan() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("Scan complete\n"); 
  } 
  delay(2000);
}

void compassTest() {

  int x,y,z; //triple axis data

  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(Compass_Address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();


  //Read data from each axis, 2 registers per axis
  Wire.requestFrom(Compass_Address, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }

  //Print out values of each axis
  Serial.print("x: ");
  Serial.print(x);
  Serial.print("  y: ");
  Serial.print(y);
  Serial.print("  z: ");
  Serial.println(z);

  delay(250);
}

void MPU6050Test() {
  int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
  Wire.beginTransmission(MPU_Address);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_Address,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Serial.print("AcX = "); 
  Serial.print(AcX);
  Serial.print(" | AcY = "); 
  Serial.print(AcY);
  Serial.print(" | AcZ = "); 
  Serial.print(AcZ);
  Serial.print(" | Tmp = "); 
  Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  Serial.print(" | GyX = "); 
  Serial.print(GyX);
  Serial.print(" | GyY = "); 
  Serial.print(GyY);
  Serial.print(" | GyZ = "); 
  Serial.println(GyZ);
  delay(250); 
}

void servoTest() {
   Servo myservo;  // create servo object to control a servo
   myservo.attach(Servo_Pin);  // Disables interrupts preventing PWM motor enable control
   int pos = 0;
   for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
   {                                  // in steps of 1 degree 
   myservo.write(pos);              // tell servo to go to position in variable 'pos' 
   delay(15);                       // waits 15ms for the servo to reach the position 
   } 
   for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
   {                                
   myservo.write(pos);              // tell servo to go to position in variable 'pos' 
   delay(15);                       // waits 15ms for the servo to reach the position 
   }
   myservo.detach();  // Doesn't free up interrupts to allow PWM motor enable control
}

void sonicForwardTest() {
  NewPing sonarFwd(Ultrasonic_Fwd_Trigger, Ultrasonic_Fwd_Echo, MAX_TEST_DISTANCE);
  delay(50);
  int uS = sonarFwd.ping();
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM);
  Serial.println("cm");  
}

void sonicDownTest() {
  NewPing sonarDown(Ultrasonic_Down_Trigger, Ultrasonic_Down_Echo, MAX_TEST_DISTANCE);
  delay(50);
  int uS = sonarDown.ping();
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM);
  Serial.println("cm"); 
}

void LDRTest() {
  Serial.print("Left: ");
  Serial.print(analogRead(Light_Sensor_Left));  
  Serial.print("  Middle: ");
  Serial.print(analogRead(Light_Sensor_Middle));  
  Serial.print("  Right: ");
  Serial.println(analogRead(Light_Sensor_Right)); 
}

void forward(int speed) {
  digitalWrite(Left_Motor_IN1,HIGH);
  digitalWrite(Left_Motor_IN2,LOW);
  digitalWrite(Right_Motor_IN3,HIGH);
  digitalWrite(Right_Motor_IN4,LOW);
  analogWrite(Left_Motor_Enable,speed);
  analogWrite(Right_Motor_Enable,speed);  
}

void forwardMeters(int speed, float meters) {
  setupRPMSensors();
  forward(speed);
  while(rpmLeftCounter <  meters * REVS_PER_METRE) {
    goStraight(speed);
  }
  brakesOn(); 
  detachInterrupt(0);
  detachInterrupt(1); 
}

void reverseMeters(int speed, float meters) {
  setupRPMSensors();
  reverse(speed);
  while(rpmLeftCounter <  meters * REVS_PER_METRE) {
    goStraight(speed);
  }
  brakesOn(); 
  detachInterrupt(0);
  detachInterrupt(1); 
}

void goStraight(int speed) {
  if(rpmLeftCounter == rpmRightCounter) {
    analogWrite(Left_Motor_Enable,speed); 
    analogWrite(Right_Motor_Enable,speed); 
  }
  if(rpmLeftCounter < rpmRightCounter) {
    analogWrite(Right_Motor_Enable,0);
    analogWrite(Left_Motor_Enable,speed);  
  }
  if(rpmLeftCounter > rpmRightCounter) {
    analogWrite(Left_Motor_Enable,0); 
    analogWrite(Right_Motor_Enable,speed);
  }
}



void reverse(int speed) {
  digitalWrite(Left_Motor_IN1,LOW);
  digitalWrite(Left_Motor_IN2,HIGH);
  digitalWrite(Right_Motor_IN3,LOW);
  digitalWrite(Right_Motor_IN4,HIGH);
  analogWrite(Left_Motor_Enable,speed);
  analogWrite(Right_Motor_Enable,speed);  
}

void left(int speed) {
  digitalWrite(Left_Motor_IN1,LOW);
  digitalWrite(Left_Motor_IN2,HIGH);
  digitalWrite(Right_Motor_IN3,HIGH);
  digitalWrite(Right_Motor_IN4,LOW);
  analogWrite(Left_Motor_Enable,speed);
  analogWrite(Right_Motor_Enable,speed);   
}

void leftTurnClyde(int speed, int clicks) {
  attachInterrupt(0, leftMotorInterrupt, RISING);
  rpmLeftCounter=0;
  while(rpmLeftCounter < clicks) {
    left(speed); 
  }
  brakesOn();
  detachInterrupt(0);     
}


void right(int speed) {
  digitalWrite(Left_Motor_IN1,HIGH);
  digitalWrite(Left_Motor_IN2,LOW);
  digitalWrite(Right_Motor_IN3,LOW);
  digitalWrite(Right_Motor_IN4,HIGH);
  analogWrite(Left_Motor_Enable,speed);
  analogWrite(Right_Motor_Enable,speed);  
}

void rightTurnClyde(int speed, int clicks) {
  attachInterrupt(1, rightMotorInterrupt, RISING);
  rpmRightCounter=0;
  while(rpmRightCounter < clicks) {
    right(speed); 
  }
  brakesOn();
  detachInterrupt(1);     
}

void brakesOn() {
  digitalWrite(Left_Motor_IN1,LOW);
  digitalWrite(Right_Motor_IN3,LOW);
  digitalWrite(Left_Motor_IN2,LOW);
  digitalWrite(Right_Motor_IN4,LOW);
  analogWrite(Left_Motor_Enable,255);
  analogWrite(Right_Motor_Enable,255); 
  delay(1000);
  analogWrite(Left_Motor_Enable,0);  // equivilent to digitalWrite(Left_Motor_Enable, LOW)
  analogWrite(Right_Motor_Enable,0); // equivilent to digitalWrite(Right_Motor_Enable, LOW)
}

void RPMTest() {
  setupRPMSensors();
  forward(100); 
  delay(1000);
  Serial.print("Left RPM : ");
  Serial.print(rpmLeftCounter*30);  // only multiply by 30 as there are two clicks per rotation
  Serial.print("  Right RPM : ");
  Serial.println(rpmRightCounter*30);  // only multiply by 30 as there are two clicks per rotation
  detachInterrupt(0);
  detachInterrupt(1);  
}

void leftMotorInterrupt() {  // in and out as quickly as possible :-)
  rpmLeftCounter++; 
}

void rightMotorInterrupt() {  // in and out as quickly as possible :-)
  rpmRightCounter++; 
}

void story4() {  // As a robot I want to move forward for 1 seconds, pause for 1 second and reverse for 1 seconds so that I can display basic motor control
  delay(1000);
  forward(PWM_SPEED);
  delay(1000);
  brakesOn();
  reverse(PWM_SPEED);
  delay(1000);
  brakesOn(); 
}

void story5() {  // As a robot I want to drive forward for 10 seconds in a straight line so that I can display accurate motor control
  delay(1000);
  forward(PWM_SPEED);
  delay(10000);
  brakesOn();    
}

void story6() {  // As a robot I want to drive forward 2 meters and stop so that I can display accurate distance measurement
  delay(1000);
  forwardMeters(PWM_SPEED, 2);   
}

void story7() {  // As a robot I want to be able to move forward creating a square shape so that I can display accurate turning features
  for (int i=4; i>0; i--) {
    forwardMeters(PWM_SPEED, 0.5);
    leftTurnClyde(PWM_SPEED, NINETY_DEG_TURN);
  }

}

void story10() {  // As a robot I want to stop before I hit an obstable so that I do not get damaged
  int i;
  NewPing sonarFwd(Ultrasonic_Fwd_Trigger, Ultrasonic_Fwd_Echo, MAX_DISTANCE);
  setupRPMSensors();
  forward(PWM_SPEED);
  do {
    goStraight(PWM_SPEED);
    i = sonarFwd.ping();
  } while (i / US_ROUNDTRIP_CM == 0);
  brakesOn();
  detachInterrupt(0);
  detachInterrupt(1); 
  leftTurnClyde(PWM_SPEED, NINETY_DEG_TURN);  
}

void story11() {  // As a robot I want to stop if there is no ground within 5cm of the front of the robot
  int i;
  NewPing sonarDown(Ultrasonic_Down_Trigger, Ultrasonic_Down_Echo, MAX_DROP);
  setupRPMSensors();
  forward(PWM_SPEED);
  do {
    goStraight(PWM_SPEED/1.5);
    i = sonarDown.ping();
  } while (i / US_ROUNDTRIP_CM < 15 && i / US_ROUNDTRIP_CM != 0);
  detachInterrupt(0);
  detachInterrupt(1);  
  reverseMeters(PWM_SPEED/1.5, 0.1);  // You really do need to "jam it in reverse"
  brakesOn();  
}

void story12() {  // As a robot I want to follow a homing beacon so that I can find my way to safety at the end of a mission
  int leftLDR;
  int middleLDR;
  int rightLDR;
  int i;
  int light=0;

  NewPing sonarFwd(Ultrasonic_Fwd_Trigger, Ultrasonic_Fwd_Echo, MAX_DISTANCE-5);

  do {
    leftLDR = analogRead(Light_Sensor_Left);
    middleLDR = analogRead(Light_Sensor_Middle);
    rightLDR = analogRead(Light_Sensor_Right);
   
    if(rightLDR < LIGHT_TRIGGER_VAL) {light = light + 1;}
    if(middleLDR < LIGHT_TRIGGER_VAL) {light = light + 2;}
    if(leftLDR < LIGHT_TRIGGER_VAL) {light = light + 4;}
  
    switch(light) {
      case 4:
      case 6:
        left(PWM_SPEED/2);
        break;
      case 2:
        forward(PWM_SPEED/2);
        break;
      case 7:  // left in but shouldn't happen
      case 5:
        forward(PWM_SPEED/2);
        break;
      case 0:
      case 1:
      case 3:
        right(PWM_SPEED/2);
    }
    light = 0;
    i = sonarFwd.ping();
  } while (i / US_ROUNDTRIP_CM == 0);

  brakesOn(); 
}

void story13() {  // As a robot I want to follow a homing beacon across hostile terrain, safely avoiding obstacles and steep drops whilst continuing to follow the beacon so that I can find my way safely back at the end of a mission
  Serial.println("");
  Serial.println("Please come back later");
  Serial.println("");  
}

void setupRPMSensors() {
  attachInterrupt(0, leftMotorInterrupt, RISING);
  attachInterrupt(1, rightMotorInterrupt, RISING);
  rpmLeftCounter=0;
  rpmRightCounter=0;
}

