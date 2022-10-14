#include "oled.h"
#include "lcd.h"

#define weatherIconX 0
#define weatherIconY 0
#define weatherIconW 24
#define weatherIconH 24

#define DisplayLEDStatus()                          \
    u8g2_DrawStr(&u8g2, 113, 56, "LED");            \
    if (HAL_GPIO_ReadPin(LED1_GPIO_Port, LED1_Pin)) \
        u8g2_DrawStr(&u8g2, 113, 64, "OFF");        \
    else                                            \
        u8g2_DrawStr(&u8g2, 113, 64, "ON");

#define DisplayOutdoorTemperature()\
	u8g2_DrawStr(&u8g2, 25, 12, otstr);\
    u8g2_DrawXBMP(&u8g2, 54, 0, 3 , 11,degree);\
    u8g2_DrawStr(&u8g2, 58, 12, "C");
	
#define DisplayIndoorTemperature()                                \
    u8g2_DrawStr(&u8g2, 25, 25, tstr);                            \
    u8g2_DrawXBMP(&u8g2, 54, 13, 3, 11, degree);                  \
    u8g2_DrawStr(&u8g2, 58, 25, "C");

#define DisplayIndoorHumidity()                             \
    u8g2_DrawStr(&u8g2, 67, 25, hstr);                      \
    u8g2_DrawXBMP(&u8g2, 95, 13, 7, 11, cent);

#define DisplayNetworkStatus()

#define DisplayTime()                                                                                \
    u8g2_SetFont(&u8g2, u8g2_font_fub20_tn);                                                         \
    u8g2_DrawStr(&u8g2, 0, 64, timestr);


//#define RefreshScreen()                           \
//    u8g2_ClearBuffer(&u8g2);                      \
//    u8g2_SetFontMode(&u8g2, 1);                   \
//    u8g2_SetFontDirection(&u8g2, 0);              \
//    DisplayTime();                                \
//    u8g2_SendBuffer(&u8g2);

extern u8g2_t u8g2;
extern uint8_t wCode;
extern int8_t outdoorTemperature;
extern uint8_t tstr[5];
extern uint8_t hstr[5];
extern uint8_t otstr[5];
extern uint8_t timestr[10];
extern uint8_t alarmstr[10];
extern RTC_TimeTypeDef tempTime;

extern const unsigned char networkConnectedSign[];
extern const unsigned char degree[];
extern const unsigned char cent[];

void GenerateString();
void RefreshScreen();
void RefreshLCD();
void LCDInitDisplayx();
void DisplayWeatherStatus(u8g2_t *u8g2, uint8_t x);