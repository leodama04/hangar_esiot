#include "global.h"


Drone drone = Drone();
HangarDoor hangarDoor = HangarDoor();
ProximitySensor* DDD = new Sonar(ECHO_PIN, TRIG_PIN, 30000);
PresenceSensor* DPD;
LedPwm* L2 = new LedPwm(L2_PIN);
Led* L1 = new Led(L1_PIN);
Led* L3 = new Led(L3_PIN);
BlinkingTask blinkingTask;
LandingTask landingTask;
TakeOffTask takeOffTask;
TempSensor* tempSensor = new TempSensorTMP(TMP_PIN);
Button* button = new ButtonImpl(BUT_PIN);

void globalSetup() {
    DPD = new Pir(PIR_PIN);
    MsgService.init();
    hangarDoor.init(MOT_PIN, 0, 90, 5);
    takeOffTask.init(200);
    landingTask.init(200);
    blinkingTask.init(5, L2);
}