#ifndef __LED_H_
#define __LED_H_

#include "stm32f10x.h"


void LED_Init();
//void LED_TOGGLE(uint16_t LEDx);


////////当LED的接口变换时，只需在此修改////////
#define LED GPIOC
#define LED1	GPIO_Pin_1	//定义IO接口
#define LED2	GPIO_Pin_2	//定义IO接口
#define RCC_LED RCC_APB2Periph_GPIOC
/////////////////////////////////////////////


#define LED1_ON GPIO_SetBits(LED,LED1);
#define LED1_OFF GPIO_ResetBits(LED,LED1);

#define LED2_ON GPIO_SetBits(LED,LED2);
#define LED2_OFF GPIO_ResetBits(LED,LED2);

#define LED1_TOGGLE GPIO_WriteBit(LED,LED1,(BitAction)(1-GPIO_ReadInputDataBit(LED,LED1))); //LED1翻转
#define LED2_TOGGLE GPIO_WriteBit(LED,LED2,(BitAction)(1-GPIO_ReadInputDataBit(LED,LED2))); //LED2翻转


#endif




