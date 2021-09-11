#include <PS4Controller.h>
#include "motor.h"

//Hardware parameters
const int PIN_PWM_LEFT_REAR = 32;         
const int PIN_PWM_LEFT_FRONT = 14;       
const int PIN_PWM_RIGHT_REAR = 5;         
const int PIN_PWM_RIGHT_FRONT = 4;       

const int PIN_LEFT_REAR_DIR_1 = 33;       
const int PIN_LEFT_REAR_DIR_2 = 25;       
const int PIN_LEFT_FRONT_DIR_1 = 27;      
const int PIN_LEFT_FRONT_DIR_2 = 26;     

const int PIN_RIGHT_REAR_DIR_1 = 18;       
const int PIN_RIGHT_REAR_DIR_2 = 19;       
const int PIN_RIGHT_FRONT_DIR_1 = 16;      
const int PIN_RIGHT_FRONT_DIR_2 = 17;

Motor motorLeftFront(PIN_LEFT_FRONT_DIR_1, PIN_LEFT_FRONT_DIR_2, PIN_PWM_LEFT_FRONT, 0, Motor::Direction::COUNTERCLOCKWISE);
Motor motorLeftRear(PIN_LEFT_REAR_DIR_1, PIN_LEFT_REAR_DIR_2, PIN_PWM_LEFT_REAR, 1, Motor::Direction::CLOCKWISE);

Motor motorRightFront(PIN_RIGHT_FRONT_DIR_1, PIN_RIGHT_FRONT_DIR_2, PIN_PWM_RIGHT_FRONT, 2, Motor::Direction::CLOCKWISE);
Motor motorRightRear(PIN_RIGHT_REAR_DIR_1, PIN_RIGHT_REAR_DIR_2, PIN_PWM_RIGHT_REAR, 3, Motor::Direction::COUNTERCLOCKWISE);

char* ps4_MAC = "03:03:03:03:03:03";

void setup() {
  Serial.begin(115200);
  
  PS4.begin(ps4_MAC);
  Serial.println("Ready to connect...");
  while(!PS4.isConnected())
  {
    delay(100);
  }
  Serial.println("Controller connected!");

  motorLeftFront.stop();
  motorLeftRear.stop();
  motorRightFront.stop();
  motorRightRear.stop();
}

void loop() {
  if (PS4.isConnected())
  {
    if (PS4.L2()) {
      int leftSpeed = map(PS4.L2Value(), 0, 255, 80, 100);
      //Serial.println(leftSpeed);

      if(PS4.L1())
      {
        motorLeftFront.reverse();
        motorLeftRear.reverse();
      }
      else
      {
        motorLeftFront.forward();
        motorLeftRear.forward();
      }
      
      motorLeftFront.changeSpeed(leftSpeed);
      motorLeftRear.changeSpeed(leftSpeed);
      motorLeftFront.run();
      motorLeftRear.run();
    }
    else
    {
      motorLeftFront.stop();
      motorLeftRear.stop();   
    }
    
    if (PS4.R2()) {
      int rightSpeed = map(PS4.R2Value(), 0, 255, 80, 100);
      //Serial.println(rightSpeed);

      if(PS4.R1())
      {
        motorRightFront.reverse();
        motorRightRear.reverse();
      }
      else
      {
        motorRightFront.forward();
        motorRightRear.forward();
      }
      
      motorRightFront.changeSpeed(rightSpeed);
      motorRightRear.changeSpeed(rightSpeed);
      motorRightFront.run();
      motorRightRear.run();
    }
    else
    {
      motorRightFront.stop();
      motorRightRear.stop();
    }
  }    

}
