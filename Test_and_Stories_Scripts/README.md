Test Menu

1. I2C Bus Scan
2. Gyro & ACC Test
3. Compass Test
4. Servo Test - Not working as disables motor enable PWM
5. Ultrasonic Forward Test
6. Ultrasonic Down Test
7. Light Detector Test
8. Motor RPM Test

f. Slow forward   F. Fast forward
b. Slow reverse   B. Fast reverse
l. Slow left      L. Fast left
r. Slow right     R. Fast right

S. Story 4        T. Story 5
U. Story 6        V. Story 7
W. Story 10       X. Story 11
Y. Story 12       Z. Story 13

SPACE to exit test and return to menu


Story 4:
	As a robot I want to move forward for 1 seconds, pause for 1 second and reverse for 1 seconds so that I can display basic motor control
Story 4 Acceptance Criteria:
	Given the code is deployed when the reset button is pressed the robot moves forwards for 1 seconds, stops for 1 second and reverses for 1 seconds repeatedly.


Story 5:
	As a robot I want to drive forward for 10 seconds in a straight line so that I can display accurate motor control
Story 5 Acceptance Criteria:
	Given the code is deployed when the reset button is pressed the robot moves forwards for 10 seconds and stops and the robot has not veered more that 2cm from a straight line and the robot remains stopped

Story 6:
	As a robot I want to drive forward 2 meters and stop so that I can display accurate distance measurement
Story 6 Acceptance Criteria:
	Given the code is deployed when the reset button is pressed the robot moves forwards 2 meters and stops and remains stopped.

Story 7:
	As a robot I want to be able to move forward creating a square shape so that I can display accurate turning features
Story 7 Acceptance Criteria:
	Given the code is deployed when the reset button is pressed the robot moves forwards x and turns 90 degrees, repeating until a square is drawn. On completion of the robot stops and remains stopped.


Story 10:
	As a robot I want to stop before I hit an obstable so that I do not get damaged
Story 10 Acceptance Criteria:
	Given the robot is moving forwards when it encounters an obstacle then it must stop, pause and turn 90 degrees left without hitting the obstabcle


Story 11:
	As a robot I want to stop if there is no ground within 5cm of the front of the robot
Story 11 Acceptance Criteria:
	Given the robot is moving forwards when it encounters a drop in terrain of 5cms it is to continue, when it encounters a drop in terrain of 6 cms it is to stop and reverse 10cm, stop and remain stopped.


Story 12:
	As a robot I want to follow a homing beacon so that I can find my way to safety at the end of a mission
Story 12 Acceptance Criteria:
	Given the front of is a 145 degrees to a light source when the reset button is pressed then robot must find a light and drive towards it.


Story 13:
	As a robot I want to follow a homing beacon across hostile terrain, safely avoiding obstacles and steep drops whilst continuing to follow the beacon so that I can find my way safely back at the end of a mission
Story 13 Acceptance Criteria:
	Given the robot has found and is following a light as in S12, it must stop at obstacles and find a way around until it arrives at the light source.
