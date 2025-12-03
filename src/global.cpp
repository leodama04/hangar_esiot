#include "global.h"


Drone drone = Drone();
HangarDoor hangarDoor = HangarDoor();

void globalSetup() {
    MsgService.init();
    hangarDoor.init(MOT_PIN, 0, 90, 5);
}