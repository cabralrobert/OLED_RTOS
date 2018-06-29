#include <inc/oled.h>
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

oled::oled(I2C_Handle* handle) : handle(handle), OledLineNum(0), OledCursorPos(0) {
    I2C_transactionInit(&this->i2cTransaction);
    i2cTransaction.slaveAddress = SSD1306_I2C_ADDRESS;
    i2cTransaction.timeout      = I2C_TRANSACTION_TIMEOUT;
}

bool oled::test(){
    uint8_t txData = SSD1306_I2C_ADDRESS;
    int16_t transferStatus;
    i2cTransaction.writeBuf     = &txData;
    i2cTransaction.writeCount   = sizeof(uint8_t);
    transferStatus = I2C_transfer(*handle, &i2cTransaction);

#ifdef DEBUG_OLED
    UART_printf("%d\n", transferStatus);
#endif

    if(transferStatus == I2C_STS_SUCCESS)
        return true;

    return false;
}

bool oled::writeCommand(char value){
    char txData[] = {SSD1306_COMMAND, value};
    i2cTransaction.writeBuf     = txData;
    i2cTransaction.writeCount   = sizeof(txData);
    transferStatus = I2C_transfer(*handle, &i2cTransaction);

    if(transferStatus == I2C_STS_SUCCESS)
        return true;

    return false;
}

bool oled::writeByte(char value){
    char txData[] = {SSD1306_DATA, value};
    i2cTransaction.writeBuf     = txData;
    i2cTransaction.writeCount   = sizeof(txData);
    transferStatus = I2C_transfer(*handle, &i2cTransaction);

    if(transferStatus == I2C_STS_SUCCESS)
        return true;

    return false;
}

void oled::displayChar(uint16_t ch){
    uint8_t dat,i=0;

    if(((OledCursorPos+FONT_SIZE)>=128) || (ch=='\n')){
        this->goToNextLine();
    }if(ch != '\n'){


        ch = ch-0x20; // As the lookup table starts from Space(0x20)

        while(1){
            dat= OledFontTable[ch][i];

            writeByte(dat);
            OledCursorPos++;
            i++;

            if(i == FONT_SIZE){
                writeByte(0);
                OledCursorPos++;
                break;
            }
        }
    }
}

void oled::setCursor(uint16_t lineNumber, uint16_t cursorPosition){
    if((lineNumber <= C_OledLastLine_U8) && (cursorPosition <= 127)){
        this->OledLineNum = lineNumber;
        this->OledCursorPos = cursorPosition;

        writeCommand(SSD1306_SET_COLUMN_ADDR);
        writeCommand(cursorPosition);
        writeCommand(127);

        writeCommand(SSD1306_SET_PAGE_ADDR);
        writeCommand(lineNumber);
        writeCommand(7);

        writeByte(SSD1306_DATA_CONTINUE);
    }
}

void oled::displayImage(char* img){
    int i=0;

    setCursor(0, 0);

    writeByte(SSD1306_DATA_CONTINUE);

    for(i = 0; i < 1024; i++){
        writeByte(img[i]);
    }
}

void oled::clear(){
    int i;

    writeCommand(SSD1306_SET_COLUMN_ADDR);
    writeCommand(0);
    writeCommand(127);

    writeCommand(SSD1306_SET_PAGE_ADDR);
    writeCommand(0);
    writeCommand(7);

    writeByte(SSD1306_DATA_CONTINUE);

    for(i = 0; i < 1024; i++)
        writeByte(0);

    writeCommand(SSD1306_SET_COLUMN_ADDR);
    writeCommand(0);
    writeCommand(127);

    writeCommand(SSD1306_SET_PAGE_ADDR);
    writeCommand(0);
    writeCommand(7);

    writeByte(SSD1306_DATA_CONTINUE);
}

void oled::goToNextLine(){
    this->OledLineNum++;
    this->OledLineNum = this->OledLineNum & 7;
    this->setCursor(this->OledLineNum, 0);
}

void oled::init(){
    writeCommand(SSD1306_DISPLAY_OFF);
    writeCommand(SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO);
    writeCommand(0x80);
    writeCommand(SSD1306_SET_MULTIPLEX_RATIO);
    writeCommand(0x3F);
    writeCommand(SSD1306_SET_DISPLAY_OFFSET);
    writeCommand(0x0);
    writeCommand(SSD1306_SET_START_LINE | 0x0);
    writeCommand(SSD1306_CHARGE_PUMP);
    writeCommand(0x14);
    writeCommand(SSD1306_MEMORY_ADDR_MODE);
    writeCommand(0x00);
    writeCommand(SSD1306_SET_SEGMENT_REMAP | 0x1);
    writeCommand(SSD1306_COM_SCAN_DIR_DEC);
    writeCommand(SSD1306_SET_COM_PINS);
    writeCommand(0x12);
    writeCommand(SSD1306_SET_CONTRAST_CONTROL);
    writeCommand(0xCF);
    writeCommand(SSD1306_SET_PRECHARGE_PERIOD);
    writeCommand(0xF1);
    writeCommand(SSD1306_SET_VCOM_DESELECT);
    writeCommand(0x40);
    writeCommand(SSD1306_DISPLAY_ALL_ON_RESUME);
    writeCommand(SSD1306_NORMAL_DISPLAY);
    writeCommand(SSD1306_DISPLAY_ON);

    clear();
}

