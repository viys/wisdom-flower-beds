#ifndef __FS00905B_H_
#define __FS00905B_H_

#include "gd32f10x.h"
#include "drv_usart.h"

typedef struct
{
	
	unsigned short PM1_0;//PM1.0
	unsigned short PM2_5;//PM2.5
	unsigned short PM10;//PM10
	unsigned short PM1_0_air;//PM1.0(大气环境下)
	unsigned short PM2_5_air;//PM2.5(大气环境下)
	unsigned short PM10_air;//PM10(大气环境下)
	unsigned short TSP_0_3um;//0.1升空气中直径在0.3μm以上的颗粒物个数
	unsigned short TSP_0_5um;//0.1升空气中直径在0.5μm以上的颗粒物个数
	unsigned short TSP_1_0um;//0.1升空气中直径在1.0μm以上的颗粒物个数
	unsigned short TSP_2_5um;//0.1升空气中直径在2.5μm以上的颗粒物个数
	unsigned short TSP_5_0um;//0.1升空气中直径在5.0μm以上的颗粒物个数
	unsigned short TSP_10um;//0.1升空气中直径在10μm以上的颗粒物个数
	unsigned short TVOC;//TVOC浓度值
	unsigned short TEM;//温度
	unsigned short HUM;//湿度
	unsigned short CO2;//二氧化碳浓度
	unsigned short CH2O;//甲醛浓度
	
}Air_Quality;


/*FS传感器串口波特率*/
#define FS_UART_BAUD     9600


/*处理函数返回码*/
#define FS_SUCCESS       0
#define FS_LEN_ERROR     1
#define FS_FRAME_ERROR   2
#define FS_CHECK_ERROR   3

/*接收标志位*/
#define FS_RX_OK         1
#define FS_RX_NO         0



/*函数声明*/
int FS_DATA_HANDLE(unsigned char *Data,unsigned short DataLen);
int FS_UART_IRQHandler(unsigned char DR_Value);
void FS_DATA_DEBUG(void);

/*变量声明*/
extern unsigned short FS_Ver;
extern unsigned char FS_RX_BUF[64];
extern unsigned char FS_RX_FLAG;
extern unsigned short FS_RX_LEN;
extern Air_Quality air_quality;


#endif

