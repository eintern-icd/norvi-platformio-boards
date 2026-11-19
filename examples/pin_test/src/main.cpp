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

void setup() {
  Serial.begin(115200);
  delay(2000);
  Wire.begin(SDA, SCL);
  pinMode(BOOT_BUTTON, INPUT_PULLUP);
  Serial1.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX);
  NorviIO::begin();
}

void loop() {
  Serial.printf("Flash: %u  PSRAM: %u\n", ESP.getFlashChipSize(), ESP.getPsramSize());
  psramTest();
  Serial.printf("Buttons/LEDs expander: %s\n", NorviIO::name());
  Serial.printf("BTN1 raw=%d  BTN2 raw=%d  BOOT=%s\n",
                NorviIO::buttonRaw(1), NorviIO::buttonRaw(2),
                digitalRead(BOOT_BUTTON) ? "released" : "PRESSED");
  Serial.println("LED: RUN");   NorviIO::led(true, false);  delay(1000);
  Serial.println("LED: ERROR"); NorviIO::led(false, true);  delay(1000);
  Serial.println("LED: off");   NorviIO::led(false, false);
  i2cScan();
  Serial.println("---");
  delay(2000);
}