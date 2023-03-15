
#include "delay.h"

void delay_us(u32 us)
{
	SysTick->CTRL &=~(1 << 2);//使用外部时钟源
	SysTick->LOAD = (72/8) * us;//计数值
	SysTick->VAL = 0;//计数器清零
	SysTick->CTRL |= 1 << 0;//开计时器
	while(!(SysTick->CTRL & (1 << 16)));
	SysTick->CTRL &= ~(1 << 0);//关计时器
	

}


void delay_ms(u16 ms)//尽量写的数值<=1000  不然可能会出错
{
	SysTick->CTRL &=~(1 << 2);
	SysTick->LOAD = (72/8) * ms * 1000;
	SysTick->VAL = 0;
	SysTick->CTRL |= 1 << 0;
	while(!(SysTick->CTRL & (1 << 16)));
	SysTick->CTRL &= ~(1 << 0);
	

}

void delay_s(u16 s) //如果使用SysTick->LOAD = (72/8) * s * 1000000;  那计数值可能会溢出
{
	while(s-- != 0)
	{
		delay_ms(1000);
	}
	

}

