#ifndef __USER_UART_H
#define __USER_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#define Rec_Long  1
#define Max_Rec_Long  64

extern unsigned char UART1_Rx_Buf[Max_Rec_Long];
extern unsigned char UART1_Rx_flg ;
extern unsigned int  UART1_Rx_cnt ;
extern unsigned char UART1_temp[Rec_Long];
void UART1_Rx_Clear(void);

extern unsigned char UART2_Rx_Buf[Max_Rec_Long];
extern unsigned char UART2_Rx_flg ;
extern unsigned int  UART2_Rx_cnt ;
extern unsigned char UART2_temp[Rec_Long];
void UART2_Rx_Clear(void);

#ifdef __cplusplus
}
#endif

#endif
