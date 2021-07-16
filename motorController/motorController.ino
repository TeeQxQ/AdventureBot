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

Motor motorLeft(PIN_REAR_DIR_1, PIN_REAR_DIR_2, PIN_PWM_REAR, Motor::Direction::CLOCKWISE);
Motor motorRight(PIN_FRONT_DIR_1, PIN_FRONT_DIR_2, PIN_PWM_FRONT, Motor::Direction::CLOCKWISE);

#ifdef USE_ENCODER
const int PIN_REAR_ENCODER_A = 0;
const int PIN_REAR_ENCODER_B = 0;
const int PIN_FRONT_ENCODER_A = 0;
const int PIN_FRONT_ENCODER_B = 0;

Encoder encoderRear(PIN_REAR_ENCODER_A, PIN_REAR_ENCODER_B);
Encoder encoderFront(PIN_FRONT_ENCODER_A, PIN_FRONT_ENCODER_B);
#endif


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

  //Serial.println(WiFi.macAddress());

  //IP based on mac address
  if (WiFi.macAddress() == MAC_LEFT)
  {
    ip = IPAddress(192, 168, 4, 3);
    motorLeft.setDefaultDirection(Motor::Direction::CLOCKWISE);
    motorRight.setDefaultDirection(Motor::Direction::CLOCKWISE);
  }
  else if (WiFi.macAddress() == MAC_RIGHT)
  {
    ip = IPAddress(192, 168, 4, 4);
    motorLeft.setDefaultDirection(Motor::Direction::CLOCKWISE);
    motorRight.setDefaultDirection(Motor::Direction::CLOCKWISE);
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

  motorLeft.stop();
  motorRight.stop();
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
      int btnLeftUp = udpPacket[1];
      int btnLeftDown = udpPacket[2];
      int btnRightUp = udpPacket[3];
      int btnRightDown = udpPacket[4];

      motorLeft.changeSpeed(speed);
      motorRight.changeSpeed(speed);

      //Left
      if (btnLeftUp && btnLeftDown)
      {
        motorLeft.stop();
      }
      else if (!btnLeftUp && btnLeftDown)
      {
        motorLeft.forward();
        motorLeft.run();
      }
      else if (btnLeftUp && !btnLeftDown)
      {
        motorLeft.reverse();
        motorLeft.run();
      }

      //Right
      if (btnRightUp && btnRightDown)
      {
        motorRight.stop();
      }
      else if (!btnRightUp && btnRightDown)
      {
        motorRight.forward();
        motorRight.run();
      }
      else if (btnRightUp && !btnRightDown)
      {
        motorRight.reverse();
        motorRight.run();
      }
      
      /*if (btnUp && btnDown)
      {
        motorLeft.stop();
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
      }*/
      
#ifdef DEBUG
      Serial.print("Speed: ");
      Serial.print(speed);
      Serial.print(", Btn left up: ");
      Serial.print(btnLeftUp);
      Serial.print(", Btn left down: ");
      Serial.print(btnLeftDown);
      Serial.print(", Btn right up: ");
      Serial.print(btnRightUp);
      Serial.print(", Btn right down: ");
      Serial.print(btnRightDown);
      Serial.println();
#endif
    }
  }
#endif

}
