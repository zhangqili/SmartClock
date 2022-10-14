#include "command.h"
#include "user_uart.h"
#include "stdio.h"
#include "string.h"
#include <time.h>
#include "usart.h"
#include "display.h"
#include "main.h"
#include "rtc.h"

uint8_t Cmd_Rx_Flg;
uint8_t cmd[10];
uint32_t tempData;
time_t rxTime;
struct tm* ptm;
RTC_TimeTypeDef rtcTime;
RTC_DateTypeDef rtcDate;

void ParseCommand(UART_HandleTypeDef *huart)
{
	uint8_t i=0;
	if (huart->Instance == USART1)
	{
		sscanf(UART1_Rx_Buf, "%s %d", cmd, &tempData);

		if (!strcmp(cmd, "SETTIME"))
		{
			rxTime = tempData;
			ptm = localtime(&rxTime);
			rtcTime.Hours = ptm->tm_hour;
			rtcTime.Minutes = ptm->tm_min;
			rtcTime.Seconds = ptm->tm_sec;
			HAL_RTC_SetTime(&hrtc,&rtcTime,RTC_FORMAT_BIN);
			rtcDate.Year = ptm->tm_year%100;
			rtcDate.Month = ptm->tm_mon+1;
			rtcDate.Date = ptm->tm_mday;
			rtcDate.WeekDay = ptm->tm_wday;
			HAL_RTC_SetDate(&hrtc,&rtcDate,RTC_FORMAT_BIN);
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
			rxTime = tempData;
			ptm = localtime(&rxTime);
			rtcTime.Hours = ptm->tm_hour;
			rtcTime.Minutes = ptm->tm_min;
			rtcTime.Seconds = ptm->tm_sec;
			HAL_RTC_SetTime(&hrtc,&rtcTime,RTC_FORMAT_BIN);
			rtcDate.Year = ptm->tm_year%100;
			rtcDate.Month = ptm->tm_mon+1;
			rtcDate.Date = ptm->tm_mday;
			rtcDate.WeekDay = ptm->tm_wday;
			HAL_RTC_SetDate(&hrtc,&rtcDate,RTC_FORMAT_BIN);
			i=1;
			printf("%d\n",ptm->tm_year%100);
			printf("%d\n",ptm->tm_mon%100);
			printf("%d\n",ptm->tm_mday%100);
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