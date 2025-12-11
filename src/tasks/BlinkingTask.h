#ifndef __BLINKINGTASK__
#define __BLINKINGTASK__

#include "Task.h"
#include "devices/LedPwm.h"

class BlinkingTask : public Task {
public:
  BlinkingTask();
  virtual void init(int period, LedPwm* led);
  virtual void tick();
  virtual void setActive(bool active);

private:
  enum State { TURNING_ON, TURNING_OFF};
  State state;
  LedPwm* led;
};


#endif