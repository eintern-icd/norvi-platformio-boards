#include <Arduino.h>
#include <Wire.h>

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
}

void loop() {
  Serial.printf("Flash: %u  PSRAM: %u  free PSRAM: %u\n",
                ESP.getFlashChipSize(), ESP.getPsramSize(), ESP.getFreePsram());
  psramTest();
  Serial.printf("Pins: SDA=%d SCL=%d W5500_CS=%d SD_CS=%d RS485_TX=%d RS485_RX=%d\n",
                SDA, SCL, W5500_CS, SD_CS, RS485_TX, RS485_RX);
  Serial.printf("BOOT button: %s\n", digitalRead(BOOT_BUTTON) ? "released" : "PRESSED");
  Serial1.println("RS485 test");
  i2cScan();
  Serial.println("---");
  delay(5000);
}