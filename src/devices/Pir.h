#ifndef __PIR__
#define __PIR__

#include "PresenceSensor.h"

class Pir: public PresenceSensor {
 
public: 
  Pir(int pin);
  bool isDetected();

private:
  void updateSyncTime(long time);
  void calibrate();
  void sync();
  long getLastSyncTime();
  long lastTimeSync;
  int pin;
  bool detected;
  
};

#endif