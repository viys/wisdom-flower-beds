#ifndef __DRV_USART_H_
#define __DRV_USART_H_


#include "gd32f10x.h"
#include "stdio.h"
#include "app.h"

extern rt_device_t u1_dev;
extern rt_device_t u2_dev;
extern rt_uint8_t USART2_RX_BUF[64];
extern rt_uint16_t rs485_data_len;



void usart0_init(uint32_t brr);
void Debug_printf(char *fmt,...);

void usart1_init(uint32_t brr);
void USART1_Send_Data(uint8_t *data,uint16_t len);

void usart2_init(uint32_t brr);
void USART2_Send_Data(uint8_t *data,uint16_t len);


#define DEBUG_USART	USART0

#endif
