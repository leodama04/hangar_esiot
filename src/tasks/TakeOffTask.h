#ifndef __TAKEOFFTASK__
#define __TAKEOFFTASK__

#include "Task.h"

class TakeOffTask : public Task {
public:
  TakeOffTask();
  virtual void init(int period);
  virtual void tick();

private:
  enum State { WAITING_MSG, WAITING_DOOR_TO_OPEN, WAITING_DRONE_TO_EXIT, WAITING_DOOR_TO_CLOSE};
  State state;
};

#endif
