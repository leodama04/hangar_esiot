#include "HangarDoor.h"
#include "Arduino.h"

HangarDoor::HangarDoor(){
  servo = nullptr;
  servoImpl = nullptr;
  closedAngle = 0;
  openAngle = 90;
  currentAngle = 0;
  step = 5;
  state = DOOR_CLOSED;
}

void HangarDoor::init(int servoPin, int closedAngle_, int openAngle_, int step_){
  if (servoImpl) {
    delete servoImpl;
    servoImpl = nullptr;
  }
  servoImpl = new ServoMotorImpl(servoPin);
  this->servo = servoImpl;
  this->closedAngle = closedAngle_;
  this->openAngle = openAngle_;
  this->step = step_ > 0 ? step_ : 1;
  this->currentAngle = this->closedAngle;
  if (this->servo){
    this->servo->on();
    this->applyPosition();
  }
  state = DOOR_CLOSED;
}

void HangarDoor::open(){
  if (state == DOOR_OPEN || state == DOOR_OPENING) return;
  state = DOOR_OPENING;
}

void HangarDoor::close(){
  if (state == DOOR_CLOSED || state == DOOR_CLOSING) return;
  state = DOOR_CLOSING;
}

void HangarDoor::update(){
  if (!servo) return;

  if (state == DOOR_OPENING){
    if (currentAngle < openAngle){
      currentAngle += step;
      if (currentAngle > openAngle) currentAngle = openAngle;
      applyPosition();
    }
    if (currentAngle >= openAngle){
      state = DOOR_OPEN;
    }
  } else if (state == DOOR_CLOSING){
    if (currentAngle > closedAngle){
      currentAngle -= step;
      if (currentAngle < closedAngle) currentAngle = closedAngle;
      applyPosition();
    }
    if (currentAngle <= closedAngle){
      state = DOOR_CLOSED;
    }
  }
}

DoorState HangarDoor::getState() const{
  return state;
}

int HangarDoor::getPosition() const{
  return currentAngle;
}

bool HangarDoor::isOpen() const{
  return state == DOOR_OPEN;
}

bool HangarDoor::isClosed() const{
  return state == DOOR_CLOSED;
}

void HangarDoor::applyPosition(){
  servo->setPosition(currentAngle);
}
