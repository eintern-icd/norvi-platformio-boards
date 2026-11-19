#include <Arduino.h>
#include <Wire.h>
#include "norvi_io.h"

bool psramTest() {
  const size_t N = 1024 * 1024;
  uint8_t *buf = (uint8_t *)ps_malloc(N);
  if (!buf) { Serial.println("PSRAM alloc FAILED"); return false; }
  for (size_t i = 0; i < N; i++) buf[i] = (uint8_t)(i * 31 + 7);
  size_t bad = 0;
  for (size_t i = 0; i < N; i++) if (buf[i] != (uint8_t)(i * 31 + 7)) bad++;
  free(buf);
  Serial.printf("PSRAM 1MB write/verify: %s (%u errors)\n", bad ? "FAIL" : "PASS", (unsigned)bad);
  return bad == 0;
}

void i2cScan() {
  for (uint8_t a = 1; a < 127; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) Serial.printf("I2C device at 0x%02X\n", a);
  }
}

bool lastB1 = false, lastB2 = false;
uint8_t ledStep = 0;
uint32_t ledTimer = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Wire.begin(SDA, SCL);
  pinMode(BOOT_BUTTON, INPUT_PULLUP);
  Serial1.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX);
  NorviIO::begin();

  Serial.printf("Flash: %u  PSRAM: %u\n", ESP.getFlashChipSize(), ESP.getPsramSize());
  psramTest();
  Serial.printf("Buttons/LEDs expander: %s\n", NorviIO::name());
  i2cScan();
  Serial.println("--- Watch the LEDs and press the front buttons ---");
  lastB1 = NorviIO::buttonRaw(1);
  lastB2 = NorviIO::buttonRaw(2);
}

void loop() {
  // Buttons: print on every change
  bool b1 = NorviIO::buttonRaw(1);
  bool b2 = NorviIO::buttonRaw(2);
  if (b1 != lastB1) { Serial.printf("BTN1 -> %d\n", b1); lastB1 = b1; }
  if (b2 != lastB2) { Serial.printf("BTN2 -> %d\n", b2); lastB2 = b2; }

  // LEDs: step every 2 seconds
  if (millis() - ledTimer >= 2000) {
    ledTimer = millis();
    switch (ledStep) {
      case 0: Serial.println("LED: RUN only");   NorviIO::led(true, false);  break;
      case 1: Serial.println("LED: ERROR only"); NorviIO::led(false, true);  break;
      case 2: Serial.println("LED: both off");   NorviIO::led(false, false); break;
    }
    ledStep = (ledStep + 1) % 3;
  }
  delay(20);
}