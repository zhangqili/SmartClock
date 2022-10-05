#ifndef __DHT11_H__
#define __DHT11_H__

#include "main.h"

#define DHT11_DQ_OUT_HIGH HAL_GPIO_WritePin(GPIOG,GPIO_PIN_11,GPIO_PIN_SET)
#define DHT11_DQ_OUT_LOW 	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_11,GPIO_PIN_RESET)

#define DHT11_DQ_IN	 HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_11)


extern uint16_t temperature;
extern uint16_t humidity;
   	
uint8_t DHT11_Init(void);
uint8_t DHT11_Read_Data(uint16_t *temp,uint16_t *humi);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Check(void);
void DHT11_Rst(void);

#endif
