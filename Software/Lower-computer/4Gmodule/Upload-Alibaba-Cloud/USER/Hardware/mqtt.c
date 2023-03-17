#include "mqtt.h"


u8 Buf[100];//�����һ��������ʱ�洢��Ϣ ������ProductKey��Port�ȣ�

/*
������MQTT��ʼ������
������void
����ֵ��SUCCESS����ʼ���ɹ�  ERROR����ʼ��ʧ��
��ע��
*/
u8 MQTT_Init(void)
{
	To_Computer((u8 *)"��ʼ��ʼ��MQTT\r\n\r\n");
	
	//���ܣ������Ʒ�����Ԫ�� �ظ�������60 ʧ�ܣ����ش���
	To_Computer((u8 *)"���������Ʒ�����Ԫ��...\r\n\r\n");
	memset(Buf,0,100);//�����Buf����
	sprintf((char *)Buf,"AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"", ProductKey, DeviceName, DeviceSecret);
	if(!(BC26_Send_Check(Buf,(u8 *)"OK",60)))return ERROR ;
	To_Computer((u8 *)"\r\n�����Ʒ�����Ԫ��ɹ�\r\n\r\n");
	
	//���ܣ����Ӱ����Ʒ����� �ظ�������100 ʧ�ܣ����ش���
	To_Computer((u8 *)"�������Ӱ����Ʒ�����...\r\n\r\n");
	memset(Buf,0,100);//�����Buf����
	sprintf((char *)Buf,"AT+QMTOPEN=0,\"%s\",%s", Domain, Port);
	if(!(BC26_Send_Check(Buf,(u8 *)"OK",100)))return ERROR ;
	To_Computer((u8 *)"\r\n���Ӱ����Ʒ������ɹ�\r\n\r\n");
	
	//���ܣ������豸 �ظ�������150 ʧ�ܣ����ش���
	To_Computer((u8 *)"���������豸...\r\n\r\n");
	memset(Buf,0,100);//�����Buf����
	sprintf((char *)Buf,"AT+QMTCONN=0,\"%s\"", ConnectID);
	if(!(BC26_Send_Check(Buf,(u8 *)"OK",150)))return ERROR ;
	To_Computer((u8 *)"\r\n�����豸�ɹ�\r\n\r\n");	
	
	//���ܣ��������� �ظ�������100 ʧ�ܣ����ش���
	To_Computer((u8 *)"���ڶ�������...\r\n\r\n");
	memset(Buf,0,100);//�����Buf����
	sprintf((char *)Buf,"AT+QMTSUB=0,1,\"%s\",0", SubTopic);
	if(!(BC26_Send_Check(Buf,(u8 *)"OK",100)))return ERROR ;
	To_Computer((u8 *)"\r\n��������ɹ�\r\n\r\n");		
	
	To_Computer((u8 *)"��ʼ���ɹ�\r\n");
	
	return SUCCESS;//���سɹ�
}




/************************************�û�����********************************************/


/*
�������������������ݺ���
�������Լ�����
����ֵ��SUCCESS����ʼ���ɹ�  ERROR����ʼ��ʧ��
��ע���������ݸ��������Լ����壬��������DHT11��ʪ�ȴ�������ʾ��
*/
u8 Data_Send(u8 tem,u8 hum)
{
	u8 Buf1[200];
	u8 Buf2[200];
	char value1[10];
	char value2[10];

	sprintf(value1,"%.2f", (float)tem);//д���¶�
	sprintf(value2,"%.2f", (float)hum);//д��ʪ��
	
	sprintf((char *)Buf1, "AT+QMTPUB=0,1,1,0,\"%s\",\"{params:{%s:%s}}\"\r\n", PubTopic, Param_Post1, value1);//д��AT
	sprintf((char *)Buf2, "AT+QMTPUB=0,1,1,0,\"%s\",\"{params:{%s:%s}}\"\r\n", PubTopic, Param_Post2, value2);//д��AT
	
	//�����¶�
	if(!(BC26_Data_Send(Buf1,(u8 *)"+QMTPUB: 0,1,0")))return ERROR ;//�������� ʧ���򷵻�ERROR
	//����ʪ��
	if(!(BC26_Data_Send(Buf2,(u8 *)"+QMTPUB: 0,1,0")))return ERROR ;//�������� ʧ���򷵻�ERROR
	

	return SUCCESS;//���سɹ�
}
