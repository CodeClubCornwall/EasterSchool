/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int ledRed = 2;
int ledYellow = 4;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(ledred, OUTPUT);
  pinMode(ledyellow, OUTPUT);  
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(ledred, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(ledyellow, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(500);               // wait for a second
  digitalWrite(ledred, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(ledyellow, HIGH);    // turn the LED off by making the voltage LOW
  delay(500);               // wait for a second
}
