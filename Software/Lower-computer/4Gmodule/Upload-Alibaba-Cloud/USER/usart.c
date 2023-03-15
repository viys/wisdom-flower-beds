#include "usart.h"

u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

u16 USART1_RX_STA=0;       //接收状态标记	

//串口初始化函数
void USART1_Config(void)
{
//1.配置GPIO——————————————————————
	//①开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//②配置GPIO模式
	GPIO_InitTypeDef GPIO_InitStructure;
		//PA9（发送）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//③初始化寄存器（PA9）
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		//PA10（接收）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	//④初始化寄存器（PA10）
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
//配置NVIC
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器 
	
	
//2.配置串口——————————————————————
	//①开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	//②配置串口模式
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = General_BaudRate;//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//配置发送和接收（全双工）
	USART_InitStructure.USART_Parity = USART_Parity_No;//不使用奇偶校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1个停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//一帧中传输或接收8个数据位
	//③初始化寄存器
	USART_Init(USART1,&USART_InitStructure);
	//开中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//④使能串口
	USART_Cmd(USART1,ENABLE);
	
	


}

//3.数据发送函数和接收函数的编写————————————
  //①重定向printf函数
int fputc(int ch,FILE *fp)
{
	while((USART1 -> SR & ( 1 << 6 )) == 0);//如果SR寄存器中的第六位TC为0，表示数据未发送完成，则会一直在此等待
	USART1 -> DR = ch;//如果发送完成，则把数据放入DR寄存器中
	return ch;
}


//串口1中断函数
//接收不定长数据--定时器中断
void USART1_IRQHandler(void)
{
	u8 Res;//设置一个变量来存储接收到的值
	if(USART1->SR & (1 << 5))
	{
		Res = USART1->DR;//提取缓存区数据
		if(!(USART1_RX_STA & (1 << 15))) //当标志位最后一位为0
		{
			if(USART1_RX_STA < USART1_REC_LEN)  //判断标志位是否超出最大限度  (防止溢出)
			{
				TIM6->CNT = 0;        //定时器 计数器清零
				if(USART1_RX_STA == 0)      //当一组数据的第一个数据到来时
				{
					TIM6->CR1 |= 1 << 0;      //立马开启10ms中断   
				}
				USART1_RX_BUF[USART1_RX_STA++] = Res;   //把数据存放在接收数组里面 
			}
			else    
			{
				USART1_RX_STA |= 1 << 15;   //如果标志位超出最大限度 让其接收完成(这里主要是防止溢出)
			}
		}
	}
	USART1->SR &= ~(1 << 5);//清空中断标志位
}





//***************************************USART2**************************************//

void USART2_Config(void)
{
//1.配置GPIO——————————————————————
	//①开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//②配置GPIO模式
	GPIO_InitTypeDef GPIO_InitStructure;
		//PA2（发送）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//③初始化寄存器（PA9）
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		//PA3（接收）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	//④初始化寄存器（PA10）
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//中断
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器 
	
	
//2.配置串口——————————————————————
	//①开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//②配置串口模式
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = General_BaudRate;//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//配置发送和接收（全双工）
	USART_InitStructure.USART_Parity = USART_Parity_No;//不使用奇偶校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1个停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//一帧中传输或接收8个数据位
	//③初始化寄存器
	USART_Init(USART2,&USART_InitStructure);
	//④使能串口
	USART_Cmd(USART2,ENABLE);

}


//串口2发送字符串函数
void Usart2_Send_String(uint8_t * str)
{
	while( *str != '\0')
	{
		USART2->DR = *str;
		while(!(USART2->SR & (1 << 7)));//等待发送完成
		str++;
	}

}

//串口2中断函数
void USART2_IRQHandler(void)
{
	if(USART2->SR & (1 << 5))
	{
		
		USART2->SR &= ~(1 << 5); //清空标志位
	
	}

}