void oled::displayString(char *ptr){
    while(*ptr)
        displayChar(*ptr++);
}

void oled::displayNumber(uint8_t v_numericSystem_u8, uint32_t v_number_u32, uint8_t v_numOfDigitsToDisplay_u8)
{
    uint8_t i=0,a[10];

    if(C_BINARY_U8 == v_numericSystem_u8)
    {
        while(v_numOfDigitsToDisplay_u8!=0)
        {
            i = util_GetBitStatus(v_number_u32,(v_numOfDigitsToDisplay_u8-1));
            displayChar(util_Dec2Ascii(i));
            v_numOfDigitsToDisplay_u8--;
        }
    }
    else if(v_number_u32==0)
    {
        for(i=0;((i<v_numOfDigitsToDisplay_u8) && (i<C_MaxDigitsToDisplay_U8));i++)
            displayChar('0');
    }
    else
    {
        for(i=0;i<v_numOfDigitsToDisplay_u8;i++)
        {
            if(v_number_u32!=0)
            {
                a[i]=util_GetMod32(v_number_u32,v_numericSystem_u8);
                v_number_u32=v_number_u32/v_numericSystem_u8;
            }
            else if( (v_numOfDigitsToDisplay_u8 == C_DisplayDefaultDigits_U8) ||
                    (v_numOfDigitsToDisplay_u8 > C_MaxDigitsToDisplay_U8))
            {
                break;
            }
            else
            {
                a[i]=0;
            }
        }

        while(i!=0)
        {
            displayChar(util_Hex2Ascii(a[i-1]));
            i--;
        }
    }
}

void oled::printf(const char *argList, ...){
    const char *ptr;
    va_list argp;
    uint16_t v_num_s16;
    uint32_t v_num_s32;
    uint16_t v_num_u16;
    uint32_t v_num_u32;
    char *str;
    char  ch;
    uint8_t v_numOfDigitsToDisp_u8;

    va_start(argp, argList);

    for(ptr = argList; *ptr != '\0'; ptr++)
    {
        ch= *ptr;
        if(ch == '%')
        {
            ptr++;
            ch = *ptr;
            if((ch>=0x30) && (ch<=0x39))
            {
                v_numOfDigitsToDisp_u8 = 0;
                while((ch>=0x30) && (ch<=0x39))
                {
                    v_numOfDigitsToDisp_u8 = (v_numOfDigitsToDisp_u8 * 10) + (ch-0x30);
                    ptr++;
                    ch = *ptr;
                }
            }
            else
            {
                v_numOfDigitsToDisp_u8 = C_MaxDigitsToDisplayUsingPrintf_U8;
            }


            switch(ch)
            {
            case 'C':
            case 'c':
                ch = va_arg(argp, int);
                displayChar(ch);
                break;

            case 'd':
                v_num_s16 = va_arg(argp, int);
                if(v_num_s16<0)
                {
                    v_num_s16 = -v_num_s16;
                    displayChar('-');
                }
                displayNumber(C_DECIMAL_U8,v_num_s16,v_numOfDigitsToDisp_u8);
                break;

            case 'D':
                v_num_s32 = va_arg(argp, uint32_t);
                if(v_num_s32<0)
                {
                    v_num_s32 = -v_num_s32;
                    displayChar('-');
                }
                displayNumber(C_DECIMAL_U8,v_num_s32,v_numOfDigitsToDisp_u8);
                break;


            case 'u':
                v_num_u16 = va_arg(argp, int);
                displayNumber(C_DECIMAL_U8,v_num_u16,v_numOfDigitsToDisp_u8);
                break;

            case 'U':
                v_num_u32 = va_arg(argp, uint32_t);
                displayNumber(C_DECIMAL_U8,v_num_u32,v_numOfDigitsToDisp_u8);
                break;

            case 'x':
                v_num_u16 = va_arg(argp, int);
                displayNumber(C_HEX_U8,v_num_u16,v_numOfDigitsToDisp_u8);
                break;

            case 'X':
                v_num_u32 = va_arg(argp, uint32_t);
                displayNumber(C_HEX_U8,v_num_u32,v_numOfDigitsToDisp_u8);
                break;


            case 'b':
                v_num_u16 = va_arg(argp, int);
                if(v_numOfDigitsToDisp_u8 == C_MaxDigitsToDisplayUsingPrintf_U8)
                    v_numOfDigitsToDisp_u8 = 16;
                displayNumber(C_BINARY_U8,v_num_u16,v_numOfDigitsToDisp_u8);
                break;

            case 'B':
                v_num_u32 = va_arg(argp, uint32_t);
                if(v_numOfDigitsToDisp_u8 == C_MaxDigitsToDisplayUsingPrintf_U8)
                    v_numOfDigitsToDisp_u8 = 16;
                displayNumber(C_BINARY_U8,v_num_u32,v_numOfDigitsToDisp_u8);
                break;


            case 'F':
            case 'f':
                break;


            case 'S':
            case 's':
                str = va_arg(argp, char *);
                displayString(str);
                break;

            case '%':
                displayChar('%');
                break;
            }
        }
        else
        {
            displayChar(ch);
        }
    }
    va_end(argp);
}
