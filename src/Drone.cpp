#include "Drone.h"
#include "MsgService.h"

// Simple concrete implementation for `Drone` declared in `Drone.h`.
// This file provides method definitions and a global `drone` instance
// so other modules can use the class without further changes.

// Initialize members with sensible defaults
Drone::Drone() {
    requestTakeoff = false;
    requestLanding = false;
    currentState = INSIDE;
}

void Drone::setDroneINSIDE(){
  currentState = INSIDE;
  MsgService.sendMsgINSIDE();
}

void Drone::setDroneOUTSIDE(){
  currentState = OUTSIDE;
  MsgService.sendMsgOUTSIDE();
}

void Drone::setDroneLANDING(){
  currentState = LANDING;
  MsgService.sendMsgLANDING();
}

void Drone::setDroneTAKINGOFF(){
  currentState = TAKING_OFF;
  MsgService.sendMsgTAKINGOFF();
}

bool Drone::isDroneINSIDE() {
  return currentState == INSIDE;
}

bool Drone::isDroneOUTSIDE() {
  return currentState == OUTSIDE;
}

bool Drone::isDroneLANDING() {
  return currentState == LANDING;
}

bool Drone::isDroneTAKINGOFF() {
  return currentState == TAKING_OFF;
}

void Drone::sendRequestTakeoff(){
  requestTakeoff = true;
}

void Drone::consumeRequestTakeOff(){
  requestTakeoff = false;
}

void Drone::sendRequestLanding(){
  requestLanding = true;
}

void Drone::consumeRequestLanding(){
  requestLanding = false;
}

bool Drone::isRequestTakeOffSent() {
  return requestTakeoff == true;
}

bool Drone::isRequestLandingSent() {
  return requestLanding == true;
}