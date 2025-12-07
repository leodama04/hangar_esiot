#include "global.h"


Drone drone = Drone();
HangarDoor hangarDoor = HangarDoor();
ProximitySensor* DDD = new Sonar(ECHO_PIN, TRIG_PIN, 30000);
PresenceSensor* DPD;

void globalSetup() {
    DPD = new Pir(PIR_PIN);
    MsgService.init();
    hangarDoor.init(MOT_PIN, 0, 90, 5);

}