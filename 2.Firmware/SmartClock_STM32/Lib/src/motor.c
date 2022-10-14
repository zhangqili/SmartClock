#include "motor.h"
#include "main.h"

/*---------------------------------电机模块与单片机连接引脚---------------------------------------------------*/
// 正转
void Motor_Forward_Ration(void)  
{  
	HAL_GPIO_WritePin(GPIOE,MOTOR1_Pin,GPIO_PIN_SET);
	HAL_Delay(3);
	HAL_GPIO_WritePin(GPIOE,MOTOR1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,MOTOR2_Pin,GPIO_PIN_SET);
	HAL_Delay(3);
	HAL_GPIO_WritePin(GPIOE,MOTOR2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,MOTOR3_Pin,GPIO_PIN_SET);
	HAL_Delay(3);
	HAL_GPIO_WritePin(GPIOE,MOTOR3_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,MOTOR4_Pin,GPIO_PIN_SET);
	HAL_Delay(3);
	HAL_GPIO_WritePin(GPIOE,MOTOR4_Pin,GPIO_PIN_RESET);
}
// 反转
void Motor_Reverse_Rotation(void)  
{  
	HAL_GPIO_WritePin(GPIOE,MOTOR4_Pin,GPIO_PIN_SET);
	HAL_Delay(3);
	HAL_GPIO_WritePin(GPIOE,MOTOR4_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,MOTOR3_Pin,GPIO_PIN_SET);
	HAL_Delay(3);
	HAL_GPIO_WritePin(GPIOE,MOTOR3_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,MOTOR2_Pin,GPIO_PIN_SET);
	HAL_Delay(3);
	HAL_GPIO_WritePin(GPIOE,MOTOR2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,MOTOR1_Pin,GPIO_PIN_SET);
	HAL_Delay(3);
	HAL_GPIO_WritePin(GPIOE,MOTOR1_Pin,GPIO_PIN_RESET);
}
//停止
void MotorStop(void) 
{  
	HAL_GPIO_WritePin(GPIOE,MOTOR4_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,MOTOR3_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,MOTOR2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,MOTOR1_Pin,GPIO_PIN_RESET);
}

//控制电机正转还是反转某个角度
//direction方向，1为正转，0为反转
//angle角度，可为0-360具有实际意义
void Motor_Ctrl_Direction_Angle(int direction, int angle)
{
	uint16_t i;

	if(direction == 1)
	{								
		for(i = 0; i < 64*angle/45; i++) 
		{
			/* 正转 */
			Motor_Forward_Ration();
		}
		MotorStop();//停止
  	}
	else
	{
		for(i = 0; i < 64*angle/45; i++) 
		{
			/* 反转 */
			Motor_Reverse_Rotation(); 
		}
		 MotorStop();//停止
	}
}
