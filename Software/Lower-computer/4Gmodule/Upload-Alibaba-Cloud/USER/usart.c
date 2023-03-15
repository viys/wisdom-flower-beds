#include "usart.h"

u8 USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

u16 USART1_RX_STA=0;       //����״̬���	

//���ڳ�ʼ������
void USART1_Config(void)
{
//1.����GPIO��������������������������������������������
	//�ٿ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//������GPIOģʽ
	GPIO_InitTypeDef GPIO_InitStructure;
		//PA9�����ͣ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//�۳�ʼ���Ĵ�����PA9��
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		//PA10�����գ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	//�ܳ�ʼ���Ĵ�����PA10��
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
//����NVIC
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 
	
	
//2.���ô��ڡ�������������������������������������������
	//�ٿ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	//�����ô���ģʽ
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = General_BaudRate;//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//���÷��ͺͽ��գ�ȫ˫����
	USART_InitStructure.USART_Parity = USART_Parity_No;//��ʹ����żУ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1��ֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//һ֡�д�������8������λ
	//�۳�ʼ���Ĵ���
	USART_Init(USART1,&USART_InitStructure);
	//���ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//��ʹ�ܴ���
	USART_Cmd(USART1,ENABLE);
	
	


}

//3.���ݷ��ͺ����ͽ��պ����ı�д������������������������
  //���ض���printf����
int fputc(int ch,FILE *fp)
{
	while((USART1 -> SR & ( 1 << 6 )) == 0);//���SR�Ĵ����еĵ���λTCΪ0����ʾ����δ������ɣ����һֱ�ڴ˵ȴ�
	USART1 -> DR = ch;//���������ɣ�������ݷ���DR�Ĵ�����
	return ch;
}


//����1�жϺ���
//���ղ���������--��ʱ���ж�
void USART1_IRQHandler(void)
{
	u8 Res;//����һ���������洢���յ���ֵ
	if(USART1->SR & (1 << 5))
	{
		Res = USART1->DR;//��ȡ����������
		if(!(USART1_RX_STA & (1 << 15))) //����־λ���һλΪ0
		{
			if(USART1_RX_STA < USART1_REC_LEN)  //�жϱ�־λ�Ƿ񳬳�����޶�  (��ֹ���)
			{
				TIM6->CNT = 0;        //��ʱ�� ����������
				if(USART1_RX_STA == 0)      //��һ�����ݵĵ�һ�����ݵ���ʱ
				{
					TIM6->CR1 |= 1 << 0;      //������10ms�ж�   
				}
				USART1_RX_BUF[USART1_RX_STA++] = Res;   //�����ݴ���ڽ����������� 
			}
			else    
			{
				USART1_RX_STA |= 1 << 15;   //�����־λ��������޶� ����������(������Ҫ�Ƿ�ֹ���)
			}
		}
	}
	USART1->SR &= ~(1 << 5);//����жϱ�־λ
}





//***************************************USART2**************************************//

void USART2_Config(void)
{
//1.����GPIO��������������������������������������������
	//�ٿ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//������GPIOģʽ
	GPIO_InitTypeDef GPIO_InitStructure;
		//PA2�����ͣ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//�۳�ʼ���Ĵ�����PA9��
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		//PA3�����գ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	//�ܳ�ʼ���Ĵ�����PA10��
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//�ж�
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 
	
	
//2.���ô��ڡ�������������������������������������������
	//�ٿ�ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//�����ô���ģʽ
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = General_BaudRate;//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//���÷��ͺͽ��գ�ȫ˫����
	USART_InitStructure.USART_Parity = USART_Parity_No;//��ʹ����żУ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1��ֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//һ֡�д�������8������λ
	//�۳�ʼ���Ĵ���
	USART_Init(USART2,&USART_InitStructure);
	//��ʹ�ܴ���
	USART_Cmd(USART2,ENABLE);

}


//����2�����ַ�������
void Usart2_Send_String(uint8_t * str)
{
	while( *str != '\0')
	{
		USART2->DR = *str;
		while(!(USART2->SR & (1 << 7)));//�ȴ��������
		str++;
	}

}

//����2�жϺ���
void USART2_IRQHandler(void)
{
	if(USART2->SR & (1 << 5))
	{
		
		USART2->SR &= ~(1 << 5); //��ձ�־λ
	
	}

}

