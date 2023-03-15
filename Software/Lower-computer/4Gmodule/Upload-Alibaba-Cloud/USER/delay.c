
#include "delay.h"

void delay_us(u32 us)
{
	SysTick->CTRL &=~(1 << 2);//ʹ���ⲿʱ��Դ
	SysTick->LOAD = (72/8) * us;//����ֵ
	SysTick->VAL = 0;//����������
	SysTick->CTRL |= 1 << 0;//����ʱ��
	while(!(SysTick->CTRL & (1 << 16)));
	SysTick->CTRL &= ~(1 << 0);//�ؼ�ʱ��
	

}


void delay_ms(u16 ms)//����д����ֵ<=1000  ��Ȼ���ܻ����
{
	SysTick->CTRL &=~(1 << 2);
	SysTick->LOAD = (72/8) * ms * 1000;
	SysTick->VAL = 0;
	SysTick->CTRL |= 1 << 0;
	while(!(SysTick->CTRL & (1 << 16)));
	SysTick->CTRL &= ~(1 << 0);
	

}

void delay_s(u16 s) //���ʹ��SysTick->LOAD = (72/8) * s * 1000000;  �Ǽ���ֵ���ܻ����
{
	while(s-- != 0)
	{
		delay_ms(1000);
	}
	

}

