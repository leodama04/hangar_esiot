#include <Arduino.h>
#include "Scheduler.h"
#include "global.h"
#include "tasks/LandingTask.h"
#include "tasks/TakeOffTask.h"

Scheduler scheduler;
LandingTask landingTask;
TakeOffTask takeOffTask;

void setup() {
  globalSetup();
  takeOffTask.init(200);
  landingTask.init(200);
  scheduler.init(200);
  scheduler.addTask(&takeOffTask);
  scheduler.addTask(&landingTask);
}

void loop() {
  scheduler.schedule();
}

