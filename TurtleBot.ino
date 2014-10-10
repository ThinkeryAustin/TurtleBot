// TurtleDuino Obstacles Avoiding Robot By:RobDavinci
#include <Servo.h> //include Servo library
#include <TimedAction.h>

// constants for motor speeds
//
// speed is an integer from 0 to 180
// 0 is full speed backward
// 90 is stopped
// 180 is full speed forward
//
// RForward is right motor forward speed
// LBackward is left motor backward speed
// RNeutral is right motor neutral (stopped) speed
// etc
const int RForward = 180;
const int RBackward = 120;
const int LForward = 0;
const int LBackward = 60;
const int RNeutral = 90;
const int LNeutral = 90;

// declare motors
Servo panMotor; 
Servo leftMotor;
Servo rightMotor;

// declare pins and settings for ping sensor
const int pingPin = 7;
const int dangerThresh = 10; // threshold for obstacles (in cm)
int leftDistance, rightDistance; // distances on either side
long duration; // time it takes to receive PING))) signal

// declare timers and settings for dancing
TimedAction danceTimer = TimedAction(60000, dance);
char dancing = 0;
const int danceInterval = 5;  // minutes between dances
const int danceStepInterval = 2000;  // milliseconds between dance steps

void setup()
{
  // setup the pins use for the servo motors
  rightMotor.attach(11);
  leftMotor.attach(10);
  panMotor.attach(6);
  
  //set PING))) pan to center
  panMotor.write(90);
  
  // set up serial port
  Serial.begin(9600);
  Serial.println("turtle");  // identify the robot to any listening devices
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
  if (leftDistance>rightDistance) // if left is less obstructed
  {
    turnLeft();
    delay(2000);
  }
  else if (rightDistance>leftDistance) // if right is less obstructed
  {
    turnRight();
    delay(2000);
  }
  else // if they are equally obstructed
  {
    turnRight(); // turn 180 degrees
    delay(2000);
  }
}

long ping()
{
  // send out PING))) signal pulse
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
 
  // get duration it takes to receive echo
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
 
  // convert duration into distance
  return duration / 29 / 2;
}

// functions for moving the bot around
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

// function to do dance routine
void dance() {
  static int minCount = 1;

  minCount++;
  if (minCount == danceInterval) {
    dancing = 1;

    Serial.println("d1b");

    // do the dance
    for (int i = 0; i < 4; i++) {
      turnRight();
      delay(danceStepInterval);
      moveForward();
      delay(danceStepInterval);
      stopMotors();
      delay(danceStepInterval);
      
      for (int j = 0; j < 4; j++) {
        turnLeft();
        delay(danceStepInterval);
        moveForward();
        delay(danceStepInterval);
        stopMotors();
        delay(danceStepInterval);
      }	
    }

    // reset flags
    Serial.println("d1e");
    dancing = 0;
    minCount = 1;
  }
}