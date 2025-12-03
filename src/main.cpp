#include <Arduino.h>
#include "Scheduler.h"
#include "global.h"
#include "tasks/TakeOffTask.h"

Scheduler scheduler;
TakeOffTask takeOffTask;

void setup() {
  globalSetup();
  takeOffTask.init(200);
  scheduler.init(200);
  scheduler.addTask(&takeOffTask);
  delay(100);
  MsgService.sendMsg("READY");
}

void loop() {
  scheduler.schedule();
}

