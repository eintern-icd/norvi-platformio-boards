# NORVI PlatformIO Boards

Custom PlatformIO board definitions for NORVI X controllers (ESP32-S3).

| Board ID  | Controller         | Modem         |
|-----------|--------------------|---------------|
| norvi_x1  | NORVI CPU-ESPS3-X1 | none          |
| norvi_x2  | NORVI CPU-ESPS3-X2 | SIMCOM A7672G |
| norvi_x3  | NORVI CPU-ESPS3-X3 | Quectel EC25  |
| norvi_xn  | NORVI XN-MAIN      | A7672X        |

All boards: ESP32-S3-WROOM-1U-N16R2, 16 MB flash (QIO 80 MHz), 2 MB quad PSRAM, native USB, 240 MHz.

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

Use `norvi_x2`, `norvi_x3` or `norvi_xn` for the other boards.

3. Build with `pio run`.

## Named pins

The variant files define names such as `SDA`, `SCL`, `RS485_TX`, `RS485_RX`, `W5500_CS`, `SD_CS` and `DISPLAY_CS`. See `variants/<board>/pins_arduino.h` for the full list.

Buttons and LEDs are on I2C expanders (PCA9536 on X1/X2/X3, PCA9539 on XN). The `NORVI_*_EXP_PIN` values are expander pin numbers, not GPIOs. Digital inputs, relays and analog inputs come from NORVI expansion modules and are not defined here.

## Notes

- Upload uses the ESP32-S3 native USB. USB CDC On Boot is enabled.
- Partition table: `default_16MB.csv`.
- The customised N16R8 (octal PSRAM) variants are not covered.
- USB VID/PID uses the Espressif default (0x303A:0x1001), pending confirmation.

## Status

Build-tested for all four boards. Hardware flash test: pending.

## Links

- https://norvi.io/
- https://github.com/NORVIControllers/NORVI-X-Version02