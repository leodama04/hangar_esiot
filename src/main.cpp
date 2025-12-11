#include <Arduino.h>
#include "Scheduler.h"
#include "global.h"
#include "tasks/LandingTask.h"
#include "tasks/TakeOffTask.h"
#include "tasks/AlarmTask.h"

Scheduler scheduler;
AlarmTask alarmTask;

void setup() {
  globalSetup();
  alarmTask.init(100);
  scheduler.init(5);
  scheduler.addTask(&takeOffTask);
  scheduler.addTask(&landingTask);
  scheduler.addTask(&blinkingTask);
  scheduler.addTask(&alarmTask);
}

void loop() {
  scheduler.schedule();          
}

