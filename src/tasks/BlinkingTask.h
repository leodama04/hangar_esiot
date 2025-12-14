#ifndef __BLINKINGTASK__
#define __BLINKINGTASK__

#include "kernel/Task.h"
#include "devices/Led.h"

class BlinkingTask : public Task {
public:
  BlinkingTask();
  virtual void init(int period, Led* led);
  virtual void tick();
  virtual void setActive(bool active);

private:
  enum State { TURNING_ON, TURNING_OFF};
  State state;
  Led* led;
};


#endif