#include "bc26.h"




/*
函数：发送AT与检查AT函数
参数：cmd：发送给BC26的AT指令  ack：想要得到的回应指令  time：最大重复次数
返回值：SUCCESS：初始化成功  ERROR：初始化失败
备注：
*/

u8 BC26_Send_Check(u8 *cmd,u8 *ack,u16 time)
{
	USART1_RX_STA = 0;//USART1接收标志位清零
	memset(USART1_RX_BUF, 0, 200);//清空USART1接收数组空间
	while(--time)
	{
		printf("%s\r\n",cmd);//发送AT指令
//		if(Send_Flag)delay_ms(100);
		delay_ms(300);//等待周期
		if(USART1_RX_STA & 0x8000)//判断接收是否完成
		{
			if(strstr((const char *)USART1_RX_BUF, (const char *)ack))//比较想要的回应值是否为USART1接收到数据的子串
			{
				To_Computer((u8 *)"BC26回应数据：\r\n");
				To_Computer(USART1_RX_BUF);//给电脑发送USART1接收到的数据
				USART1_RX_STA = 0;
				break;//当接收到有效回应时退出循环
			}
			USART1_RX_STA = 0;
		}
	}		
	if(time == 0)return ERROR;//如果得不到回应 则返回ERROR
	else return SUCCESS;     //否则返回SUCCESS
	
}

/*
函数：给服务器发布消息函数
参数：cmd：发送给BC26的AT指令  ack：想要得到的回应指令
返回值：SUCCESS：初始化成功  ERROR：初始化失败
备注：因为给服务器设备发布消息需要特殊的等待周期和重发次数，故此函数专门用来给服务器发布消息
*/

u8 BC26_Data_Send(u8 *cmd,u8 *ack)
{
	u8 i = 0;//重复发送次数i
	u8 flag = 0;//定义一个结束标志位 
	u16 time = 300;//等待回复次数
	USART1_RX_STA = 0;//USART1接收标志位清零
	memset(USART1_RX_BUF,0,200);//清空USART1接收数组空间
	for(i=0;(i < 5) && (flag == 0);i++)//最多重复5次
	{
		printf("%s\r\n",cmd);//发送指令
		while(--time)
		{
			delay_ms(100);//等待周期
			if(USART1_RX_STA & 0x8000)//判断接收是否完成
			{
				if(strstr((const char *)USART1_RX_BUF, (const char *)ack))//比较想要的回应值是否为USART1接收到数据的子串
				{
					To_Computer((u8 *)"BC26回复：\r\n");
					To_Computer(USART1_RX_BUF);//给电脑发送USART1接收到的数据
					USART1_RX_STA = 0;
					flag = 1;//结束标志位置1
					break;//当接收到有效回应时退出循环
				}
				USART1_RX_STA = 0;
			}
		}
	
	}
	
	if(time == 0)return ERROR;//如果得不到回应 则返回ERROR
	else return SUCCESS;     //否则返回SUCCESS
	
}


/*
函数：BC26初始化函数
参数：void
返回值：SUCCESS：初始化成功  ERROR：初始化失败
备注：
*/
u8 BC26_Init(void)
{
	
	To_Computer((u8 *)"开始初始化BC26\r\n\r\n");
	
	//功能：重启BC26 重复次数：30 失败：返回错误
	To_Computer((u8 *)"正在重启...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"AT+QRST=1",(u8 *)"BROM",30)))return ERROR ;
	To_Computer((u8 *)"\r\n重启成功\r\n\r\n");
	
	//功能：关闭回显 重复次数：30 失败：返回错误
	To_Computer((u8 *)"正在关闭回显...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"ATE0",(u8 *)"OK",30)))return ERROR ;
	To_Computer((u8 *)"\r\n关闭回显成功\r\n\r\n");
	
	//功能：连接 重复次数：30 失败：返回错误
	To_Computer((u8 *)"正在连接...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"AT",(u8 *)"OK",30)))return ERROR;
	To_Computer((u8 *)"\r\n连接成功\r\n\r\n");
	
	//功能：设置MT完整功能 重复次数：30 失败：返回错误
	To_Computer((u8 *)"正在设置MT完整功能...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"AT+CFUN=1",(u8 *)"OK",30)))return ERROR;
	To_Computer((u8 *)"\r\n设置MT完整功能成功\r\n\r\n");
	
	//功能：获取卡号 重复次数：30 失败：返回错误
	To_Computer((u8 *)"正在获取卡号...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"AT+CIMI",(u8 *)"460",30)))return ERROR;
	To_Computer((u8 *)"\r\n获取卡号成功\r\n\r\n");
	
	//功能：激活网络 重复次数：30 失败：返回错误
	To_Computer((u8 *)"正在激活网络...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"AT+CGATT=1",(u8 *)"OK",30)))return ERROR;
	To_Computer((u8 *)"\r\n激活网络成功\r\n\r\n");
	
	//功能：查看是否激活成功 重复次数：30 失败：返回错误
	To_Computer((u8 *)"正在查看是否激活成功...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"AT+CGATT?",(u8 *)"+CGATT: 1",30)))return ERROR;
	To_Computer((u8 *)"\r\n激活网络成功\r\n\r\n");
	
	//功能：查询CSQ信号强度 重复次数：30 失败：返回错误
	To_Computer((u8 *)"正在查询CSQ信号强度...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"AT+CESQ",(u8 *)"OK",30)))return ERROR;
	To_Computer((u8 *)"\r\n查询成功\r\n\r\n");


	To_Computer((u8 *)"初始化成功\r\n");
	
	return SUCCESS;//返回成功

}