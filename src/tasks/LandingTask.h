#ifndef __LANDINGTASK__
#define __LANDINGTASK__

#include "kernel/Task.h"

class LandingTask : public Task {
public:
  LandingTask();
  virtual void init(int period);
  virtual void tick();

private:
  enum State { WAITING_MSG, DETECTING_DRONE, WAITING_DOOR_TO_OPEN, WAITING_DRONE_TO_ENTER,
    WAITING_TO_CLOSE_DOOR, WAITING_DOOR_TO_CLOSE };
  State state;
  unsigned long tStart;
  bool timerRunning;
};


#endif