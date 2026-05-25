#include "global.h"


Drone drone = Drone();
HangarDoor hangarDoor = HangarDoor();
ProximitySensor* DDD = new Sonar(ECHO_PIN, TRIG_PIN, 30000);
PresenceSensor* DPD;
Led* L2 = new Led(L2_PIN);
Led* L1 = new Led(L1_PIN);
Led* L3 = new Led(L3_PIN);
BlinkingTask blinkingTask;
LandingTask landingTask;
TakeOffTask takeOffTask;
TempSensor* tempSensor = new TempSensorTMP(TMP_PIN);
Button* button = new ButtonImpl(BUT_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);  

void globalSetup() {
    DPD = new Pir(PIR_PIN);
    MsgService.init();
    hangarDoor.init(MOT_PIN, 0, 90, 5);
    takeOffTask.init(200);
    landingTask.init(200);
    blinkingTask.init(500, L2);
    L1->switchOn();
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("DRONE INSIDE");
    drone.setDroneINSIDE();
}