#ifndef __HANGAR_DOOR__
#define __HANGAR_DOOR__

#include "devices/ServoMotor.h"
#include "devices/ServoMotorImpl.h"

enum DoorState { DOOR_OPEN, DOOR_CLOSED, DOOR_OPENING, DOOR_CLOSING };

class HangarDoor {
public:
  HangarDoor();
  // initialize by providing servo pin; HangarDoor will create the ServoMotorImpl
  void init(int servoPin, int closedAngle = 0, int openAngle = 90, int step = 5);

  // request operations (non-blocking)
  void open();
  void close();

  // call periodically (e.g. from a task) to advance servo and state
  void update();

  DoorState getState() const;
  int getPosition() const;
  bool isOpen() const;
  bool isClosed() const;

private:
  ServoMotor* servo;
  ServoMotorImpl* servoImpl; // owned implementation created internally
  int closedAngle;
  int openAngle;
  int currentAngle;
  int step;
  DoorState state;
  void applyPosition();
};

extern HangarDoor hangarDoor;

#endif
