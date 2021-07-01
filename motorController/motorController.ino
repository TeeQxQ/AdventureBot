#include "ESP8266WiFi.h"
#include "encoder.h"
#include "messages.h"
#include "motor.h"

#define DEBUG
#define USE_WIFI
#define USE_CONTROLLER

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

//Wifi related variables:
static WiFiClient wifiController;
const char* controllerAddress = "192.168.4.1";
const int controllerPort = 80;

#endif


//Function to initialize connection with remote controller
void connectController()
{
  if (!wifiController.connected())
  {
    if (!wifiController.connect(controllerAddress, controllerPort))
    {
#ifdef DEBUG
      Serial.println("Unable to connect controller");
#endif
      
      return;
    }
#ifdef DEBUG
    Serial.println("Connected to controller:");
    Serial.print("- ip: ");
    Serial.println(controllerAddress);
    Serial.print("- port: ");
    Serial.println(controllerPort);
#endif
  }
}

//Function to read new commands from the controller
void listenControllerCommands()
{
  //Verify connection still exist
  if(wifiController.connected())
  {
    //
  }
}

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

#ifdef USE_CONTROLLER
  connectController();
#endif


  if (wifiController && wifiController.available())
  {
    msg::err = deserializeJson(msg::msg, wifiController);
    if (msg::err)
    {
#ifdef DEBUG
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(msg::err.f_str());
#endif
    }
    else
    {
      int s = msg::msg["speed"];
      Serial.println(s);
      motorRear.changeSpeed(s);
      motorRear.run();
    }
  }
  
  // put your main code here, to run repeatedly:
  /*delay(1000);
  //motorRear.stop();
  motorRear.changeSpeed(50);
  delay(1000);
  motorRear.changeSpeed(100);
  delay(1000);
  motorRear.reverse();
  motorRear.run();
  delay(1000);
  motorRear.forward();
  motorRear.run();*/
  

}
