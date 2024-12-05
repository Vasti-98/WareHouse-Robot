#include <stdint.h>
#include <Encoder.h>
#include <RSLK_Pins.h> 
#include <Romi_Motor_Power.h>
/** DOWNLOAD Energia -RSLK-LIBRARY" **/

// Define motor control pins
#define LEFT_MOTOR_PWM_PIN 40 // Energia pin 40
#define RIGHT_MOTOR_PWM_PIN 39 // Energia pin 39
#define LEFT_MOTOR_DIR_PIN 29 // Energia pin 29
#define RIGHT_MOTOR_DIR_PIN 30 // Energia pin 30
#define Rsleep 11
#define Lsleep 31

Romi_Motor_Power left_m;
Romi_Motor_Power right_m;

// Robot Specifications
const float wheelDiameter = 7.0;       // inches
const uint32_t ticksPerRev = 360;      // ticks per wheel revolution
const float trackWidth = 14.5;         // inches (distance between the two wheels)
// Derived Constants
const float wheelCircumference = PI * wheelDiameter; // inches
const float distancePerTick = wheelCircumference / ticksPerRev; // inches/tick

void straightNoFeed() {
  left_m.directionForward();
  right_m.directionForward();
}





void straightFeedback(uint32_t tickR, uint32_t tickL){
    left_m.enableMotor();
    right_m.enableMotor();
  
  int32_t diff = ((int32_t) tickL - (int32_t)tickR)/2;  //positive 

  const float Kp = 0.04; 
  float correct = Kp * diff;
  float baseSpeed = 150; 
  const float deadband = 2.0;     // Deadband threshold to ignore minor errors

  float leftSpeed = baseSpeed - correct;
  float rightSpeed = baseSpeed + correct;

  //no correction is applied to prevent constant minior adjustments 
  if (abs(diff) < deadband) {
    leftSpeed = baseSpeed;
    rightSpeed = baseSpeed;
  
  } 
  // Ensure motor speeds are within valid range
  leftSpeed = constrain(leftSpeed, 0, 255);   // Assuming motor speed range is 0-255
  rightSpeed = constrain(rightSpeed, 0, 255);

  //debugg
  Serial.print("Diff: ");
  Serial.print(diff);
  Serial.print(" | Correction: ");
  Serial.print(correct);
  Serial.print(" | Left Speed: ");
  Serial.print(leftSpeed);
  Serial.print(" | Right Speed: ");
  Serial.println(rightSpeed);
  //setting the speeds
  left_m.setSpeed(leftSpeed);
  right_m.setSpeed(rightSpeed);
}


void turnR() {
  left_m.setSpeed(150);
  right_m.setSpeed(0);
}

void turnL() {
  left_m.setSpeed(0);
  right_m.setSpeed(150);
}

// Function to calculate the number of ticks for a desired turn angle
float calculateTurnAngle(uint32_t ticksLeft, uint32_t ticksRight, uint32_t desiredAngleDegrees) {
    float deltaTheta = desiredAngleDegrees * (PI / 180.0); // radians
    
    // Calculate the differential distance
    float deltaDistance = deltaTheta * trackWidth; // inches
    
    // Distance each wheel needs to move
    float distanceLeft = -deltaDistance / 2.0;  // inches (backward)
    float distanceRight = deltaDistance / 2.0;  // inches (forward)
    
    // Convert distances to encoder ticks
    ticksLeft = distanceLeft / distancePerTick;   // Negative for backward
    ticksRight = distanceRight / distancePerTick; // Positive for forward
}

void testEncoder() {
  // Keep running the motors
  Serial.println("Motors running...");
  delay(1000); // Print status every second

  //straightFeedback(getEncoderRightCnt(),getEncoderLeftCnt());
  turnL();
  Serial.println("Left");
  Serial.println(getEncoderLeftCnt());
  Serial.println("Right");
  Serial.println(getEncoderRightCnt());

  resetLeftEncoderCnt(); 
  resetRightEncoderCnt();
  delay (1000);
 
  
}
