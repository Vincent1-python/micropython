// Both of these can be set by mpconfigboard.cmake if a BOARD_VARIANT is
// specified.

#ifndef MICROPY_HW_BOARD_NAME
#define MICROPY_HW_BOARD_NAME "Generic ESP32P4 module"
#endif

#ifndef MICROPY_HW_MCU_NAME
#define MICROPY_HW_MCU_NAME "ESP32P4"
#endif

#define MICROPY_PY_ESPNOW         	 (0)

#define MICROPY_HW_ENABLE_SDCARD            (0)

#ifndef USB_SERIAL_JTAG_PACKET_SZ_BYTES
#define USB_SERIAL_JTAG_PACKET_SZ_BYTES (64)
#endif

// Enable UART REPL for modules that have an external USB-UART and don't use native USB.
#define MICROPY_HW_ENABLE_UART_REPL             (1)

#define MICROPY_PY_MACHINE_I2S (1)

/*
#define MICROPY_HW_I2C0_SCL      (32)
#define MICROPY_HW_I2C0_SDA      (31)

#define MICROPY_HW_SPI1_MOSI                (44)
#define MICROPY_HW_SPI1_MISO                (39)
#define MICROPY_HW_SPI1_SCK                 (43)
*/
