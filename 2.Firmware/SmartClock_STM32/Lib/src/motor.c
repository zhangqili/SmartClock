#include "motor.h"
#include "main.h"

/*---------------------------------���ģ���뵥Ƭ����������---------------------------------------------------*/
// ��ת
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
// ��ת
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
//ֹͣ
void MotorStop(void) 
{  
	HAL_GPIO_WritePin(GPIOE,MOTOR4_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,MOTOR3_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,MOTOR2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,MOTOR1_Pin,GPIO_PIN_RESET);
}

//���Ƶ����ת���Ƿ�תĳ���Ƕ�
//direction����1Ϊ��ת��0Ϊ��ת
//angle�Ƕȣ���Ϊ0-360����ʵ������
void Motor_Ctrl_Direction_Angle(int direction, int angle)
{
	uint16_t i;

	if(direction == 1)
	{								
		for(i = 0; i < 64*angle/45; i++) 
		{
			/* ��ת */
			Motor_Forward_Ration();
		}
		MotorStop();//ֹͣ
  	}
	else
	{
		for(i = 0; i < 64*angle/45; i++) 
		{
			/* ��ת */
			Motor_Reverse_Rotation(); 
		}
		 MotorStop();//ֹͣ
	}
}
