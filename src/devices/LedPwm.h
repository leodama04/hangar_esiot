#ifndef __LED_PWM__
#define __LED_PWM__

#include "Led.h"

class LedPwm: public Led { 
public:
  LedPwm(int pin);
  LedPwm(int pin, int intensity);
  void switchOn();
  void switchOff();
  void setIntensity(int intensity);
  int getIntensity();

private:
  bool isOn;
  int currentIntensity;

};

#endif