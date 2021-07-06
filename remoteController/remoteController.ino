#define DEBUG
#define CREATE_AP
#define USE_DISPLAY

#include "ESP8266WiFi.h"
#include "messages.h"
#include "WiFiUdp.h"

#ifdef USE_DISPLAY
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Wire.h"

const int SCREEN_WIDTH = 128; // OLED display width, in pixels
const int SCREEN_HEIGHT = 64; // OLED display height, in pixels
const int SCREEN_WIDTH_HALF = SCREEN_WIDTH / 2;
const int SCREEN_HEIGHT_HALF = SCREEN_HEIGHT / 2;
const int TEXT_SIZE = 4;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#endif


//Hardware parameters
const int PIN_BTN_LEFT_UP = 14;   //D5
const int PIN_BTN_LEFT_DOWN = 12; //D6
const int PIN_BTN_RIGHT_UP = 0;   //D3
const int PIN_BTN_RIGHT_DOWN = 2; //D4
const int PIN_POT = A0;           //Analog input
const int I2C_ADD = 0x3C;

//Wifi access point (AP) parameters:
const char* AP_ssid = "MotorController";
const char* AP_password = "motor12345"; //must be >= 8 chars
#ifdef CREATE_AP
const int CHANNEL = 1;
const bool HIDE_AP = true;
const int MAX_NOF_CLIENTS = 2;
#endif

//Measured values for the potentiometer
const int POT_MIN_VALUE = 1024;
const int POT_MAX_VALUE = 2;

//UDP related parameters
WiFiUDP udpLeft;
WiFiUDP udpRight;
const int UDP_PORT_LEFT = 4210;
const int UDP_PORT_RIGHT = 4210;
const char *IP_LEFT = "192.168.4.3";
const char *IP_RIGHT = "192.168.4.4";
const int UDP_PACKET_SIZE = 255;
char udpPacket[UDP_PACKET_SIZE];
char udpReply[UDP_PACKET_SIZE];

int currentSpeed = 0;

#ifdef CREATE_AP
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

#else
void connectWifi()
{
  WiFi.begin(AP_ssid, AP_password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
#ifdef DEBUG
    Serial.print(".");
#endif
  }
}

#endif

int convertPotentiometerToProcent()
{
  return map(analogRead(PIN_POT), POT_MIN_VALUE, POT_MAX_VALUE, 0, 100);
}

#ifdef USE_DISPLAY
  void drawCenter(int value)
  {
    display.clearDisplay();
    display.setTextSize(TEXT_SIZE);
    display.setTextColor(WHITE);
    const int charW = 24; //Width of a char at TEXT_SIZE == 4
    const int charH = 16; //Height of a char at TEXT_SIZE == 4
    int textLen = 1;
    if (value >= 10) textLen = 2;
    if (value >= 100) textLen = 3;
    display.setCursor(SCREEN_WIDTH_HALF - textLen * charW /2, 
                      SCREEN_HEIGHT_HALF - charW /2);
    display.println(value);
    display.display(); 
  }
#endif

void setup() {

#ifdef DEBUG
  //Serial connection for debugging purposes
  Serial.begin(115200);
  delay(1000);
#endif

#ifdef USE_DISPLAY
  if(!display.begin(SSD1306_SWITCHCAPVCC, I2C_ADD)) { // Address 0x3D for 128x64
#ifdef DEBUG
    Serial.println(F("SSD1306 display allocation failed"));
#endif
    for(;;);
  }
#endif

#ifdef CREATE_AP
  createSoftAP();
#else
  connectWifi();
#endif

  udpLeft.begin(UDP_PORT_LEFT);
  udpRight.begin(UDP_PORT_RIGHT);
#ifdef DEBUG
  Serial.print("Listening on udp ports: ");
  Serial.println(UDP_PORT_LEFT);
  Serial.println(UDP_PORT_RIGHT);
#endif

}

void loop() {

  delay(10);
  int potValue = convertPotentiometerToProcent();

#ifdef USE_DISPLAY
  drawCenter(potValue);
#endif

  udpPacket[0] = potValue;
  udpPacket[1] = digitalRead(PIN_BTN_LEFT_UP);
  udpPacket[2] = digitalRead(PIN_BTN_LEFT_DOWN);
  udpLeft.beginPacket(IP_LEFT, UDP_PORT_LEFT);
  udpLeft.write(udpPacket);
  udpLeft.endPacket();

  udpPacket[1] = digitalRead(PIN_BTN_RIGHT_UP);
  udpPacket[2] = digitalRead(PIN_BTN_RIGHT_DOWN);
  udpRight.beginPacket(IP_RIGHT, UDP_PORT_RIGHT);
  udpRight.write(udpPacket);
  udpRight.endPacket();          

}
