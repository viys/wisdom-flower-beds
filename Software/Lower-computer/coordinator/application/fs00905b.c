#include "fs00905b.h"

unsigned short FS_Ver = 0;//校验
unsigned char FS_RX_BUF[64];//接收数组
unsigned char FS_RX_FLAG = FS_RX_NO;//接收标志位
unsigned short FS_RX_LEN = 0;//接收长度

Air_Quality air_quality;


/*
函数功能：空气质量传感器数据处理
参数：Data：传入数据 DataLen：数据长度 
返回值：FS_SUCCESS：成功
		FS_LEN_ERROR：长度错误
		FS_FRAME_ERROR：数据帧格式错误
		FS_CHECK_ERROR：校验错误
*/
int FS_DATA_HANDLE(unsigned char *Data,unsigned short DataLen)
{
	
	if(DataLen != 40)return FS_LEN_ERROR;
	
	if((Data[0] != 0x42) || (Data[1] != 0x4d))return FS_FRAME_ERROR;
	
	unsigned short cheak = Data[38] << 8 | Data[39];//计算校验值
	
	FS_Ver -= Data[38] + Data[39];//减去后两位
	
	if(cheak == FS_Ver && (Data[2] << 8 | Data[3]) == 36)//长度位为：数据长度+校验 2×17 + 2 = 36
	{
		
		unsigned short *AirQuality = &air_quality.PM1_0;//取Air_Quality结构体首地址
		
		for(int i = 0;i < 17;i ++)//一共17组数据
		{
			*(AirQuality++) = Data[4 + (i * 2)] << 8 | Data[5 + (i * 2)];//依次赋值
		}
		
	}
	else
	{
		return FS_CHECK_ERROR;
	}
	
	return FS_SUCCESS;

}
/*
函数功能：空气质量传感器串口数据接收
参数：串口的缓存区的值 
返回值：FS_RX_NO：未接收完成
		FS_RX_OK：接收完成
备注：空气质量传感器数据帧固定40字节
*/
int FS_UART_IRQHandler(unsigned char DR_Value)
{
	static unsigned char fs_data_len = 2;//开始接受长度置(跳过固定包头)
	static unsigned short fs_ver = 0;//校验初始为0
	static unsigned char fs_head = 0;//包头高位缓存区
	static unsigned char fs_receive_start = 0;//开始接收标志位
	
	if(fs_receive_start == 1)//当开始接收标志位置1时，开始将数据存入数组
	{
		
		FS_RX_BUF[fs_data_len++] = DR_Value;//存入数组
		
		fs_ver += DR_Value;//校验+
		
		if(fs_data_len == 40)//接收完成
		{
			FS_RX_LEN = 40;//长度标志位
			FS_RX_FLAG = FS_RX_OK;//接收完成标志位置1
			FS_Ver = fs_ver;//校验值
			fs_ver = 0;//标志位复位
			fs_data_len = 2;//标志位复位
			fs_head = 0;//标志位复位
			fs_receive_start = 0;//标志位复位
			return FS_RX_OK;//返回成功
		}
		
		return FS_RX_NO;//返回接收为完成
	}
	
	
	if(DR_Value == 0x42 && fs_receive_start == 0)//当数据为0x42且未开始接收
	{
		fs_head = 0x42;//包头缓存区存入0x42
	}	
	else if(DR_Value == 0x4d && fs_head == 0x42 && fs_receive_start == 0)//当第二个字节为0x4d且包头高位为0x42且为开始接收
	{
		fs_receive_start = 1;//开始接收标志位置1
		fs_head = 0;//包头高位缓存区清0
		FS_RX_BUF[0] = 0x42;//BUF[0] 存入包头高位
		FS_RX_BUF[1] = 0x4d;//BUF[1] 存入包头低位
		fs_ver = 0x42 + 0x4d;//校验把包头加上
	}
	else
		fs_head = 0;//否则包头缓存区置0
	
	
	
	return FS_RX_NO;//返回接收为完成
	
}


/*
函数功能：空气质量传感器打印数据
参数：void
返回值：void
备注：使用串口0打印 用户可更改
*/
void FS_DATA_DEBUG(void)
{
	
	Debug_printf("TSET_INTERRUPT:%d\r\n",TSET_INTERRUPT);
	Debug_printf("PM1.0    :%d ug/m3\r\n",air_quality.PM1_0);
	Debug_printf("PM2.5    :%d ug/m3\r\n",air_quality.PM2_5);
	Debug_printf("PM10     :%d ug/m3\r\n",air_quality.PM10);
	Debug_printf("PM1.0_air:%d ug/m3\r\n",air_quality.PM1_0_air);
	Debug_printf("PM2.0_air:%d ug/m3\r\n",air_quality.PM2_5_air);
	Debug_printf("PM10_air :%d ug/m3\r\n",air_quality.PM10_air);
	Debug_printf("TSP 0.3  :%d\r\n",air_quality.TSP_0_3um);
	Debug_printf("TSP 0.5  :%d\r\n",air_quality.TSP_0_5um);
	Debug_printf("TSP 1.0  :%d\r\n",air_quality.TSP_1_0um);
	Debug_printf("TSP 2.5  :%d\r\n",air_quality.TSP_2_5um);
	Debug_printf("TSP 5.0  :%d\r\n",air_quality.TSP_5_0um);
	Debug_printf("TSP 10   :%d\r\n",air_quality.TSP_10um);
	Debug_printf("TVOC     :%d ug/m3\r\n",air_quality.TVOC);
	Debug_printf("TEM      :%.1f C\r\n",(float)(air_quality.TEM - 450) / 10.0);
	Debug_printf("HUM      :%.1f %%RH\r\n",(float)(air_quality.HUM)/10.0);
	Debug_printf("CO2      :%d ppm\r\n",air_quality.CO2);
	Debug_printf("CH2O     :%d ug/m3\r\n",air_quality.CH2O);

}


