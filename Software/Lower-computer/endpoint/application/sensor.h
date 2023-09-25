#ifndef __SENSOR_H
#define __SENSOR_H

#include <app.h>

/*传感器寄存器地址*/
#define REG_PH    0x0006
#define REG_HUM   0x0012
#define REG_TEM   0x0013
#define REG_EC    0x0015
#define REG_ADD   0x0100
#define REG_BAUD  0x0101

#if RS485_CONTROL_TX_RX

/*RS485 CON控制口*/
#define RS485_CON_CLOCK   RCU_GPIOB
#define RS485_CON_PORT    GPIOB
#define RS485_CON_PIN     GPIO_PIN_1

#define RS485_CON_TX()     gpio_bit_reset(RS485_CON_PORT,RS485_CON_PIN);
#define RS485_CON_RX()     gpio_bit_set(RS485_CON_PORT,RS485_CON_PIN);

#endif

/*传感器可取值*/
typedef enum
{
	TYPE_PH,    //PH
	TYPE_HUM,   //湿度
	TYPE_TEM,   //温度
	TYPE_EC,    //电导率
	TYPE_ADD,   //从机地址
	TYPE_BAUD,  //波特率
	TYPE_FREE,  //任意
}ENUM_TYPETypeDef;

/*DEBUG传感器数值*/
typedef struct
{
	float _PH;
	float _HUM;
	float _TEM;
	unsigned short _EC;
	unsigned short _ADD;
	unsigned short _FREE;
	int _BAUD;
	
}STRUCT_DEBUGTypeDef;




/*变量声明*/
extern STRUCT_DEBUGTypeDef debug_value;


/*函数声明*/

#if RS485_CONTROL_TX_RX
void RS485_CON_Init(void);
void RS485_TIME_OneByte(unsigned int baud);
#endif
bool Sensor_TakeAValue(ENUM_TYPETypeDef enumTYPE,unsigned short REG);
int Sensor_MakeAValue(unsigned short REG,unsigned short value);
bool Modify_Baud_Rate(unsigned int baud);
bool Modify_Address(unsigned short addre);


#endif 

