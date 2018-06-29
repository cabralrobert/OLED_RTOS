#ifndef OLED_TEST_H
#define OLED_TEST_H

#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>
#include <string.h>

/* TI-RTOS Header files */
#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
#include <ti/drv/i2c/test/eeprom_read/src/I2C_log.h>
#include <ti/drv/i2c/test/eeprom_read/src/I2C_board.h>

#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
#include <ti/drv/i2c/test/eeprom_read/src/I2C_log.h>
#include <ti/drv/i2c/test/eeprom_read/src/I2C_board.h>

#define I2C_TRANSACTION_TIMEOUT         (2000U)

#define SSD1306_I2C_ADDRESS  0x3C

#define SSD1306_ADDR        0x3C

#define LEFT    0
#define RIGHT   9999
#define CENTER  9998

#define SSD1306_COMMAND         0x00
#define SSD1306_DATA            0xC0
#define SSD1306_DATA_CONTINUE   0x40

#define RST_NOT_IN_USE  255

// SSD1306 Commandset
// ------------------
// Fundamental Commands
#define SSD1306_SET_CONTRAST_CONTROL                    0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME                   0xA4
#define SSD1306_DISPLAY_ALL_ON                          0xA5
#define SSD1306_NORMAL_DISPLAY                          0xA6
#define SSD1306_INVERT_DISPLAY                          0xA7
#define SSD1306_DISPLAY_OFF                             0xAE
#define SSD1306_DISPLAY_ON                              0xAF
#define SSD1306_NOP                                     0xE3
// Scrolling Commands
#define SSD1306_HORIZONTAL_SCROLL_RIGHT                 0x26
#define SSD1306_HORIZONTAL_SCROLL_LEFT                  0x27
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_RIGHT    0x29
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_LEFT     0x2A
#define SSD1306_DEACTIVATE_SCROLL                       0x2E
#define SSD1306_ACTIVATE_SCROLL                         0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA                0xA3
// Addressing Setting Commands
#define SSD1306_SET_LOWER_COLUMN                        0x00
#define SSD1306_SET_HIGHER_COLUMN                       0x10
#define SSD1306_MEMORY_ADDR_MODE                        0x20
#define SSD1306_SET_COLUMN_ADDR                         0x21
#define SSD1306_SET_PAGE_ADDR                           0x22
// Hardware Configuration Commands
#define SSD1306_SET_START_LINE                          0x40
#define SSD1306_SET_SEGMENT_REMAP                       0xA0
#define SSD1306_SET_MULTIPLEX_RATIO                     0xA8
#define SSD1306_COM_SCAN_DIR_INC                        0xC0
#define SSD1306_COM_SCAN_DIR_DEC                        0xC8
#define SSD1306_SET_DISPLAY_OFFSET                      0xD3
#define SSD1306_SET_COM_PINS                            0xDA
#define SSD1306_CHARGE_PUMP                             0x8D
// Timing & Driving Scheme Setting Commands
#define SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO             0xD5
#define SSD1306_SET_PRECHARGE_PERIOD                    0xD9
#define SSD1306_SET_VCOM_DESELECT   0xDB

#define C_OledLastLine_U8 0x07u

