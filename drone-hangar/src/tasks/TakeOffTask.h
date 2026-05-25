#ifndef __TAKEOFFTASK__
#define __TAKEOFFTASK__

#include "kernel/Task.h"

class TakeOffTask : public Task {
public:
  TakeOffTask();
  virtual void init(int period);
  virtual void tick();

private:
  enum State { WAITING_MSG, WAITING_DOOR_TO_OPEN, WAITING_DRONE_TO_EXIT, WAITING_TO_CLOSE_DOOR, WAITING_DOOR_TO_CLOSE};
  State state;
  unsigned long tStart;
  bool timerRunning;
};

#endif
