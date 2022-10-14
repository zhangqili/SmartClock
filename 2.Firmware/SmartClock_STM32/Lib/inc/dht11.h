#ifndef __DHT11_H__
#define __DHT11_H__

#include "main.h"

#ifndef _HUGE_ENUF
    #define _HUGE_ENUF  1e+300  // _HUGE_ENUF*_HUGE_ENUF must overflow
#endif

#define NAN        ((float)(INFINITY * 0.0F))
#define INFINITY   ((float)(_HUGE_ENUF * _HUGE_ENUF))

#define DHT11_DQ_OUT_HIGH HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_SET)
#define DHT11_DQ_OUT_LOW 	HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_RESET)

#define DHT11_DQ_IN	 HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)


extern float temperature;
extern float humidity;

uint8_t DHT11_Init(void);
uint8_t DHT11_Read_Data(uint16_t *temp,uint16_t *humi);
uint8_t DHT11_Read_FloatData(float *temp,float *humi);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Check(void);
void DHT11_Rst(void);

#endif
