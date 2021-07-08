#ifndef ENCODER_H
#define ENCODER_H

/*
 * Encoder wiring:
 * Green:   Quad encoder ground
 * Blue:    Quad encoder +5V
 * Yellow:  Quad encoder A signal
 * White:   Quad encoder B signal
 * 
 * A signal: 
 *  is the primary output pulse for determining the amount of rotation. 
 *  Each time the knob is rotated by one detent (click) in either direction, 
 *  the ‘CLK’ output goes through one cycle of going HIGH and then LOW.
 * 
 * B signal:
 *  is the same as the CLK output, but it lags the CLK by a 90° phase shift. 
 *  This output can be used to determine the direction of rotation.
 */

 //Global interrupt service routine
ICACHE_RAM_ATTR void updateEncoder()
{
  //TODO
};

class Encoder
{
  public:

    //wire configuration for the encoder
    typedef struct Config
    {
      const int pin_quad_A;
      const int pin_quad_B;

      Config(const int pin_quad_A,
             const int pin_quad_B): pin_quad_A(pin_quad_A), 
                                    pin_quad_B(pin_quad_B) {}
      
    } Config;
  
    Encoder(const int pin_quad_A, 
            const int pin_quad_B): config(pin_quad_A, pin_quad_B)
    {
      pinMode(this->config.pin_quad_A, INPUT);
      pinMode(this->config.pin_quad_B, INPUT);
      attachInterrupt(digitalPinToInterrupt(this->config.pin_quad_A), updateEncoder, CHANGE);
      //attachInterrupt(digitalPinToInterrupt(this->config.pin_quad_B), updateEncoder, CHANGE);
    }

  private:
    const int PPR = 11;     //11 pulses per revolution 
    Encoder::Config config;
};

#endif
