#ifndef __BC26_H_
#define __BC26_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "usart.h"
#include "delay.h"
#include "mqtt.h"


//º¯ÊıÉùÃ÷
u8 BC26_Init(void);
u8 BC26_Send_Check(u8 *cmd,u8 *ack,u16 time);
u8 BC26_Data_Send(u8 *cmd,u8 *ack);


#endif
