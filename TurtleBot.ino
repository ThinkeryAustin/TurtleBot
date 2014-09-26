// TurtleDuino Obstacles Avoiding Robot By:RobDavinci
#include <Servo.h> //include Servo library
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
  Serial.print("Ping returned");
  Serial.print(distanceFwd);
  Serial.println();
  
  if (distanceFwd>dangerThresh) //if path is clear
  {
    Serial.println("Moving forward");
    leftMotor.write(LForward);
    rightMotor.write(RForward); //move forward
  }
  else //if path is blocked
  {
    Serial.println("Path blocked. Scanning...");
    leftMotor.write(LNeutral);
    rightMotor.write(RNeutral);
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
    leftMotor.write(LBackward);
    rightMotor.write(RForward); //turn left
    delay(2000);
  }
  else if (rightDistance>leftDistance) //if right is less obstructed
  {
    leftMotor.write(LForward);
    rightMotor.write(RBackward); //turn right
    delay(2000);
  }
   else //if they are equally obstructed
  {
    leftMotor.write(LForward);
    rightMotor.write(RBackward); //turn 180 degrees
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

