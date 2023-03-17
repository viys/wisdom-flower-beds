#ifndef __MQTT_H_
#define __MQTT_H_

#include "stm32f10x.h"
#include "bc26.h"

//���ķ��͵ķ�����ʱ�����¸���-------------------------------------------------
#define ProductKey "i3uocf6XqNy"						
#define DeviceName "bc26_1"					
#define DeviceSecret "8150afbe44b5c6ed14fd16adf40e4b72" 

#define Domain "iot-06z00dymnfjkl1d.mqtt.iothub.aliyuncs.com"			 
#define Port "1883"														                    //�˿�
#define ConnectID "bc26_1"						                          //�豸����
#define PubTopic "/sys/i3uocf6XqNy/bc26_1/thing/event/property/post"			      //����·��
#define SubTopic "/i3uocf6XqNy/bc26_1/user/get" 							//����·��
#define Param_Post1 "CurrentTemperature"			                            //��ǩ����
#define Param_Post2 "CurrentHumidity"	                                //
//���ķ��͵ķ�����ʱ�����ϸ���-------------------------------------------------

//ȫ�ֱ�������
extern u8 Buf[100];

//��������
u8 MQTT_Init(void);
u8 Data_Send(u8 tem,u8 hum);

#endif
