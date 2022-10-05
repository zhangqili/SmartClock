/* Includes ------------------------------------------------------------------*/
#include "user_uart.h"
#include "usart.h"
#include "command.h"

unsigned char UART1_Rx_Buf[Max_Rec_Long] = {0};	// 串口1存储接收数据
unsigned char UART1_Rx_flg = 0; 				// 串口1接收完成标志
unsigned int  UART1_Rx_cnt = 0;					// 串口1接受数据计数器
unsigned char UART1_temp[Rec_Long] = {0};		// 串口1接收数据缓存

unsigned char UART2_Rx_Buf[Max_Rec_Long] = {0};	// 串口1存储接收数据
unsigned char UART2_Rx_flg = 0; 				// 串口1接收完成标志
unsigned int  UART2_Rx_cnt = 0;					// 串口1接受数据计数器
unsigned char UART2_temp[Rec_Long] = {0};		// 串口1接收数据缓存

/**
  * 函数名称：串口中断回调函数
  * 函数功能：调用回调函数的串口
  * 形   参：串口名
  * 返回值 ：无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART1)
    {
        UART1_Rx_Buf[UART1_Rx_cnt] = UART1_temp[0];
        UART1_Rx_cnt++;
        if(0x0a == UART1_temp[0])
        {
            UART1_Rx_flg = 1;
        }
		if(UART1_Rx_cnt >=Max_Rec_Long)
			UART1_Rx_Clear();
        HAL_UART_Receive_IT(&huart1,(uint8_t *)UART1_temp,Rec_Long); // 重新开启串口接收中断
    }
    if(huart->Instance==USART2)
    {
        UART2_Rx_Buf[UART2_Rx_cnt] = UART2_temp[0];
        UART2_Rx_cnt++;
        if(0x0a == UART2_temp[0])
        {
            UART2_Rx_flg = 1;
        }
		if(UART2_Rx_cnt >=Max_Rec_Long)
			UART2_Rx_Clear();
        HAL_UART_Receive_IT(&huart2,(uint8_t *)UART2_temp,Rec_Long); // 重新开启串口接收中断
    }
}

void UART1_Rx_Clear(void)
{
	unsigned char i;
	for (i=0;i<UART1_Rx_cnt;i++)
		UART1_Rx_Buf[i]=0;
	UART1_Rx_flg=0;
	UART1_Rx_cnt=0;
}

void UART2_Rx_Clear(void)
{
	unsigned char i;
	for (i=0;i<UART2_Rx_cnt;i++)
		UART2_Rx_Buf[i]=0;
	UART2_Rx_flg=0;
	UART2_Rx_cnt=0;
}