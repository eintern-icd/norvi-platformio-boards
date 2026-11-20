# NORVI PlatformIO Boards

Custom PlatformIO board definitions for NORVI X controllers (ESP32-S3).

| Board ID | Controller | Flash / PSRAM | Modem |
|---|---|---|---|
| norvi_x1 | NORVI CPU-ESPS3-X1 | 16 MB / 2 MB quad (N16R2) | none |
| norvi_x1_n16r8 | NORVI CPU-ESPS3-X1 | 16 MB / 8 MB octal (N16R8) | none |
| norvi_x2 | NORVI CPU-ESPS3-X2 | 16 MB / 2 MB quad (N16R2) | SIMCOM A7672G |
| norvi_x3 | NORVI CPU-ESPS3-X3 | 16 MB / 2 MB quad (N16R2) | Quectel EC25 |
| norvi_xn | NORVI XN-MAIN | 16 MB / 2 MB quad (N16R2) | A7672X |

All boards: ESP32-S3, 16 MB flash (QIO 80 MHz), native USB, 240 MHz.

> A pull request to platformio/platform-espressif32 is planned. Once it is merged, these files are no longer needed.

## Install

1. Copy the `boards/` and `variants/` folders into your PlatformIO project, next to `platformio.ini`.
2. Add this to `platformio.ini`:

```ini
[env:norvi_x1]
platform = espressif32
board = norvi_x1
framework = arduino
board_build.variants_dir = variants
board_build.variant = norvi_x1
monitor_speed = 115200
```

Use `norvi_x1_n16r8`, `norvi_x2`, `norvi_x3` or `norvi_xn` for the other boards.

3. Build with `pio run`.

## Named pins

The variant files define names such as `SDA`, `SCL`, `RS485_TX`, `RS485_RX`, `W5500_CS`, `SD_CS` and `DISPLAY_CS`. See `variants/<board>/pins_arduino.h` for the full list.

Buttons and LEDs are on I2C expanders. The `NORVI_*_EXP_PIN` values are expander pin numbers, not GPIOs. Digital inputs, relays and analog inputs come from NORVI expansion modules and are not defined here.

Expander found on the test units (see `examples/full_test`):

- XN: PCA9536 at 0x41 (buttons PB1/PB3), PCA9539 at 0x75.
- X1 N16R8: no PCA9536; buttons and LEDs on the PCA9539 at 0x75.
- X1 N16R2: PCA9536 at 0x41 (buttons PB1/PB3, LEDs), PCA9539 at 0x75. In `full_test`, PB3 is reported as the "PB2" read.

The `full_test` example detects which expander is present at start-up.

## Notes

- Upload uses the ESP32-S3 native USB. USB CDC On Boot is enabled.
- Partition table: `default_16MB.csv`.
- `norvi_x1_n16r8` uses octal PSRAM (`qio_opi`). On octal-PSRAM modules GPIO35 to GPIO37 are used by the PSRAM and are not available as normal I/O.
- USB VID/PID uses the Espressif default (0x303A:0x1001), pending confirmation.

## Tested versions

Espressif 32 platform 8.0.0, framework-arduinoespressif32 3.3.12, esptool 5.4.0, PlatformIO Core 6.2.0.

## Test results

Results for the units tested so far, using `examples/full_test`.

| Board | Build | Upload | Chip check | full_test |
|---|---|---|---|---|
| norvi_xn | Pass | Pass | 2 MB PSRAM | Pass: PSRAM, PCA9536 buttons, RTC, Wi-Fi AP, display |
| norvi_x1_n16r8 | Pass | Pass | 8 MB PSRAM | Pass: PSRAM 8 MB, PCA9539 buttons, LEDs, touch, RTC, Wi-Fi AP, display |
| norvi_x1 (N16R2) | Pass | Pass | 2 MB PSRAM | Pass: PSRAM, PCA9536 buttons, LEDs, touch, RTC, Wi-Fi AP, display, output test (SD, Ethernet, NTP, RS485 not tested: no card, cable, network or loopback) |

Not covered by the hardware tests: SD card, Ethernet, RS485 and NTP.

### Serial output

Serial output from `examples/full_test` on the NORVI X1 (N16R2):
[test-results/norvi_x1_n16r2_serial.txt](test-results/norvi_x1_n16r2_serial.txt)

Summary block from that run:

```text
================ TEST SUMMARY ================
I2C devices : 4 found (0x41 present = PCA9536 version, absent = PCA9539 version)
Buttons/LEDs: PCA9536 @0x41 (OK)
RTC DS3231  : OK
PSRAM       : 2097152 bytes
SD card     : NOT FOUND (no card?)
Ethernet    : NOT OK (no cable or no link)
NTP         : NO RESPONSE
Wi-Fi AP    : OK
RS485 echo  : NO ECHO (no loopback?)
==============================================
```

SD card, Ethernet, NTP and RS485 show as not OK because no card, cable, network or loopback was connected.

Serial output for the NORVI XN (full_test): [test-results/norvi_xn_serial.txt](test-results/norvi_xn_serial.txt)

Serial output for the NORVI X1 N16R8 (full_test): [test-results/norvi_x1_n16r8_serial.txt](test-results/norvi_x1_n16r8_serial.txt)

## Links

- https://norvi.io/
- https://github.com/NORVIControllers/NORVI-X-Version02

## Troubleshooting

If the serial monitor is blank after an upload, try `platformio run -e norvi_x1 -t clean`, upload again, and open the monitor with `platformio device monitor --dtr 0 --rts 0`. This fixed a blank monitor on one N16R2 X1 unit. The exact cause was not confirmed.