#define FONT_SIZE 5
static const uint8_t OledFontTable[][FONT_SIZE] = {
                                                   0x00, 0x00, 0x00, 0x00, 0x00,   // space
                                                   0x00, 0x00, 0x2f, 0x00, 0x00,   // !
                                                   0x00, 0x07, 0x00, 0x07, 0x00,   // "
                                                   0x14, 0x7f, 0x14, 0x7f, 0x14,   // #
                                                   0x24, 0x2a, 0x7f, 0x2a, 0x12,   // $
                                                   0x23, 0x13, 0x08, 0x64, 0x62,   // %
                                                   0x36, 0x49, 0x55, 0x22, 0x50,   // &
                                                   0x00, 0x05, 0x03, 0x00, 0x00,   // '
                                                   0x00, 0x1c, 0x22, 0x41, 0x00,   // (
                                                   0x00, 0x41, 0x22, 0x1c, 0x00,   // )
                                                   0x14, 0x08, 0x3E, 0x08, 0x14,   // *
                                                   0x08, 0x08, 0x3E, 0x08, 0x08,   // +
                                                   0x00, 0x00, 0xA0, 0x60, 0x00,   // ,
                                                   0x08, 0x08, 0x08, 0x08, 0x08,   // -
                                                   0x00, 0x60, 0x60, 0x00, 0x00,   // .
                                                   0x20, 0x10, 0x08, 0x04, 0x02,   // /
                                                   0x3E, 0x51, 0x49, 0x45, 0x3E,   // 0
                                                   0x00, 0x42, 0x7F, 0x40, 0x00,   // 1
                                                   0x42, 0x61, 0x51, 0x49, 0x46,   // 2
                                                   0x21, 0x41, 0x45, 0x4B, 0x31,   // 3
                                                   0x18, 0x14, 0x12, 0x7F, 0x10,   // 4
                                                   0x27, 0x45, 0x45, 0x45, 0x39,   // 5
                                                   0x3C, 0x4A, 0x49, 0x49, 0x30,   // 6
                                                   0x01, 0x71, 0x09, 0x05, 0x03,   // 7
                                                   0x36, 0x49, 0x49, 0x49, 0x36,   // 8
                                                   0x06, 0x49, 0x49, 0x29, 0x1E,   // 9
                                                   0x00, 0x36, 0x36, 0x00, 0x00,   // :
                                                   0x00, 0x56, 0x36, 0x00, 0x00,   // ;
                                                   0x08, 0x14, 0x22, 0x41, 0x00,   // <
                                                   0x14, 0x14, 0x14, 0x14, 0x14,   // =
                                                   0x00, 0x41, 0x22, 0x14, 0x08,   // >
                                                   0x02, 0x01, 0x51, 0x09, 0x06,   // ?
                                                   0x32, 0x49, 0x59, 0x51, 0x3E,   // @
                                                   0x7C, 0x12, 0x11, 0x12, 0x7C,   // A
                                                   0x7F, 0x49, 0x49, 0x49, 0x36,   // B
                                                   0x3E, 0x41, 0x41, 0x41, 0x22,   // C
                                                   0x7F, 0x41, 0x41, 0x22, 0x1C,   // D
                                                   0x7F, 0x49, 0x49, 0x49, 0x41,   // E
                                                   0x7F, 0x09, 0x09, 0x09, 0x01,   // F
                                                   0x3E, 0x41, 0x49, 0x49, 0x7A,   // G
                                                   0x7F, 0x08, 0x08, 0x08, 0x7F,   // H
                                                   0x00, 0x41, 0x7F, 0x41, 0x00,   // I
                                                   0x20, 0x40, 0x41, 0x3F, 0x01,   // J
                                                   0x7F, 0x08, 0x14, 0x22, 0x41,   // K
                                                   0x7F, 0x40, 0x40, 0x40, 0x40,   // L
                                                   0x7F, 0x02, 0x0C, 0x02, 0x7F,   // M
                                                   0x7F, 0x04, 0x08, 0x10, 0x7F,   // N
                                                   0x3E, 0x41, 0x41, 0x41, 0x3E,   // O
                                                   0x7F, 0x09, 0x09, 0x09, 0x06,   // P
                                                   0x3E, 0x41, 0x51, 0x21, 0x5E,   // Q
                                                   0x7F, 0x09, 0x19, 0x29, 0x46,   // R
                                                   0x46, 0x49, 0x49, 0x49, 0x31,   // S
                                                   0x01, 0x01, 0x7F, 0x01, 0x01,   // T
                                                   0x3F, 0x40, 0x40, 0x40, 0x3F,   // U
                                                   0x1F, 0x20, 0x40, 0x20, 0x1F,   // V
                                                   0x3F, 0x40, 0x38, 0x40, 0x3F,   // W
                                                   0x63, 0x14, 0x08, 0x14, 0x63,   // X
                                                   0x07, 0x08, 0x70, 0x08, 0x07,   // Y
                                                   0x61, 0x51, 0x49, 0x45, 0x43,   // Z
                                                   0x00, 0x7F, 0x41, 0x41, 0x00,   // [
                                                   0x55, 0xAA, 0x55, 0xAA, 0x55,   // Backslash (Checker pattern)
                                                   0x00, 0x41, 0x41, 0x7F, 0x00,   // ]
                                                   0x04, 0x02, 0x01, 0x02, 0x04,   // ^
                                                   0x40, 0x40, 0x40, 0x40, 0x40,   // _
                                                   0x00, 0x03, 0x05, 0x00, 0x00,   // `
                                                   0x20, 0x54, 0x54, 0x54, 0x78,   // a
                                                   0x7F, 0x48, 0x44, 0x44, 0x38,   // b
                                                   0x38, 0x44, 0x44, 0x44, 0x20,   // c
                                                   0x38, 0x44, 0x44, 0x48, 0x7F,   // d
                                                   0x38, 0x54, 0x54, 0x54, 0x18,   // e
                                                   0x08, 0x7E, 0x09, 0x01, 0x02,   // f
                                                   0x18, 0xA4, 0xA4, 0xA4, 0x7C,   // g
                                                   0x7F, 0x08, 0x04, 0x04, 0x78,   // h
                                                   0x00, 0x44, 0x7D, 0x40, 0x00,   // i
                                                   0x40, 0x80, 0x84, 0x7D, 0x00,   // j
                                                   0x7F, 0x10, 0x28, 0x44, 0x00,   // k
                                                   0x00, 0x41, 0x7F, 0x40, 0x00,   // l
                                                   0x7C, 0x04, 0x18, 0x04, 0x78,   // m
                                                   0x7C, 0x08, 0x04, 0x04, 0x78,   // n
                                                   0x38, 0x44, 0x44, 0x44, 0x38,   // o
                                                   0xFC, 0x24, 0x24, 0x24, 0x18,   // p
                                                   0x18, 0x24, 0x24, 0x18, 0xFC,   // q
                                                   0x7C, 0x08, 0x04, 0x04, 0x08,   // r
                                                   0x48, 0x54, 0x54, 0x54, 0x20,   // s
                                                   0x04, 0x3F, 0x44, 0x40, 0x20,   // t
                                                   0x3C, 0x40, 0x40, 0x20, 0x7C,   // u
                                                   0x1C, 0x20, 0x40, 0x20, 0x1C,   // v
                                                   0x3C, 0x40, 0x30, 0x40, 0x3C,   // w
                                                   0x44, 0x28, 0x10, 0x28, 0x44,   // x
                                                   0x1C, 0xA0, 0xA0, 0xA0, 0x7C,   // y
                                                   0x44, 0x64, 0x54, 0x4C, 0x44,   // z
                                                   0x00, 0x10, 0x7C, 0x82, 0x00,   // {
                                                   0x00, 0x00, 0xFF, 0x00, 0x00,   // |
                                                   0x00, 0x82, 0x7C, 0x10, 0x00,   // }
                                                   0x00, 0x06, 0x09, 0x09, 0x06    // ~ (Degrees)
};

