#include "ESP8266WiFi.h"
#include "encoder.h"
#include "motor.h"

//Hardware parameters
const int PIN_PWM_REAR = 0;
const int PIN_PWM_FRONT = 0;
const int PIN_REAR_DIR_1 = 0;
const int PIN_REAR_DIR_2 = 0;
const int PIN_FRONT_DIR_1 = 0;
const int PIN_FRONT_DIR_2 = 0;
const int PIN_REAR_ENCODER_A = 0;
const int PIN_REAR_ENCODER_B = 0;
const int PIN_FRONT_ENCODER_A = 0;
const int PIN_FRONT_ENCODER_B = 0;

const Motor motorRear(PIN_REAR_DIR_1, PIN_REAR_DIR_2, PIN_PWM_REAR);
const Motor motorFront(PIN_FRONT_DIR_1, PIN_FRONT_DIR_2, PIN_PWM_FRONT);

//Wifi access point (AP) parameters:
const char* AP_ssid = "MotorController";
const char* AP_password = "motor12345";
const int CHANNEL = 3;
const bool HIDE_AP = true;


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
