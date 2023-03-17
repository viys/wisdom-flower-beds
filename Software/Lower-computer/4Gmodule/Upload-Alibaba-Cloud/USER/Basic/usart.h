#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x.h"
#include "stdio.h"//��Ҫ�ض���printf����
#include "tim6.h"//��Ҫ�õ�Tim6�ж�

#define Simulation_BaudRate 57600 //Proteus 8.9��������������������Ϊ57600����˸�ֵΪ����ʱʹ�� 
#define General_BaudRate 115200 //һ���ڿ�������ʹ�õĲ�����

#define USART1_REC_LEN  			200  	//����USART1�������ֽ���

#define To_Computer Usart2_Send_String

extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA;         		//����״̬���	


//��������
void USART1_Config(void);
int fputc(int ch,FILE *fp);

void USART2_Config(void);
void Usart2_Send_String(uint8_t * str);


#endif