#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>
#include "soc/soc_caps.h"

// USB (VID/PID to be confirmed with lead; hardware reports 0x303A:0x1001)
#define USB_VID 0x303a
#define USB_PID 0x1001
#define USB_MANUFACTURER "NORVI"
#define USB_PRODUCT "NORVI XN"
#define USB_SERIAL ""

// Default UART0 (ESP32-S3 defaults, not from the datasheet)
static const uint8_t TX = 43;
static const uint8_t RX = 44;

// I2C (touch 0x15, PCA9536 0x41, DS3231 0x68, PCA9539 0x75)
static const uint8_t SDA = 8;
static const uint8_t SCL = 9;

// SPI (SS assumed to be the Ethernet CS, confirm with lead)
static const uint8_t SS   = 1;
static const uint8_t MOSI = 11;
static const uint8_t MISO = 13;
static const uint8_t SCK  = 12;

// Ethernet W5500
static const uint8_t W5500_CS  = 1;
static const uint8_t W5500_RST = 2;
static const uint8_t W5500_INT = 4;

// SD card
static const uint8_t SD_CS = 42;

// Display (ST7789) and touch (CST816S)
static const uint8_t DISPLAY_CS  = 45;
static const uint8_t DISPLAY_DC  = 46;
static const uint8_t DISPLAY_RST = 47;
static const uint8_t TOUCH_INT   = 48;

// RS-485 (direction control is automatic)
static const uint8_t RS485_TX = 15;
static const uint8_t RS485_RX = 16;

// Boot button
static const uint8_t BOOT_BUTTON = 0;

// PCA9536 expander (address 0x41): buttons and LEDs
// Hardware-tested on XN: buttons are here, not on the PCA9539 as the datasheet says.
// The *_EXP_PIN values are EXPANDER pins, not GPIOs.
#define NORVI_PCA9536_ADDR      0x41
static const uint8_t PCA9536_RST = 38;   // GPIO that resets the expander
#define NORVI_BTN1_EXP_PIN      0        // P0
#define NORVI_BTN2_EXP_PIN      3        // P3
#define NORVI_LED_ERROR_EXP_PIN 1        // P1
#define NORVI_LED_RUN_EXP_PIN   2        // P2

// PCA9539 expander (address 0x75): chip-selects for expansion modules
// Numbering: port 0 = 0..7, port 1 = 8..15
#define NORVI_PCA9539_ADDR      0x75
#define NORVI_EXP_CS1_PIN       8        // P1_0
#define NORVI_EXP_CS2_PIN       7        // P0_7
#define NORVI_EXP_CS3_PIN       6        // P0_6
#define NORVI_EXP_CS4_PIN       5        // P0_5

// Expansion port: UART and free GPIOs (the A7672X modem is handled
// by the hardware, so IO17/IO18 are plain expansion UART pins)
static const uint8_t EXP_TX1  = 17;
static const uint8_t EXP_RX1  = 18;
static const uint8_t EXP_IO5  = 5;
static const uint8_t EXP_IO6  = 6;
static const uint8_t EXP_IO7  = 7;
static const uint8_t EXP_IO10 = 10;
static const uint8_t EXP_IO14 = 14;
static const uint8_t EXP_IO21 = 21;
static const uint8_t EXP_IO39 = 39;
static const uint8_t EXP_IO40 = 40;

// ---- Two hardware versions: which chip holds the buttons and LEDs ----
// Version A: PCA9536 at 0x41 (NORVI_BTN*/NORVI_LED_* above; hardware tested on XN).
// Version B: PCA9539 at 0x75 (NORVI_B_* below; from XN datasheet, NOT yet hardware tested).
// Detect at runtime: if 0x41 answers use version A, otherwise version B.
#ifndef NORVI_PCA9536_ADDR
#define NORVI_PCA9536_ADDR      0x41
#endif
#ifndef NORVI_PCA9539_ADDR
#define NORVI_PCA9539_ADDR      0x75
#endif
#define NORVI_B_BTN1_EXP_PIN      1   // P0_1 (left button, hardware tested on X1 N16R8)
#define NORVI_B_BTN2_EXP_PIN      2   // P0_2 (right button, hardware tested on X1 N16R8)
#define NORVI_B_LED_RUN_EXP_PIN   4   // P0_4 (active-low, hardware tested on X1 N16R8; datasheet had P0_3)
#define NORVI_B_LED_ERROR_EXP_PIN 3   // P0_3 (active-low, hardware tested on X1 N16R8; datasheet had P0_4)

#endif /* Pins_Arduino_h */