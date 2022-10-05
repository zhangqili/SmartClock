#define RAIN !HAL_GPIO_ReadPin(RAIN_GPIO_Port,RAIN_Pin)

extern unsigned char CURTAIN;

void OpenCurtain(void);
void CloseCurtain(void);