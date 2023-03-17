#ifndef __TIM6_H_
#define __TIM6_H_

#include "stm32f10x.h"
#include "usart.h"

void TIM6_Delay_Init();
void TIM6_Delay_ms(u16 ms);
void TIM6_Delay_us(u16 us);
void TIM6_Inter_Init(u16 psc,u32 arr);



#endif
