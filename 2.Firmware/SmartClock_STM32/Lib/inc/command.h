#include "main.h"

extern uint8_t Cmd_Rx_Flg;
extern uint8_t cmd[10];
extern uint32_t tempData;

void ParseCommand(UART_HandleTypeDef *huart);