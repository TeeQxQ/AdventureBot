#include "ESP8266WiFi.h"
#include "encoder.h"
#include "motor.h"

#define DEBUG
#define USE_WIFI

//Hardware parameters
const int PIN_PWM_REAR = 2;         //D4
const int PIN_PWM_FRONT = 14;       //D5
const int PIN_REAR_DIR_1 = 0;       //D3
const int PIN_REAR_DIR_2 = 4;       //D2
const int PIN_FRONT_DIR_1 = 5;      //D1
const int PIN_FRONT_DIR_2 = 12;     //D6
/*const int PIN_REAR_ENCODER_A = 0;
const int PIN_REAR_ENCODER_B = 0;
const int PIN_FRONT_ENCODER_A = 0;
const int PIN_FRONT_ENCODER_B = 0;*/

Motor motorRear(PIN_REAR_DIR_1, PIN_REAR_DIR_2, PIN_PWM_REAR);
Motor motorFront(PIN_FRONT_DIR_1, PIN_FRONT_DIR_2, PIN_PWM_FRONT);

#ifdef USE_WIFI

//Wifi access point (AP) parameters:
const char* AP_ssid = "MotorController";
const char* AP_password = "motor12345";
const int CHANNEL = 3;
const bool HIDE_AP = true;

//Wifi related variables:
static WiFiClient wifiClient;
const char* clientAddress = "192.168.4.1";
const int clientPort = 80;

#endif

void setup() {

#ifdef DEBUG
  //Serial connection for debugging purposes
  Serial.begin(115200);
  delay(100);
#endif

#ifdef USE_WIFI
  //Explicitely define this is a station, not access point
  WiFi.mode(WIFI_STA);

  //Connect to wifi network:
  WiFi.begin(AP_ssid, AP_password);

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
#ifdef DEBUG
    Serial.println("Trying to connect wifi...");
#endif
  }

#ifdef DEBUG
  Serial.println("Wifi Connected:");
  Serial.print("- ip: ");
  Serial.println(WiFi.localIP());
#endif

#endif

  motorRear.changeSpeed(100);
  motorRear.run();

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  //motorRear.stop();
  motorRear.changeSpeed(50);
  delay(1000);
  motorRear.changeSpeed(100);
  delay(1000);
  motorRear.reverse();
  motorRear.run();
  delay(1000);
  motorRear.forward();
  motorRear.run();
  

}
