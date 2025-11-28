#include <Arduino.h>
#include "Scheduler.h"

Scheduler sched;

void setup() {
  sched.init(100);
}

void loop() {
  sched.schedule();
}
