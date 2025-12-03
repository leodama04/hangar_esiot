#ifndef __GLOBAL__
#define __GLOBAL__

#include "Drone.h"
#include "HangarDoor.h"
#include "MsgService.h"

#define MOT_PIN 9

extern Drone drone;
extern HangarDoor hangarDoor;

void globalSetup();

#endif