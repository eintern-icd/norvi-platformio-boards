#include <Arduino.h>
#include <Wire.h>

void i2cScan() {
  for (uint8_t a = 1; a < 127; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) {
      Serial.printf("I2C device at 0x%02X\n", a);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.printf("Flash size: %u bytes\n", ESP.getFlashChipSize());
  Serial.printf("PSRAM size: %u bytes\n", ESP.getPsramSize());
  Wire.begin(SDA, SCL);
  Serial1.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX);
  Serial.printf("W5500 CS = %d, SD CS = %d\n", W5500_CS, SD_CS);
  i2cScan();
}

void loop() {
  delay(1000);
}