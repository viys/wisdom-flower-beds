#ifndef __DHT11_H
#define __DHT11_H	 
#include "stm32f10x.h"
#include "delay.h"

#define DHT11PORT	GPIOC	//����IO�ӿ�
#define DHT11_IO	GPIO_Pin_0	//����IO�ӿ�


void DHT11_IO_OUT (void);
void DHT11_IO_IN (void);
void DHT11_RST (void);
u8 Dht11_Check(void); 	   
u8 Dht11_ReadBit(void); 
u8 Dht11_ReadByte(void); 
u8 DHT11_Init (void);
u8 DHT11_ReadData(u8 *h,u8 *t);
		 				    
#endif
