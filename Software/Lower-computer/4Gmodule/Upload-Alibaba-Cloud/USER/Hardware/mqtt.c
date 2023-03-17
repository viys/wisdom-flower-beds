#include "mqtt.h"


u8 Buf[100];//定义个一数组来临时存储信息 （例如ProductKey、Port等）

/*
函数：MQTT初始化函数
参数：void
返回值：SUCCESS：初始化成功  ERROR：初始化失败
备注：
*/
u8 MQTT_Init(void)
{
	To_Computer((u8 *)"开始初始化MQTT\r\n\r\n");
	
	//功能：向阿里云发送三元组 重复次数：60 失败：返回错误
	To_Computer((u8 *)"正在向阿里云发送三元组...\r\n\r\n");
	memset(Buf,0,100);//先清空Buf数组
	sprintf((char *)Buf,"AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"", ProductKey, DeviceName, DeviceSecret);
	if(!(BC26_Send_Check(Buf,(u8 *)"OK",60)))return ERROR ;
	To_Computer((u8 *)"\r\n向阿里云发送三元组成功\r\n\r\n");
	
	//功能：连接阿里云服务器 重复次数：100 失败：返回错误
	To_Computer((u8 *)"正在连接阿里云服务器...\r\n\r\n");
	memset(Buf,0,100);//先清空Buf数组
	sprintf((char *)Buf,"AT+QMTOPEN=0,\"%s\",%s", Domain, Port);
	if(!(BC26_Send_Check(Buf,(u8 *)"OK",100)))return ERROR ;
	To_Computer((u8 *)"\r\n连接阿里云服务器成功\r\n\r\n");
	
	//功能：连接设备 重复次数：150 失败：返回错误
	To_Computer((u8 *)"正在连接设备...\r\n\r\n");
	memset(Buf,0,100);//先清空Buf数组
	sprintf((char *)Buf,"AT+QMTCONN=0,\"%s\"", ConnectID);
	if(!(BC26_Send_Check(Buf,(u8 *)"OK",150)))return ERROR ;
	To_Computer((u8 *)"\r\n连接设备成功\r\n\r\n");	
	
	//功能：订阅主题 重复次数：100 失败：返回错误
	To_Computer((u8 *)"正在订阅主题...\r\n\r\n");
	memset(Buf,0,100);//先清空Buf数组
	sprintf((char *)Buf,"AT+QMTSUB=0,1,\"%s\",0", SubTopic);
	if(!(BC26_Send_Check(Buf,(u8 *)"OK",100)))return ERROR ;
	To_Computer((u8 *)"\r\n订阅主题成功\r\n\r\n");		
	
	To_Computer((u8 *)"初始化成功\r\n");
	
	return SUCCESS;//返回成功
}




/************************************用户定义********************************************/


/*
函数：服务器发送数据函数
参数：自己定义
返回值：SUCCESS：初始化成功  ERROR：初始化失败
备注：函数内容根据需求自己定义，本程序拿DHT11温湿度传感器做示例
*/
u8 Data_Send(u8 tem,u8 hum)
{
	u8 Buf1[200];
	u8 Buf2[200];
	char value1[10];
	char value2[10];

	sprintf(value1,"%.2f", (float)tem);//写入温度
	sprintf(value2,"%.2f", (float)hum);//写入湿度
	
	sprintf((char *)Buf1, "AT+QMTPUB=0,1,1,0,\"%s\",\"{params:{%s:%s}}\"\r\n", PubTopic, Param_Post1, value1);//写入AT
	sprintf((char *)Buf2, "AT+QMTPUB=0,1,1,0,\"%s\",\"{params:{%s:%s}}\"\r\n", PubTopic, Param_Post2, value2);//写入AT
	
	//发送温度
	if(!(BC26_Data_Send(Buf1,(u8 *)"+QMTPUB: 0,1,0")))return ERROR ;//发送数据 失败则返回ERROR
	//发送湿度
	if(!(BC26_Data_Send(Buf2,(u8 *)"+QMTPUB: 0,1,0")))return ERROR ;//发送数据 失败则返回ERROR
	

	return SUCCESS;//返回成功
}
