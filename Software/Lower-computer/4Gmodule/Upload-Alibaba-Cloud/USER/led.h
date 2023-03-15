#ifndef __LED_H_
#define __LED_H_

#include "stm32f10x.h"


void LED_Init();
//void LED_TOGGLE(uint16_t LEDx);


////////��LED�Ľӿڱ任ʱ��ֻ���ڴ��޸�////////
#define LED GPIOC
#define LED1	GPIO_Pin_1	//����IO�ӿ�
#define LED2	GPIO_Pin_2	//����IO�ӿ�
#define RCC_LED RCC_APB2Periph_GPIOC
/////////////////////////////////////////////


#define LED1_ON GPIO_SetBits(LED,LED1);
#define LED1_OFF GPIO_ResetBits(LED,LED1);

#define LED2_ON GPIO_SetBits(LED,LED2);
#define LED2_OFF GPIO_ResetBits(LED,LED2);

#define LED1_TOGGLE GPIO_WriteBit(LED,LED1,(BitAction)(1-GPIO_ReadInputDataBit(LED,LED1))); //LED1��ת
#define LED2_TOGGLE GPIO_WriteBit(LED,LED2,(BitAction)(1-GPIO_ReadInputDataBit(LED,LED2))); //LED2��ת


#endif




