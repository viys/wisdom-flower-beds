#include "led.h"


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_LED,ENABLE);//��ʱ��
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Pin=LED1|LED2;//�˿�
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ٶ�
	
	GPIO_Init(LED,&GPIO_InitStructure);
	

}

