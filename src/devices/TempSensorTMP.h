#ifndef __TEMP_SENSOR_TMP__
#define __TEMP_SENSOR_TMP__

#include "TempSensor.h"

class TempSensorTMP : public TempSensor{
public:
  TempSensorTMP(int pin);
  virtual float getTemperature();
private:
  int pin;
};


#endif