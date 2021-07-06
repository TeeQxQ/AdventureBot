#include "ESP8266WiFi.h"
#include "encoder.h"
#include "messages.h"
#include "motor.h"
#include "WiFiUdp.h"

#define DEBUG
#define USE_WIFI
#define USE_CONTROLLER
//#define USE_ENCODER

//Hardware parameters
const int PIN_PWM_REAR = 2;         //D4
const int PIN_PWM_FRONT = 14;       //D5
const int PIN_REAR_DIR_1 = 0;       //D3
const int PIN_REAR_DIR_2 = 4;       //D2
const int PIN_FRONT_DIR_1 = 5;      //D1
const int PIN_FRONT_DIR_2 = 12;     //D6

#ifdef USE_ENCODER
const int PIN_REAR_ENCODER_A = 0;
const int PIN_REAR_ENCODER_B = 0;
const int PIN_FRONT_ENCODER_A = 0;
const int PIN_FRONT_ENCODER_B = 0;
#endif

Motor motorRear(PIN_REAR_DIR_1, PIN_REAR_DIR_2, PIN_PWM_REAR, Motor::Direction::CLOCKWISE);
Motor motorFront(PIN_FRONT_DIR_1, PIN_FRONT_DIR_2, PIN_PWM_FRONT, Motor::Direction::CLOCKWISE);

#ifdef USE_WIFI
//Wifi access point (AP) parameters:
const char* AP_ssid = "MotorController";
const char* AP_password = "motor12345";
const char* MAC_LEFT = "9C:9C:1F:45:88:6E";
const char* MAC_RIGHT = "9C:9C:1F:45:BC:8F";
IPAddress ip(192, 168, 4, 2);
IPAddress dns(192, 168, 1, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
#endif

//UDP related parameters
WiFiUDP udp;
const int UDP_PORT = 4210;
const int UDP_PACKET_SIZE = 255;
char udpPacket[UDP_PACKET_SIZE];
char udpReply[UDP_PACKET_SIZE];

void setup() {

#ifdef DEBUG
  //Serial connection for debugging purposes
  Serial.begin(115200);
  delay(1000);
#endif

#ifdef USE_WIFI
  //Explicitely define this is a station, not access point
  WiFi.mode(WIFI_STA);

  Serial.println(WiFi.macAddress());

  //IP based on mac address
  if (WiFi.macAddress() == MAC_LEFT)
  {
    ip = IPAddress(192, 168, 4, 3);
    motorRear.setDefaultDirection(Motor::Direction::COUNTERCLOCKWISE);
    motorFront.setDefaultDirection(Motor::Direction::COUNTERCLOCKWISE);
  }
  else if (WiFi.macAddress() == MAC_RIGHT)
  {
    ip = IPAddress(192, 168, 4, 4);
    motorRear.setDefaultDirection(Motor::Direction::CLOCKWISE);
    motorFront.setDefaultDirection(Motor::Direction::CLOCKWISE);
    Serial.println("Terve");
  }

  //Set static ip address (based on mac)
  WiFi.config(ip, dns, gateway, subnet);

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

  udp.begin(UDP_PORT);

#endif

  motorRear.stop();
  motorFront.stop();
}

void loop() {

#ifdef USE_WIFI
  if(udp.parsePacket())
  {
    int len = udp.read(udpPacket, UDP_PACKET_SIZE);
    if(len > 0)
    {
      udpPacket[len] = 0;
      int speed = udpPacket[0];
      int btnUp = udpPacket[1];
      int btnDown = udpPacket[2];

      motorRear.changeSpeed(speed);
      motorFront.changeSpeed(speed);
      
      if (btnUp && btnDown)
      {
        motorRear.stop();
        motorFront.stop();
      }
      else if (!btnUp && btnDown)
      {
        motorRear.forward();
        motorFront.forward();
        motorRear.run();
        motorFront.run();
      }
      else if (!btnDown && btnUp)
      {
        motorRear.reverse();
        motorFront.reverse();
        motorRear.run();
        motorFront.run();
      }
      else
      {
        //Intentionally empty
      }
      
#ifdef DEBUG
      Serial.print("Speed: ");
      Serial.print(speed);
      Serial.print(", Btn up: ");
      Serial.print(btnUp);
      Serial.print(", Btn down: ");
      Serial.print(btnDown);
      Serial.println();
#endif
    }
  }
#endif

}
