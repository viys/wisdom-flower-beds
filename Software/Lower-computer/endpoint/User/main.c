#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include <rtthread.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "app.h"
#include "drv_demo.h"



rt_thread_t start = NULL;



void AppTaskStart(void *parameter)
{
	int ret = 0;//临时变量
	
	
	
	//U1设备初始化
	u1_dev = rt_device_find("uart1");
	if(u1_dev == RT_NULL)
		LOG_E("rt_device_find[uart1] failed..\n");
	else
	{
		LOG_D("rt_device_find[uart1] successed..\n");
		ret = rt_device_init(u1_dev);
		if(ret < 0)
			LOG_E("rt_device_init[uart1] failed..\n");
		else
			LOG_D("rt_device_init[uart1] successed..\n");
		ret = rt_device_open(u1_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret < 0)
			LOG_E("rt_device_open[uart1] failed..\n");
		else
			LOG_D("rt_device_open[uart1] successed..\n");		
	}
	
	//U2设备初始化
	u2_dev = rt_device_find("usart2");
	if(u2_dev == RT_NULL)
		LOG_E("rt_device_find[usart2] failed..\n");
	else
	{
		LOG_D("rt_device_find[usart2] successed..\n");
		ret = rt_device_init(u2_dev);
		if(ret < 0)
			LOG_E("rt_device_init[usart2] failed..\n");
		else
			LOG_D("rt_device_init[usart2] successed..\n");
		ret = rt_device_open(u2_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret < 0)
			LOG_E("rt_device_open[usart2] failed..\n");
		else
			LOG_D("rt_device_open[usart2] successed..\n");		
	}
	
	
	//Senor设备初始化
	Sensor_dev = rt_device_find("sensor");
	if(Sensor_dev == RT_NULL)
		LOG_E("rt_device_find[sensor] failed..\n");
	else
	{
		LOG_D("rt_device_find[sensor] successed..\n");
		ret = rt_device_init(Sensor_dev);
		if(ret < 0)
			LOG_E("rt_device_init[sensor] failed..\n");
		else
			LOG_D("rt_device_init[sensor] successed..\n");
		ret = rt_device_open(Sensor_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret < 0)
			LOG_E("rt_device_open[sensor] failed..\n");
		else
			LOG_D("rt_device_open[sensor] successed..\n");		
	}
	
	//A72设备初始化
	A72_dev = rt_device_find("A72");
	if(A72_dev == RT_NULL)
		LOG_E("rt_device_find[A72] failed..\n");
	else
	{
		LOG_D("rt_device_find[A72] successed..\n");
		ret = rt_device_init(A72_dev);
		if(ret < 0)
			LOG_E("rt_device_init[A72] failed..\n");
		else
			LOG_D("rt_device_init[A72] successed..\n");
		ret = rt_device_open(A72_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret < 0)
			LOG_E("rt_device_open[A72] failed..\n");
		else
			LOG_D("rt_device_open[A72] successed..\n");		
	}
	
	//动态信号量创建
	modbus_respond = rt_sem_create("modbus_respond",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(modbus_respond == RT_NULL)
		LOG_E("dynamic modbus_respond rt_sem_create failed..\n");
	else
		LOG_D("dynamic modbus_respond rt_sem_create successed..\n");
	
	A72_Respond = rt_sem_create("A72_Respond",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(A72_Respond == RT_NULL)
		LOG_E("dynamic A72_Respond rt_sem_create failed..\n");
	else
		LOG_D("dynamic A72_Respond rt_sem_create successed..\n");	
	
	A72_Connect = rt_sem_create("A72_Connect",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(A72_Connect == RT_NULL)
		LOG_E("dynamic A72_Connect rt_sem_create failed..\n");
	else
		LOG_D("dynamic A72_Connect rt_sem_create successed..\n");
	
	A72_Data_handle = rt_sem_create("A72_Data_handle",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(A72_Data_handle == RT_NULL)
		LOG_E("dynamic A72_Data_handle rt_sem_create failed..\n");
	else
		LOG_D("dynamic A72_Data_handle rt_sem_create successed..\n");
	
	//动态硬定时创建
	modbus_timer = rt_timer_create("modbus_timer",modbus_callback,NULL,1, //1ms执行一次   
	RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_HARD_TIMER);//周期性 软定时
	if(modbus_timer == RT_NULL)
		LOG_E("rt_timer_create failed..\n");
	else
		LOG_D("rt_timer_create successed..\n");
	
	//任务1创建
	ret = rt_thread_init(&th1,"Sensor_Debug",Sensor_Debug,NULL,th1_stack,sizeof(th1_stack),th1_test_priority,5);
	if(ret < 0)
		LOG_E("th1 create failed..\n");
	else
		LOG_D("th1 create successed..\n");
	
	//任务2创建
	ret = rt_thread_init(&th2,"WLAN_INIT",WLAN_INIT,NULL,th2_stack,sizeof(th2_stack),th2_test_priority,5);
	if(ret < 0)
		LOG_E("th2 create failed..\n");
	else
		LOG_D("th2 create successed..\n");
	
	//任务3创建
	ret = rt_thread_init(&th3,"Cheak_front",Zigbee_Network_Status_Cheak_front,NULL,th3_stack,sizeof(th3_stack),th3_test_priority,5);
	if(ret < 0)
		LOG_E("th3 create failed..\n");
	else
		LOG_D("th3 create successed..\n");
	
	//任务4创建
	ret = rt_thread_init(&th4,"Cheak_after",Zigbee_Network_Status_Cheak_after,NULL,th4_stack,sizeof(th4_stack),th4_test_priority,5);
	if(ret < 0)
		LOG_E("th4 create failed..\n");
	else
		LOG_D("th4 create successed..\n");
	
	//任务5创建
	ret = rt_thread_init(&th5,"Zigbee_Send_Data",Zigbee_Send_Data,NULL,th5_stack,sizeof(th5_stack),th5_test_priority,5);
	if(ret < 0)
		LOG_E("th5 create failed..\n");
	else
		LOG_D("th5 create successed..\n");
	
	//任务6创建
	ret = rt_thread_init(&th6,"Zigbee_Data_handle",Zigbee_Data_handle,NULL,th6_stack,sizeof(th6_stack),th6_test_priority,5);
	if(ret < 0)
		LOG_E("th6 create failed..\n");
	else
		LOG_D("th6 create successed..\n");
	
	//任务7创建
	ret = rt_thread_init(&th7,"Power_Metering",Power_Metering,NULL,th7_stack,sizeof(th7_stack),th7_test_priority,5);
	if(ret < 0)
		LOG_E("th7 create failed..\n");
	else
		LOG_D("th7 create successed..\n");

	//任务启动
	rt_thread_startup(&th1);
	rt_thread_startup(&th2);
	rt_thread_startup(&th7);
	
	//定时器启动
	rt_timer_start(modbus_timer);
}


//主函数
int main()
{
	//开始任务创建
  start = rt_thread_create("App Task Start", AppTaskStart, NULL,1024, 20, 5);
  if(start == RT_NULL){
     LOG_E("App Task Start create failed..\n");
     return -RT_ENOMEM;
   }

   LOG_D("App Task Start create successed..\n");
   
   rt_thread_startup(start);//启动
	 
}

