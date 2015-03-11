
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

#define Compass_Address 0x1E
#define MPU_Address 0x68 // I2C address of the MPU-6050

#define MAX_DISTANCE 200

#include <Wire.h>
#include <Servo.h>
#include <NewPing.h>

char keypress=' ';
volatile int rpmLeft=0;
volatile int rpmRight=0;

Servo myservo;  // create servo object to control a servo 
NewPing sonarFwd(Ultrasonic_Fwd_Trigger, Ultrasonic_Fwd_Echo, MAX_DISTANCE);
NewPing sonarDown(Ultrasonic_Down_Trigger, Ultrasonic_Down_Echo, MAX_DISTANCE);

void setup() {
  setI2C(); 
//  myservo.attach(Servo_Pin);  // Only enable if you require a servo as this kills the H Bridge PWM enable and you can only use speed 0 or 255
  setPullUps();
  brakesOn(); 
  Serial.begin(115200);
  displayMenu();
  Serial.flush();
}

void loop() {
  checkForKeyPress();
  doTest();
}

void displayMenu() {
  Serial.println("");
  Serial.println("Test Menu");
  Serial.println("");
  Serial.println("1. I2C Bus Scan");
  Serial.println("2. Gyro & ACC Test");
  Serial.println("3. Compass Test");
  Serial.println("4. Servo Test - Not working as disables motor enable PWM");
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
  Serial.println("SPACE to exit test and return to menu");
  Serial.println("");
  keypress='z'; 
}

void checkForKeyPress() {
  if(Serial.available()>0) {
    keypress=Serial.read();
    Serial.flush();
  }
} 

void doTest() {
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
    case 'z':
      break;
    default:
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
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);
  Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);
  delay(250); 
}

void servoTest() {
/*  int pos = 0;
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } */ 
}

void sonicForwardTest() {
  delay(250);
  int uS = sonarFwd.ping();
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM);
  Serial.println("cm");  
}

void sonicDownTest() {
  delay(250);
  int uS = sonarDown.ping();
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM);
  Serial.println("cm"); 
}

void LDRTest() {
  delay(250);
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

void right(int speed) {
  digitalWrite(Left_Motor_IN1,HIGH);
  digitalWrite(Left_Motor_IN2,LOW);
  digitalWrite(Right_Motor_IN3,LOW);
  digitalWrite(Right_Motor_IN4,HIGH);
  analogWrite(Left_Motor_Enable,speed);
  analogWrite(Right_Motor_Enable,speed);  
}

void brakesOn() {
  digitalWrite(Left_Motor_IN1,LOW);
  digitalWrite(Left_Motor_IN2,LOW);
  digitalWrite(Right_Motor_IN3,LOW);
  digitalWrite(Right_Motor_IN4,LOW);
  delay(200);
  analogWrite(Left_Motor_Enable,0);  // equivilent to digitalWrite(Left_Motor_Enable, LOW)
  analogWrite(Right_Motor_Enable,0); // equivilent to digitalWrite(Right_Motor_Enable, LOW)
}

void RPMTest() {
  attachInterrupt(0, leftMotorInterrupt, RISING);
  attachInterrupt(1, rightMotorInterrupt, RISING);
  rpmLeft=0;
  rpmRight=0;
  forward(100); 
  delay(1000);
  Serial.print("Left RPM : ");
  Serial.print(rpmLeft*30);
  Serial.print("  Right RPM : ");
  Serial.println(rpmRight*30);
  detachInterrupt(0);
  detachInterrupt(1);  
}

void leftMotorInterrupt() {
 rpmLeft++; 
}

void rightMotorInterrupt() {
 rpmRight++; 
}

void setI2C() {
  Wire.begin();
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(Compass_Address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
  //Put the MPU6050 IC into the correct operating mode
  Wire.beginTransmission(MPU_Address);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission();
}

void setPullUps() {
  digitalWrite(Light_Sensor_Left_PUP,HIGH);
  digitalWrite(Light_Sensor_Middle_PUP,HIGH);
  digitalWrite(Light_Sensor_Right_PUP,HIGH);
  digitalWrite(Left_Motor_Feedback, HIGH);  // Enable pull up resistors for left  motor sensor input
  digitalWrite(Right_Motor_Feedback, HIGH);  // Enable pull up resistors for right motor sensor input
}
