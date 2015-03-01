/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13. 
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead(). 
 
 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground
 
 * Note: because most Arduinos have a built-in LED attached 
 to pin 13 on the board, the LED is optional.
 
 
 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe
 
 This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/AnalogInput
 
 */

int sensorPinLeft = A0;    // select the input pin for the potentiometer
int sensorPinRight = A1;
int sensorPinMiddle = A2;
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

void setup() {
  // declare the ledPin as an OUTPUT:
digitalWrite(sensorPinLeft, HIGH);
digitalWrite(sensorPinMiddle, HIGH);
digitalWrite(sensorPinRight, HIGH);

  Serial.begin(9600);
  calibrateSensors();
}

void loop() {
  
  int winner = 0;
  // read the value from the sensor:
  sensorValueLeft = analogRead(sensorPinLeft);
  sensorValueMiddle = analogRead(sensorPinMiddle);
  sensorValueRight = analogRead(sensorPinRight);  
  // turn the ledPin on
  
  leftCalibrated = (sensorValueLeft) / leftAdjustPercent;//;
  middleCalibrated = (sensorValueMiddle) / middleAdjustPercent;//;
  rightCalibrated = (sensorValueRight) / rightAdjustPercent; //;  

  
  
  Serial.print("  left:  ");
  Serial.print(leftCalibrated);
  Serial.print(" (");
  Serial.print(sensorValueLeft);
  Serial.print(") ");  
  
  Serial.print("  middle:  ");
  Serial.print(middleCalibrated);
  Serial.print(" (");
  Serial.print(sensorValueMiddle);
  Serial.print(") ");  
  
  Serial.print("  right:  ");
  Serial.print(rightCalibrated);
  Serial.print(" (");
  Serial.print(sensorValueRight);
  Serial.print(") ");




/*
  Serial.print("  winner:  ");
  Serial.print();
  Serial.print(" (");
  Serial.print();
  Serial.print(") ");
*/

  Serial.println("");
  
  delay(500);
  
  // stop the program for <sensorValue> milliseconds:
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

