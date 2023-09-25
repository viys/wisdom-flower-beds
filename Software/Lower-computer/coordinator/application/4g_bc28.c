#include "4g_bc28.h"

rt_uint8_t BC28_RX_BUF[200] = {0};//BC28接收数组
rt_uint8_t BC28_RX_FLAG = BC28_RX_OFF;//BC28接收标志位
rt_uint16_t BC28_RX_DATA_LEN = 0;//BC28接收数据长度


/*
函数：检查BC28回应数据是否与预期相符
参数：ack：想要得到的回应指令 waitTime：等待周期时间(ms)
返回值：SUCCESS：成功  ERROR：失败
备注：
*/
static int BC28_Reply(char *ack,rt_uint16_t waitTime)
{
	if(rt_sem_take(BC28_Respond,waitTime) == 0)//判断接收是否完成
	{
		if(strstr((const char*)BC28_RX_BUF,ack))//比较想要的回应值是否为USART1接收到数据的子串
		{
			return SUCCESS;//当接收到有效回应时退出循环
		}
	}

	return ERROR;

}


/*
函数：发送AT与检查AT函数
参数：cmd：发送给BC26的AT指令  ack：想要得到的回应指令  frequency：最大等待周期次数 waitTime：等待周期时间(ms)
返回值：SUCCESS：成功  ERROR：失败
备注：
*/
int BC28_Send_Check(char *cmd,char *ack,rt_uint8_t frequency,rt_uint16_t waitTime)
{
	
	unsigned char MAX_Repeat = 10;
	
	unsigned char _time = frequency;
	
	
	while(MAX_Repeat--)
	{
		
		memset(BC28_RX_BUF, 0, 200);
			
		BC28_SEND_DATA("%s\r\n",cmd);//发送AT指令
		
		while(_time)
		{
			if(BC28_Reply(ack,waitTime) == SUCCESS)
				return SUCCESS;
			_time--;
		}
		_time = frequency;
	}		
	
	return ERROR;//如果得不到回应 则返回ERROR
	
}


/*
函数：向服务器发布消息
参数：data；待发布的数据(JSON格式)
返回值：SUCCESS：成功  ERROR：失败
备注：
*/
int MQTT_PUB(char *data)
{
	char buf[200];
	
	int flag;
	
	sprintf(buf,"AT+QMTPUB=0,1,1,0,\"%s\"",PubTopic);
	
	
	if(!BC28_Send_Check(buf,">",2,300))return ERROR;
	
	
	BC28_SEND_DATA("%s\r\n",data);
	
	
	rt_thread_mdelay(50);
	
	BC28_SEND_DATA("%c",0x1A);	
	
	for(rt_uint8_t i = 0; i < 2; i ++)
	{
		flag = BC28_Reply("0,1,0",4000);
	}
	
	return flag;
}



/*
函数：BC28初始化函数
参数：void
返回值：SUCCESS：初始化成功  ERROR：初始化失败
备注：
*/
int BC28_Init(void)
{
	
	rt_kprintf("Start initializing BC28\n\n");
	
	//功能：重启BC28
	rt_kprintf("Restarting...\n\n");
	if(!(BC28_Send_Check("AT+NRB","OK",5,300)))return ERROR ;
	rt_kprintf("\nReboot successful\n\n");
	
	
	//功能：连接
	rt_kprintf("connecting...\n\n");
	if(!(BC28_Send_Check("AT","OK",1,300)))return ERROR;
	rt_kprintf("\nSuccessfully connected\n\n");
	
	//功能：设置UE完整功能
	rt_kprintf("Setting all UE features...\n\n");
	if(!(BC28_Send_Check("AT+CFUN=1","OK",5,600)))return ERROR;
	rt_kprintf("\nSuccessfully set\n\n");
	
	//功能：获取卡号
	rt_kprintf("Getting CIMI...\n\n");
	if(!(BC28_Send_Check("AT+CIMI","460",1,300)))return ERROR;
	rt_kprintf("\nobtain\n\n");
	
	//功能：激活网络
	rt_kprintf("Activating network...\n\n");
	if(!(BC28_Send_Check("AT+CGATT=1","OK",1,300)))return ERROR;
	rt_kprintf("\nobtain\n\n");
	
	//功能：查看是否激活成功
	rt_kprintf("Checking if activation was successful...\n\n");
	if(!(BC28_Send_Check("AT+CGATT?","+CGATT:1",3,600)))return ERROR;
	rt_kprintf("\nSuccessfully activated network\n\n");
	
	rt_kprintf("BC28 initialization successful\r\n");
	
	return SUCCESS;//返回成功

}
/*
函数：MQTT初始化函数
参数：void
返回值：SUCCESS：初始化成功  ERROR：初始化失败
备注：
*/
int MQTT_Init(void)
{
	
	char Buf[100];
	
	rt_kprintf("Start initializing MQTT\n");
	
	//功能：向阿里云发送3元组
	rt_kprintf("Sending triplets to Aliyun...\n");
	memset(Buf,0,100);
	sprintf(Buf,"AT+QMTCFG=\"aliauth\",0,\"%s\",\"%s\",\"%s\"", ProductKey, DeviceName, DeviceSecret);
	if(!BC28_Send_Check(Buf,"OK",1,1000))return ERROR ;
	rt_kprintf("\nSending completed\n");
	
	rt_thread_mdelay(2000);
	
	//功能：连接阿里云服务器
	rt_kprintf("Connecting to Aliyun server...\n");
	memset(Buf,0,100);
	sprintf(Buf,"AT+QMTOPEN=0,\"%s\",%s", Domain, Port);
	if(!BC28_Send_Check(Buf,": 0,0",5,1000))return ERROR ;
	rt_kprintf("\nSuccessfully connected to Aliyun server\n");
	
	rt_thread_mdelay(2000);
	
	
	//功能：连接阿里云设备
	rt_kprintf("Connecting device...\n");
	memset(Buf,0,100);
	sprintf(Buf,"AT+QMTCONN=0,\"%s\"", ConnectID);
	if(!BC28_Send_Check(Buf,"OK",2,1000))return ERROR ;
	rt_kprintf("\nSuccessfully connected device\n");
	
	//功能：订阅主题
	rt_kprintf("Subscribing to topic...\n");
	memset(Buf,0,100);
	sprintf(Buf,"AT+QMTSUB=0,1,\"%s\",0", SubTopic);
	if(!BC28_Send_Check(Buf,"OK",2,1000))return ERROR ;
	rt_kprintf("\nSubscription successful\n");
	
	rt_kprintf("MQTT initialization successful\n");
	
	return SUCCESS;

}
