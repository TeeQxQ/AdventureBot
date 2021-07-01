#include "ESP8266WiFi.h"
#include "messages.h"

#define DEBUG

//Hardware parameters
const int PIN_BTN_LEFT_UP = 14;   //D5
const int PIN_BTN_LEFT_DOWN = 12; //D6
const int PIN_BTN_RIGHT_UP = 0;   //D3
const int PIN_BTN_RIGHT_DOWN = 2; //D4
const int PIN_POT = A0;           //Analog input

//Wifi access point (AP) parameters:
const char* AP_ssid = "MotorController";
const char* AP_password = "motor12345"; //must be >= 8 chars
const int CHANNEL = 3;
const bool HIDE_AP = true;
const int MAX_NOF_CLIENTS = 2;

//Measured values for potentiometer
const int POT_MIN_VALUE = 1024;
const int POT_MAX_VALUE = 2;

//Server related parameters:
const int WIFI_PORT = 80;
WiFiServer wifiServer(WIFI_PORT);
WiFiClient *clients[MAX_NOF_CLIENTS] = { NULL };
int nofJoinedClients = 0;

void createSoftAP()
{
  if (!WiFi.softAP(AP_ssid, AP_password, CHANNEL, HIDE_AP, MAX_NOF_CLIENTS))
  {
#ifdef DEBUG
    Serial.println("Failed to create soft access point");
#endif
  }
  else
  {
#ifdef DEBUG
    Serial.println("Soft access point created.");
    Serial.print("Soft access point ssid: ");
    Serial.println(AP_ssid);
    Serial.print("Soft access point password: ");
    Serial.println(AP_password);
    Serial.print("Soft access point IP address: ");
    Serial.println(WiFi.softAPIP());
#endif
  }
}

void checkNewClients()
{
  WiFiClient newClient = wifiServer.available();

  //New client found
  if (newClient)
  {
    clients[nofJoinedClients++] = new WiFiClient(newClient);
#ifdef DEBUG
    Serial.println("New client added");
#endif
  }
}

int convertPotentiometerToProcent()
{
  return map(analogRead(PIN_POT), POT_MIN_VALUE, POT_MAX_VALUE, 0, 100);
}

void setup() {

#ifdef DEBUG
  //Serial connection for debugging purposes
  Serial.begin(115200);
#endif

  createSoftAP();

  //Start the server
  wifiServer.begin();
#ifdef DEBUG
  Serial.println("Server started");
#endif

}

void loop() {

  checkNewClients();

  if(nofJoinedClients > 0)
  {
    //Construct a message
    msg::msg["speed"] = convertPotentiometerToProcent();
    msg::msg["left"] = 1;
    msg::msg["right"] = 0;

    //Serialize the message and send it
    serializeJson(msg::msg, *clients[0]);

    Serial.println(convertPotentiometerToProcent());
  }
}
