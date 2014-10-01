// TurtleDuino Obstacles Avoiding Robot By:RobDavinci
#include <Servo.h> //include Servo library
#include <TimedAction.h>

const int RForward = 180;
const int RBackward = 120;
const int LForward = RForward;
const int LBackward = RBackward;
const int RNeutral = 90;
const int LNeutral = 90; //constants for motor speed
const int pingPin = 7;
const int irPin = 0;  //Sharp infrared sensor pin
const int dangerThresh = 10; //threshold for obstacles (in cm)
int leftDistance, rightDistance; //distances on either side
Servo panMotor; 
Servo leftMotor;
Servo rightMotor; //declare motors
long duration; //time it takes to recieve PING))) signal
TimedAction danceTimer = TimedAction(60000, dance);
char dancing = 0;
const int danceInterval = 5;  // minutes between dances

void setup()
{
  rightMotor.attach(11);
  leftMotor.attach(10);
  panMotor.attach(6); //attach motors to proper pins
  panMotor.write(90); //set PING))) pan to center
  Serial.begin(9600);
}

void loop()
{
  int distanceFwd = ping();
  //Serial.print("Ping returned");
  //Serial.print(distanceFwd);
  //Serial.println();
  
  danceTimer.check();
  
  if (distanceFwd>dangerThresh && !dancing) //if path is clear
  {
    //Serial.println("Moving forward");
    moveForward();
  }
  else if (!dancing) //if path is blocked
  {
    //Serial.println("Path blocked. Scanning...");
    stopMotors();
    panMotor.write(0);
    delay(500);
    rightDistance = ping(); //scan to the right
    delay(500);
    panMotor.write(180);
    delay(700);
    leftDistance = ping(); //scan to the left
    delay(500);
    panMotor.write(90); //return to center
    delay(100);
    compareDistance();
  }
}
 
void compareDistance()
{
  if (leftDistance>rightDistance) //if left is less obstructed
  {
    turnLeft();
    delay(2000);
  }
  else if (rightDistance>leftDistance) //if right is less obstructed
  {
    turnRight();
    delay(2000);
  }
   else //if they are equally obstructed
  {
    turnRight(); //turn 180 degrees
    delay(2000);
  }
}

long ping()
{
  // Send out PING))) signal pulse
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
 
  //Get duration it takes to receive echo
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
 
  //Convert duration into distance
  return duration / 29 / 2;
}

void turnLeft() {
  leftMotor.write(LBackward);
  rightMotor.write(RForward);
}

void turnRight() {
  leftMotor.write(LForward);
  rightMotor.write(RBackward);
}

void moveForward() {
  leftMotor.write(LForward);
  rightMotor.write(RForward);
}

void stopMotors() {
  leftMotor.write(LNeutral);
  rightMotor.write(RNeutral);
}

void dance() {
  static int minCount = 1;
	
  minCount++;
  if (minCount == danceInterval) {
    dancing = 1;

    Serial.println("d1b");

    // do the dance
    for (int i = 0; i < 4; i++) {
      turnRight();
      delay(2000);
	  moveForward();
	  delay(2000);
	  stopMotors();
	  delay(2000);
	
	  for (int j = 0; j < 4; j++) {
	    turnLeft();
        delay(2000);
	    moveForward();
	    delay(2000);
	    stopMotors();
	    delay(2000);
      }	
	}

    // reset flags
    Serial.println("d1e");
    dancing = 0;
    minCount = 1;
  }
}