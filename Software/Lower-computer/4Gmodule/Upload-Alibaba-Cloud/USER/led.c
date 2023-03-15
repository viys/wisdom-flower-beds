#include "led.h"


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_LED,ENABLE);//开时钟
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin=LED1|LED2;//端口
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//速度
	
	GPIO_Init(LED,&GPIO_InitStructure);
	

}

