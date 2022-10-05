#include "rain.h"
#include "main.h"

unsigned char CURTAIN = 0;

void OpenCurtain(void)
{
		CURTAIN = 0;
		Motor_Ctrl_Direction_Angle(0,180);
}
void CloseCurtain(void)
{

		CURTAIN = 1;
		Motor_Ctrl_Direction_Angle(1,180);
}