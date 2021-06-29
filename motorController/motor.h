#ifndef MOTOR_H
#define MOTOR_H

/*
 * Motor wiring:
 * Red:   Motor power (+)
 * Black: Motor power (-)
 */

 class Motor
 {
  public:

    //wire configuration for the motor
    typedef struct Config
    {
      const int pin_direction_in1;
      const int pin_direction_in2;
      const int pin_pwm;

      Config(const int pin_direction_in1,
             const int pin_direction_in2,
             const int pin_pwm): pin_direction_in1(pin_direction_in1), 
                                 pin_direction_in2(pin_direction_in2), 
                                 pin_pwm(pin_pwm) {}
      
    } Config;

    enum Direction
    {
      COUNTERCLOCKWISE = 0,
      CLOCKWISE = 1
    };

    //Constructor
    Motor(const int pin_direction_in1,
          const int pin_direction_in2,
          const int pin_pwm) : config(pin_direction_in1, pin_direction_in2, pin_pwm)
    {
      pinMode(this->config.pin_pwm, OUTPUT);
      pinMode(this->config.pin_direction_in1, OUTPUT);
      pinMode(this->config.pin_direction_in2, OUTPUT);

      //By default, stop the motor
      this->stop();

    }

    //Stops motor spinning
    void stop()
    {
      digitalWrite(this->config.pin_direction_in1, LOW);
      digitalWrite(this->config.pin_direction_in2, LOW);
      analogWrite(this->config.pin_pwm, this->MIN_PWM_SPEED);
    }

    //Starts the motor
    void run()
    {
      if (this->direction == Motor::Direction::CLOCKWISE)
      {
        digitalWrite(this->config.pin_direction_in1, LOW);
        digitalWrite(this->config.pin_direction_in2, LOW);
      }
      else
      {
        digitalWrite(this->config.pin_direction_in1, LOW);
        digitalWrite(this->config.pin_direction_in2, LOW);
      }

      this->changeSpeed();
    }

    void changeSpeed(int newSpeed)
    {
      if (newSpeed > this->MAX_SPEED)
      {
        this->speed = this->MAX_SPEED;
      }
      else if (newSpeed < this->MIN_SPEED)
      {
        this->speed = this->MIN_SPEED;
      }
      else
      {
        this->speed = newSpeed;
      }

      this->changeSpeed();
    }

    void changeSpeed()
    {
      analogWrite(this->config.pin_pwm, this->convertSpeedToPWM());
    }
    

  private:
    const int MAX_SPEED = 100;
    const int MIN_SPEED = 0;
    const int MAX_PWM_SPEED = 255;
    const int MIN_PWM_SPEED = 0;
    const Motor::Config config;
    Motor::Direction direction = Motor::Direction::CLOCKWISE;
    int speed = 0;

    int convertSpeedToPWM()
    {
      return map(this->speed, MIN_SPEED, MAX_SPEED, MIN_PWM_SPEED, MAX_PWM_SPEED);
    }
    
 };

#endif
