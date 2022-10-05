#include "command.h"
#include "user_uart.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "usart.h"
#include "display.h"

uint8_t Cmd_Rx_Flg;
uint8_t cmd[10];
uint32_t tempData;

void ParseCommand(UART_HandleTypeDef *huart)
{
	uint8_t i=0;
	if (huart->Instance == USART1)
	{
		sscanf(UART1_Rx_Buf, "%s %d", cmd, &tempData);

		if (!strcmp(cmd, "SETTIME"))
		{
			CURRENT_TIME.hour = tempData / 3600;
			CURRENT_TIME.minute = tempData / 60 % 60;
			CURRENT_TIME.second = tempData % 60;
			i=1;
		}
		if (!strcmp(cmd, "SETW"))
		{
			wCode = (uint8_t)tempData;
			i=1;
		}
		if (!strcmp(cmd, "SETTEMP"))
		{
			outdoorTemperature = (uint8_t)tempData;
			i=1;
		}
		if (!strcmp(cmd, "SETLED"))
		{
			if((uint8_t)tempData)
				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
			i=1;
		}
		if(i)
			HAL_UART_Transmit(&huart1, "OVER\n", 5, 0xff);

		UART1_Rx_Clear();
	}
	if (huart->Instance == USART2)
	{
		sscanf(UART2_Rx_Buf, "%s %d", cmd, &tempData);
		HAL_UART_Transmit(&huart2, UART2_Rx_Buf, UART2_Rx_cnt, 0xff);

		if (!strcmp(cmd, "SETTIME"))
		{
			CURRENT_TIME.hour = tempData / 3600;
			CURRENT_TIME.minute = tempData / 60 % 60;
			CURRENT_TIME.second = tempData % 60;
			i=1;
		}
		if (!strcmp(cmd, "SETW"))
		{
			wCode = (uint8_t)tempData;
			i=1;
		}
		if (!strcmp(cmd, "SETTEMP"))
			outdoorTemperature = (uint8_t)tempData;
			i=1;
		if (!strcmp(cmd, "SETLED"))
		{
			if((uint8_t)tempData)
				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
			i=1;
		}
		if(i)
			HAL_UART_Transmit(&huart2, "OVER\n", 5, 0xff);
		UART2_Rx_Clear();
	}
}