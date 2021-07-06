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
          const int pin_pwm,
          const Motor::Direction defaultDirection) : config(pin_direction_in1, pin_direction_in2, pin_pwm),
                                                    defaultDirection(defaultDirection)
    {
      pinMode(this->config.pin_pwm, OUTPUT);
      pinMode(this->config.pin_direction_in1, OUTPUT);
      pinMode(this->config.pin_direction_in2, OUTPUT);

      //Set current direction to default
      this->resetDirection();

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
      if (this->direction == Motor::Direction::COUNTERCLOCKWISE)
      {
        digitalWrite(this->config.pin_direction_in1, LOW);
        digitalWrite(this->config.pin_direction_in2, HIGH);
      }
      else
      {
        digitalWrite(this->config.pin_direction_in1, HIGH);
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

    void resetDirection()
    {
      this->direction = this->defaultDirection;
    }

    void changeDirection()
    {
      if (this->direction == Motor::Direction::CLOCKWISE)
      {
        this->direction = Motor::Direction::COUNTERCLOCKWISE;
      }
      else
      {
        this->direction = Motor::Direction::CLOCKWISE;
      }
    }

    void reverse()
    {
      //this->direction = Motor::Direction::COUNTERCLOCKWISE;
      if (this->defaultDirection == Motor::Direction::CLOCKWISE)
      {
        this->direction = Motor::Direction::COUNTERCLOCKWISE;
      }
      else
      {
        this->direction = Motor::Direction::CLOCKWISE;
      }
    }

    void forward()
    {
      //this->direction = Motor::Direction::CLOCKWISE;
      this->direction = this->defaultDirection;
    }

    void setDefaultDirection(Motor::Direction newDefaultDirection)
    {
      this->defaultDirection = newDefaultDirection;
    }
    

  private:
    const int MAX_SPEED = 100;
    const int MIN_SPEED = 0;
    const int MAX_PWM_SPEED = PWMRANGE;
    const int MIN_PWM_SPEED = 0;
    Motor::Config config;
    Motor::Direction defaultDirection;
    Motor::Direction direction = Motor::Direction::CLOCKWISE;
    int speed = 0;

    int convertSpeedToPWM()
    {
      return map(this->speed, MIN_SPEED, MAX_SPEED, MIN_PWM_SPEED, MAX_PWM_SPEED);
    }
    
 };

 class MotorGroup
 {
  public:
    MotorGroup()
    {
      
    }

    //Target speed for the motors in the group
    void changeSpeed()
    {
      //
    }

    void reverse()
    {
      //
    }

    void forward()
    {
      //
    }
  private:
    //
 };

#endif
