#include "Pir.h"
#include "Arduino.h"

#define PIR_READ_INTERVAL 100

Pir::Pir(int pin) {
  this->pin = pin;
  pinMode(pin, INPUT);
  this->calibrate();
  detected = false;
  lastTimeSync = 0;
}

void Pir::sync() {
  detected = digitalRead(pin) == HIGH;
  updateSyncTime(millis());
}

bool Pir::isDetected() {
  unsigned long now = millis();
  if (now - lastTimeSync >= PIR_READ_INTERVAL) {
    this->sync();
  }
  return detected;
}

void Pir::calibrate() {
  delay(10000);
}

void Pir::updateSyncTime(long time) {
  lastTimeSync = time;
}

long Pir::getLastSyncTime() {
  return lastTimeSync;
}
