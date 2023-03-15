#include "bc26.h"




/*
����������AT����AT����
������cmd�����͸�BC26��ATָ��  ack����Ҫ�õ��Ļ�Ӧָ��  time������ظ�����
����ֵ��SUCCESS����ʼ���ɹ�  ERROR����ʼ��ʧ��
��ע��
*/

u8 BC26_Send_Check(u8 *cmd,u8 *ack,u16 time)
{
	USART1_RX_STA = 0;//USART1���ձ�־λ����
	memset(USART1_RX_BUF, 0, 200);//���USART1��������ռ�
	while(--time)
	{
		printf("%s\r\n",cmd);//����ATָ��
//		if(Send_Flag)delay_ms(100);
		delay_ms(300);//�ȴ�����
		if(USART1_RX_STA & 0x8000)//�жϽ����Ƿ����
		{
			if(strstr((const char *)USART1_RX_BUF, (const char *)ack))//�Ƚ���Ҫ�Ļ�Ӧֵ�Ƿ�ΪUSART1���յ����ݵ��Ӵ�
			{
				To_Computer((u8 *)"BC26��Ӧ���ݣ�\r\n");
				To_Computer(USART1_RX_BUF);//�����Է���USART1���յ�������
				USART1_RX_STA = 0;
				break;//�����յ���Ч��Ӧʱ�˳�ѭ��
			}
			USART1_RX_STA = 0;
		}
	}		
	if(time == 0)return ERROR;//����ò�����Ӧ �򷵻�ERROR
	else return SUCCESS;     //���򷵻�SUCCESS
	
}

/*
��������������������Ϣ����
������cmd�����͸�BC26��ATָ��  ack����Ҫ�õ��Ļ�Ӧָ��
����ֵ��SUCCESS����ʼ���ɹ�  ERROR����ʼ��ʧ��
��ע����Ϊ���������豸������Ϣ��Ҫ����ĵȴ����ں��ط��������ʴ˺���ר��������������������Ϣ
*/

u8 BC26_Data_Send(u8 *cmd,u8 *ack)
{
	u8 i = 0;//�ظ����ʹ���i
	u8 flag = 0;//����һ��������־λ 
	u16 time = 300;//�ȴ��ظ�����
	USART1_RX_STA = 0;//USART1���ձ�־λ����
	memset(USART1_RX_BUF,0,200);//���USART1��������ռ�
	for(i=0;(i < 5) && (flag == 0);i++)//����ظ�5��
	{
		printf("%s\r\n",cmd);//����ָ��
		while(--time)
		{
			delay_ms(100);//�ȴ�����
			if(USART1_RX_STA & 0x8000)//�жϽ����Ƿ����
			{
				if(strstr((const char *)USART1_RX_BUF, (const char *)ack))//�Ƚ���Ҫ�Ļ�Ӧֵ�Ƿ�ΪUSART1���յ����ݵ��Ӵ�
				{
					To_Computer((u8 *)"BC26�ظ���\r\n");
					To_Computer(USART1_RX_BUF);//�����Է���USART1���յ�������
					USART1_RX_STA = 0;
					flag = 1;//������־λ��1
					break;//�����յ���Ч��Ӧʱ�˳�ѭ��
				}
				USART1_RX_STA = 0;
			}
		}
	
	}
	
	if(time == 0)return ERROR;//����ò�����Ӧ �򷵻�ERROR
	else return SUCCESS;     //���򷵻�SUCCESS
	
}


/*
������BC26��ʼ������
������void
����ֵ��SUCCESS����ʼ���ɹ�  ERROR����ʼ��ʧ��
��ע��
*/
u8 BC26_Init(void)
{
	
	To_Computer((u8 *)"��ʼ��ʼ��BC26\r\n\r\n");
	
	//���ܣ�����BC26 �ظ�������30 ʧ�ܣ����ش���
	To_Computer((u8 *)"��������...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"AT+QRST=1",(u8 *)"BROM",30)))return ERROR ;
	To_Computer((u8 *)"\r\n�����ɹ�\r\n\r\n");
	
	//���ܣ��رջ��� �ظ�������30 ʧ�ܣ����ش���
	To_Computer((u8 *)"���ڹرջ���...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"ATE0",(u8 *)"OK",30)))return ERROR ;
	To_Computer((u8 *)"\r\n�رջ��Գɹ�\r\n\r\n");
	
	//���ܣ����� �ظ�������30 ʧ�ܣ����ش���
	To_Computer((u8 *)"��������...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"AT",(u8 *)"OK",30)))return ERROR;
	To_Computer((u8 *)"\r\n���ӳɹ�\r\n\r\n");
	
	//���ܣ�����MT�������� �ظ�������30 ʧ�ܣ����ش���
	To_Computer((u8 *)"��������MT��������...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"AT+CFUN=1",(u8 *)"OK",30)))return ERROR;
	To_Computer((u8 *)"\r\n����MT�������ܳɹ�\r\n\r\n");
	
	//���ܣ���ȡ���� �ظ�������30 ʧ�ܣ����ش���
	To_Computer((u8 *)"���ڻ�ȡ����...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"AT+CIMI",(u8 *)"460",30)))return ERROR;
	To_Computer((u8 *)"\r\n��ȡ���ųɹ�\r\n\r\n");
	
	//���ܣ��������� �ظ�������30 ʧ�ܣ����ش���
	To_Computer((u8 *)"���ڼ�������...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"AT+CGATT=1",(u8 *)"OK",30)))return ERROR;
	To_Computer((u8 *)"\r\n��������ɹ�\r\n\r\n");
	
	//���ܣ��鿴�Ƿ񼤻�ɹ� �ظ�������30 ʧ�ܣ����ش���
	To_Computer((u8 *)"���ڲ鿴�Ƿ񼤻�ɹ�...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"AT+CGATT?",(u8 *)"+CGATT: 1",30)))return ERROR;
	To_Computer((u8 *)"\r\n��������ɹ�\r\n\r\n");
	
	//���ܣ���ѯCSQ�ź�ǿ�� �ظ�������30 ʧ�ܣ����ش���
	To_Computer((u8 *)"���ڲ�ѯCSQ�ź�ǿ��...\r\n\r\n");
	if(!(BC26_Send_Check((u8 *)"AT+CESQ",(u8 *)"OK",30)))return ERROR;
	To_Computer((u8 *)"\r\n��ѯ�ɹ�\r\n\r\n");


	To_Computer((u8 *)"��ʼ���ɹ�\r\n");
	
	return SUCCESS;//���سɹ�

}