#define C_DisplayDefaultDigits_U8 0xffu
#define C_MaxDigitsToDisplayUsingPrintf_U8  0xff
#define C_MaxDigitsToDisplay_U8             10u
#define C_BINARY_U8                         2
#define C_DECIMAL_U8                        10
#define C_HEX_U8                            16
#define  util_GetBitMask(bit)          (1<<(bit))
#define  util_GetBitStatus(x,bit)      (((x)&(util_GetBitMask(bit)))!=0u)
#define util_Dec2Ascii(Dec) ((Dec)+0x30)
#define util_GetMod32(dividend,divisor) (uint32_t)(dividend - (divisor * (uint32_t)(dividend/divisor)))
#define util_Hex2Ascii(Hex) (((Hex)>0x09) ? ((Hex) + 0x37): ((Hex) + 0x30))

class oled{
private:
    I2C_Handle* handle;
    I2C_Transaction i2cTransaction;
    int16_t transferStatus;
    uint16_t OledLineNum, OledCursorPos;

public:

    oled(I2C_Handle* handle);
    bool test();
    bool writeCommand(char value);
    bool writeByte(char value);
    void displayChar(uint16_t ch);
    void setCursor(uint16_t lineNumber, uint16_t cursorPosition);
    void displayImage(char* img);
    void clear();
    void goToNextLine();
    void init();
    void displayString(char *ptr);
    void displayNumber(uint8_t v_numericSystem_u8, uint32_t v_number_u32, uint8_t v_numOfDigitsToDisplay_u8);
    void printf(const char *argList, ...);

};

#endif
