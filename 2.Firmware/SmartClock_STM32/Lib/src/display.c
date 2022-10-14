#include "display.h"
#include "dht11.h"
#include "time.h"
#include "stdio.h"
#include "usart.h"
#include "lock.h"
#include "main.h"
#include "rain.h"
#include "rtc.h"

u8g2_t u8g2;
uint8_t wCode = 99;
int8_t outdoorTemperature = 99;
uint8_t tstr[5];
uint8_t hstr[5];
uint8_t otstr[5];
uint8_t timestr[10];
uint8_t datestr[12];
uint8_t alarmstr[10];
uint8_t unlockstr[17];
RTC_TimeTypeDef tempTime;
RTC_DateTypeDef tempDate;

const unsigned char col0[] U8X8_PROGMEM = {0x00, 0x7e, 0x00, 0x80, 0xff, 0x01, 0xe0, 0xff, 0x07, 0xf0, 0xff, 0x0f, 0xf8, 0xff, 0x1f, 0xfc, 0xff, 0x3f, 0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x3f, 0xfc, 0xff, 0x3f, 0xf8, 0xff, 0x1f, 0xf0, 0xff, 0x0f, 0xe0, 0xff, 0x07, 0x80, 0xff, 0x01, 0x00, 0x7e, 0x00};
const unsigned char col1[] U8X8_PROGMEM = {0x00, 0x03, 0x00, 0xc0, 0x01, 0x00, 0xf0, 0x01, 0x00, 0xf8, 0x01, 0x00, 0xf8, 0x01, 0x00, 0xfc, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xff, 0x01, 0x00, 0xff, 0x01, 0x00, 0xff, 0x03, 0x00, 0xff, 0x07, 0x00, 0xff, 0x07, 0x00, 0xff, 0x1f, 0x00, 0xff, 0x3f, 0x80, 0xfe, 0xff, 0xff, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x7f, 0xfc, 0xff, 0x3f, 0xf8, 0xff, 0x3f, 0xf0, 0xff, 0x1f, 0xe0, 0xff, 0x07, 0x80, 0xff, 0x03, 0x00, 0xfe, 0x00};
const unsigned char col2[] U8X8_PROGMEM = {0x00, 0x7e, 0x00, 0x80, 0xff, 0x01, 0xe0, 0xff, 0x07, 0xf0, 0xff, 0x0f, 0xf8, 0xff, 0x1f, 0xfc, 0xff, 0x3f, 0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x3f, 0xfc, 0xff, 0x3f, 0xf8, 0xff, 0x1f, 0xf0, 0xff, 0x0f, 0xe0, 0xff, 0x07, 0x80, 0xff, 0x01, 0x00, 0x7e, 0x00};
const unsigned char col3[] U8X8_PROGMEM = {0x00, 0x03, 0x00, 0xc0, 0x01, 0x00, 0xf0, 0x01, 0x00, 0xf8, 0x01, 0x00, 0xf8, 0x01, 0x00, 0xfc, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xfe, 0x01, 0x00, 0xff, 0x01, 0x00, 0xff, 0x01, 0x00, 0xff, 0x03, 0x00, 0xff, 0x07, 0x00, 0xff, 0x07, 0x00, 0xff, 0x1f, 0x00, 0xff, 0x3f, 0x80, 0xfe, 0xff, 0xff, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x7f, 0xfc, 0xff, 0x3f, 0xf8, 0xff, 0x3f, 0xf0, 0xff, 0x1f, 0xe0, 0xff, 0x07, 0x80, 0xff, 0x03, 0x00, 0xfe, 0x00};
const unsigned char col4[] U8X8_PROGMEM = {0x00, 0xf0, 0x01, 0x00, 0xf8, 0x03, 0x00, 0xfc, 0x07, 0xc0, 0xfd, 0x0f, 0xe0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x3f, 0xfc, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x3f, 0xfc, 0xff, 0x1f};
const unsigned char col5[] U8X8_PROGMEM = {0x00, 0x00, 0x0f, 0x00, 0x80, 0x1f, 0x00, 0xc0, 0x3f, 0x00, 0xf0, 0x3f, 0x00, 0xf8, 0x7f, 0x00, 0xfc, 0x7f, 0xe0, 0xfe, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0x7f, 0xfc, 0xff, 0x7f, 0xfe, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x1f, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0x1f, 0xfc, 0xff, 0x0f, 0xf8, 0xff, 0x07};
const unsigned char col6[] U8X8_PROGMEM = {0x00, 0x80, 0x00, 0x00, 0xc0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xff, 0x01, 0xc0, 0xff, 0x01, 0xe0, 0xff, 0x07, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0x7f, 0xf8, 0xff, 0x7f, 0xf8, 0xff, 0x3f, 0xf8, 0xff, 0x1f, 0xfc, 0xff, 0x1f, 0xfe, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x1f, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0x0f, 0xfc, 0xff, 0x07};
const unsigned char col7[] U8X8_PROGMEM = {0x00, 0x00, 0x0f, 0x00, 0x80, 0x1f, 0x00, 0xc0, 0x3f, 0x00, 0xf0, 0x3f, 0x00, 0xf8, 0x7f, 0x00, 0xfc, 0x7f, 0xe0, 0xfe, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0x7f, 0xfc, 0xff, 0x7f, 0xfe, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x1f, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0x1f, 0xfc, 0xff, 0x0f, 0xf8, 0xff, 0x07};
const unsigned char col8[] U8X8_PROGMEM = {0x00, 0x80, 0x00, 0x00, 0xc0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xff, 0x01, 0xc0, 0xff, 0x01, 0xe0, 0xff, 0x07, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0x7f, 0xf8, 0xff, 0x7f, 0xf8, 0xff, 0x3f, 0xf8, 0xff, 0x1f, 0xfc, 0xff, 0x1f, 0xfe, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x1f, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0x0f, 0xfc, 0xff, 0x07};
const unsigned char col9[] U8X8_PROGMEM = {0x00, 0xe0, 0x01, 0x00, 0xf0, 0x03, 0x00, 0xf8, 0x07, 0x80, 0xfc, 0x07, 0xe0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x3f, 0xf8, 0xff, 0x1f};
const unsigned char col10[] U8X8_PROGMEM = {0x00, 0x00, 0x0f, 0x00, 0xc0, 0x1f, 0x00, 0xf0, 0x3f, 0x00, 0xfc, 0x7f, 0x40, 0xfe, 0xff, 0xf0, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xfc, 0xff, 0x7f, 0xfe, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0x1f, 0xfc, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x04, 0x01, 0x10, 0x04, 0x01, 0x18, 0x86, 0x01, 0x08, 0x82, 0x00, 0x00, 0x00, 0x00};
const unsigned char col11[] U8X8_PROGMEM = {0x00, 0xf0, 0x01, 0x00, 0xf8, 0x03, 0xc0, 0xfc, 0x07, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x3f, 0xf8, 0xff, 0x1f, 0x00, 0x3c, 0x00, 0x00, 0x1c, 0x00, 0x10, 0x1e, 0x04, 0x10, 0x1e, 0x04, 0x18, 0x0c, 0x06, 0x08, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char col12[] U8X8_PROGMEM = {0x00, 0xf0, 0x01, 0x00, 0xf8, 0x03, 0xc0, 0xfc, 0x07, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x3f, 0xf8, 0xff, 0x1f, 0x00, 0x3c, 0x00, 0x00, 0x1c, 0x00, 0x10, 0x1e, 0x04, 0x10, 0x1e, 0x04, 0x18, 0x0c, 0x06, 0x08, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char col13[] U8X8_PROGMEM = {0x00, 0xf0, 0x01, 0x00, 0xf8, 0x03, 0xc0, 0xfc, 0x07, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x08, 0x02, 0x20, 0x08, 0x02, 0x30, 0x0c, 0x03, 0x10, 0x04, 0x01, 0x00, 0x00, 0x00};
const unsigned char col14[] U8X8_PROGMEM = {0x00, 0xf0, 0x01, 0x00, 0xf8, 0x03, 0xc0, 0xfc, 0x07, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x22, 0x02, 0x10, 0x33, 0x02, 0x18, 0x11, 0x03, 0x08, 0x11, 0x01, 0x00, 0x00, 0x00};
const unsigned char col15[] U8X8_PROGMEM = {0x00, 0xc0, 0x00, 0x00, 0xf0, 0x03, 0x00, 0xfc, 0x07, 0xe0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xfc, 0xff, 0x7f, 0xfe, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x08, 0xcc, 0xcc, 0x0c, 0x44, 0x44, 0x04, 0x44, 0x44, 0x04, 0x00, 0x00, 0x00};
const unsigned char col16[] U8X8_PROGMEM = {0x00, 0xf0, 0x01, 0x00, 0xf8, 0x03, 0xc0, 0xfc, 0x07, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0xfc, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0x3f, 0xf8, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0xa6, 0x0d, 0x48, 0x92, 0x04, 0x48, 0x92, 0x04, 0x6c, 0xdb, 0x06, 0x24, 0x49, 0x02, 0x00, 0x00, 0x00};
const unsigned char col17[] U8X8_PROGMEM = {0x00, 0xe0, 0x01, 0x00, 0xf8, 0x03, 0xc0, 0xfd, 0x07, 0xe0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x1f, 0xf8, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x3f, 0xf0, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x08, 0x04, 0x40, 0xa1, 0x00, 0x09, 0x84, 0x12, 0x00, 0x00, 0x00};
const unsigned char col18[] U8X8_PROGMEM = {0x00, 0xe0, 0x01, 0x00, 0xf8, 0x03, 0xc0, 0xfd, 0x07, 0xe0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x1f, 0xf8, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x3f, 0xf0, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x08, 0x04, 0x40, 0xa1, 0x00, 0x09, 0x84, 0x12, 0x00, 0x00, 0x00};
const unsigned char col99[] U8X8_PROGMEM = {0x81, 0x20, 0x08, 0x83, 0x20, 0x08, 0x83, 0x20, 0x0c, 0x83, 0x20, 0x0c, 0x87, 0x20, 0x1c, 0x87, 0x30, 0x1c, 0x87, 0x10, 0x14, 0x85, 0x10, 0x16, 0x8d, 0x10, 0x12, 0x89, 0x10, 0x32, 0x89, 0x10, 0x22, 0x99, 0x08, 0x22, 0x99, 0x08, 0x23, 0x91, 0x08, 0x23, 0xb1, 0x08, 0x73, 0xb1, 0x08, 0x7f, 0xe1, 0x0c, 0x7f, 0xe1, 0x84, 0x61, 0xe1, 0x84, 0x41, 0xc1, 0x84, 0xc0, 0xc1, 0x84, 0xc0, 0xc1, 0x84, 0x80, 0x81, 0x80, 0x80, 0x00, 0x00, 0x80};

const unsigned char networkConnectedSign[] U8X8_PROGMEM = {0x80, 0x1f, 0x00, 0xe0, 0x79, 0x00, 0xb8, 0x90, 0x01, 0xcc, 0x30, 0x03, 0x44, 0x20, 0x02, 0x66, 0x60, 0x04, 0xfe, 0xff, 0x07, 0xff, 0xff, 0x0f, 0x21, 0x40, 0x08, 0x21, 0x40, 0x00, 0x21, 0x40, 0x00, 0x21, 0x40, 0x00, 0xff, 0x73, 0x00, 0xfe, 0x33, 0x0c, 0x66, 0x00, 0x06, 0x44, 0x00, 0x03, 0x4c, 0x84, 0x01, 0x18, 0xc8, 0x00, 0xe0, 0x70, 0x00, 0x80, 0x21, 0x00};
const unsigned char degree[] U8X8_PROGMEM = {0x07, 0x05, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char cent[] U8X8_PROGMEM = {0x47, 0x25, 0x27, 0x10, 0x10, 0x08, 0x04, 0x04, 0x72, 0x52, 0x71};

void GenerateString()
{
}

void RefreshScreen()
{
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFontMode(&u8g2, 1);
    u8g2_SetFontDirection(&u8g2, 0);
    if (LOCK)
    {
        u8g2_SetFont(&u8g2, u8g2_font_freedoomr10_tu);
        u8g2_DrawStr(&u8g2, 0, 13, "TOUCH SCANNER");
        u8g2_DrawStr(&u8g2, 0, 25, "TO UNLOCK");
        if (LOCKStatus)
            u8g2_DrawStr(&u8g2, 0, 37, "ERROR");
        sprintf(unlockstr, "%d TIMES REMAIN", 5 - attempts);
        u8g2_DrawStr(&u8g2, 0, 50, unlockstr);
    }
    else
    {
        // u8g2_SetFont(&u8g2, u8g2_font_5x8_tf);
        // DisplayLEDStatus();
        // u8g2_SetFont(&u8g2, u8g2_font_freedoomr10_tu);
        // DisplayOutdoorTemperature();
        // DisplayIndoorTemperature();
        // DisplayIndoorHumidity();
        // DisplayWeatherStatus(&u8g2, wCode);
        // if (RAIN)
        //{
        //     u8g2_DrawStr(&u8g2, 80, 13, "RAIN");
        // }
    }
    // DisplayTime();
    u8g2_SendBuffer(&u8g2);
}

void RefreshLCD()
{
	HAL_RTC_GetTime(&hrtc,&tempTime,FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&tempDate,FORMAT_BIN);
    sprintf(timestr, "%02d:%02d:%02d", tempTime.Hours, tempTime.Minutes, tempTime.Seconds);
    sprintf(datestr, "%04d/%02d/%02d", tempDate.Year+2000, tempDate.Month, tempDate.Date);
    LCD_ShowString(0,  72*0, 36*10, 72, 72, datestr);
    LCD_ShowString(0,  72*1, 36*8, 72, 72, timestr);
    switch (tempTime.Seconds % 3)
    {
    case 0:
        sprintf(tstr, "%2.1f", temperature);
        LCD_ShowString(0, 72*2, 180, 72, 72, tstr);
        break;
    case 1:
        sprintf(hstr, "%2.1f", humidity);
        LCD_ShowString(0, 72*3, 180, 72, 72, hstr);
        break;
    case 2:
        sprintf(otstr, "%d.0", outdoorTemperature);
        LCD_ShowString(0, 72*4, 180, 72, 72, otstr);
        break;
    }
}

void LCDInitDisplay()
{
	HAL_RTC_GetTime(&hrtc,&tempTime,FORMAT_BIN);
    sprintf(timestr, "%02d:%02d:%02d", tempTime.Hours, tempTime.Minutes, tempTime.Seconds);
    LCD_ShowString(0, 0, 36*8, 72, 72, timestr);
    sprintf(tstr, "%2.1f", temperature);
    sprintf(hstr, "%2.1f", humidity);
    sprintf(otstr, "%d.0", outdoorTemperature);
    LCD_DrawXBMP(36*4,72,72,72,(uint8_t*)centigrade,0);
    LCD_DrawXBMP(36*4,72*2,72,72,(uint8_t*)percentSign,0);
    LCD_DrawXBMP(36*4,72*3,72,72,(uint8_t*)centigrade,0);
    LCD_ShowString(0, 72, 180, 72, 72, tstr);
    LCD_ShowString(0, 144, 180, 72, 72, hstr);
    LCD_ShowString(0, 216, 180, 72, 72, otstr);
}

void DisplayWeatherStatus(u8g2_t *u, uint8_t x)
{
    switch (x)
    {
    case 0:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col0);
        break;
    case 1:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col1);
        break;
    case 2:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col2);
        break;
    case 3:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col3);
        break;
    case 4:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col4);
        break;
    case 5:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col5);
        break;
    case 6:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col6);
        break;
    case 7:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col7);
        break;
    case 8:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col8);
        break;
    case 9:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col9);
        break;
    case 10:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col10);
        break;
    case 11:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col11);
        break;
    case 12:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col12);
        break;
    case 13:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col13);
        break;
    case 14:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col14);
        break;
    case 15:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col15);
        break;
    case 16:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col16);
        break;
    case 17:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col17);
        break;
    case 18:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col18);
        break;
    default:
        u8g2_DrawXBMP(u, weatherIconX, weatherIconY, weatherIconW, weatherIconH, col99);
        break;
    }
}
