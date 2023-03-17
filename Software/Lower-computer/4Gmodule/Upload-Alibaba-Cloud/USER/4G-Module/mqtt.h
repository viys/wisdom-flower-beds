#ifndef __MQTT_H_
#define __MQTT_H_

#include "stm32f10x.h"
#include "bc26.h"

//更改发送的服务器时在以下更改-------------------------------------------------
#define ProductKey "i3uocf6XqNy"						
#define DeviceName "bc26_1"					
#define DeviceSecret "8150afbe44b5c6ed14fd16adf40e4b72" 

#define Domain "iot-06z00dymnfjkl1d.mqtt.iothub.aliyuncs.com"			 
#define Port "1883"														                    //端口
#define ConnectID "bc26_1"						                          //设备名称
#define PubTopic "/sys/i3uocf6XqNy/bc26_1/thing/event/property/post"			      //发送路径
#define SubTopic "/i3uocf6XqNy/bc26_1/user/get" 							//订阅路径
#define Param_Post1 "CurrentTemperature"			                            //标签名称
#define Param_Post2 "CurrentHumidity"	                                //
//更改发送的服务器时在以上更改-------------------------------------------------

//全局变量声明
extern u8 Buf[100];

//函数声明
u8 MQTT_Init(void);
u8 Data_Send(u8 tem,u8 hum);

#endif
