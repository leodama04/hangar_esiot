#ifndef __ALARMTASK__
#define __ALARMTASK__

#include "Task.h"

class AlarmTask : public Task {
public:
  AlarmTask();
  virtual void init(int period);
  virtual void tick();

private:
  enum State { NORMAL_STATE, WAITING_PRE_ALARM, WAIT_TO_COMPLETE_ACTIONS, 
      PRE_ALARM, WAITING_ALARM, WAITING_DOOR_TO_CLOSE,ALARM };
  State state;
  unsigned long tStart;
  bool timerRunning;
};


#endif