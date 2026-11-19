#pragma once
#include <Arduino.h>
#include <Wire.h>

// Detects which expander holds the front buttons and LEDs.
// 0x41 (PCA9536) is checked first, because 0x75 (PCA9539) exists on both versions.
enum NorviExpType : uint8_t { NORVI_EXP_NONE = 0, NORVI_EXP_PCA9536, NORVI_EXP_PCA9539 };

namespace NorviIO {
  static NorviExpType type = NORVI_EXP_NONE;
  static uint8_t addr = 0, regIn = 0, regOut = 0, regCfg = 0;
  static uint8_t pinBtn1 = 0, pinBtn2 = 0, pinRun = 0, pinErr = 0;

  static bool present(uint8_t a) {
    Wire.beginTransmission(a);
    return Wire.endTransmission() == 0;
  }
  static bool rd(uint8_t reg, uint8_t &v) {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) return false;
    if (Wire.requestFrom(addr, (uint8_t)1) != 1) return false;
    v = Wire.read();
    return true;
  }
  static bool wr(uint8_t reg, uint8_t v) {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(v);
    return Wire.endTransmission() == 0;
  }

  // Call after Wire.begin(SDA, SCL). Returns the detected type.
  static NorviExpType begin() {
    if (present(NORVI_PCA9536_ADDR)) {
      type = NORVI_EXP_PCA9536; addr = NORVI_PCA9536_ADDR;
      regIn = 0x00; regOut = 0x01; regCfg = 0x03;
      pinBtn1 = NORVI_BTN1_EXP_PIN; pinBtn2 = NORVI_BTN2_EXP_PIN;
      pinRun = NORVI_LED_RUN_EXP_PIN; pinErr = NORVI_LED_ERROR_EXP_PIN;
    } else if (present(NORVI_PCA9539_ADDR)) {
      type = NORVI_EXP_PCA9539; addr = NORVI_PCA9539_ADDR;
      regIn = 0x00; regOut = 0x02; regCfg = 0x06;   // port 0 registers
      pinBtn1 = NORVI_B_BTN1_EXP_PIN; pinBtn2 = NORVI_B_BTN2_EXP_PIN;
      pinRun = NORVI_B_LED_RUN_EXP_PIN; pinErr = NORVI_B_LED_ERROR_EXP_PIN;
    } else {
      type = NORVI_EXP_NONE;
      return type;
    }
    // LEDs: outputs, off (high). Read-modify-write so buttons and chip-selects are untouched.
    uint8_t out = 0xFF, cfg = 0xFF;
    rd(regOut, out); out |= (1 << pinRun) | (1 << pinErr); wr(regOut, out);
    rd(regCfg, cfg); cfg &= ~((1 << pinRun) | (1 << pinErr)); wr(regCfg, cfg);
    return type;
  }

  // Raw pin level of button 1 or 2 (1 = high). Polarity: a press read 1 on XN (PCA9536);
  // the PCA9539 version is untested.
  static bool buttonRaw(uint8_t n) {
    uint8_t v = 0;
    rd(regIn, v);
    return v & (1 << (n == 1 ? pinBtn1 : pinBtn2));
  }

  // LEDs assumed active-low (confirmed on XN PCA9536; PCA9539 version untested).
  static void led(bool run, bool err) {
    uint8_t out = 0xFF;
    rd(regOut, out);
    out |= (1 << pinRun) | (1 << pinErr);
    if (run) out &= ~(1 << pinRun);
    if (err) out &= ~(1 << pinErr);
    wr(regOut, out);
  }

  static const char *name() {
    return type == NORVI_EXP_PCA9536 ? "PCA9536 @0x41"
         : type == NORVI_EXP_PCA9539 ? "PCA9539 @0x75" : "none";
  }
}