#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include <rtthread.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "app.h"



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
	
	//U3设备初始化
	u3_dev = rt_device_find("uart3");
	if(u3_dev == RT_NULL)
		LOG_E("rt_device_find[uart3] failed..\n");
	else
	{
		LOG_D("rt_device_find[uart3] successed..\n");
		ret = rt_device_init(u3_dev);
		if(ret < 0)
			LOG_E("rt_device_init[uart3] failed..\n");
		else
			LOG_D("rt_device_init[uart3] successed..\n");
		ret = rt_device_open(u3_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret < 0)
			LOG_E("rt_device_open[uart3] failed..\n");
		else
			LOG_D("rt_device_open[uart3] successed..\n");		
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
	
	//BC28设备初始化
	BC28_dev = rt_device_find("BC28");
	if(BC28_dev == RT_NULL)
		LOG_E("rt_device_find[BC28] failed..\n");
	else
	{
		LOG_D("rt_device_find[BC28] successed..\n");
		ret = rt_device_init(BC28_dev);
		if(ret < 0)
			LOG_E("rt_device_init[BC28] failed..\n");
		else
			LOG_D("rt_device_init[BC28] successed..\n");
		ret = rt_device_open(BC28_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret < 0)
			LOG_E("rt_device_open[BC28] failed..\n");
		else
			LOG_D("rt_device_open[BC28] successed..\n");		
	}
	
	//动态信号量创建
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
	
//	A72_Data_handle = rt_mutex_create("A72_Data_handle",RT_IPC_FLAG_FIFO);//先进先出
//	if(A72_Data_handle == RT_NULL)
//		LOG_E("A72_Data_handle rt_mutex_create create failed..\n");
//	else
//		LOG_D("A72_Data_handle rt_mutex_create create successed..\n");
	
	FS_Respond = rt_sem_create("FS_Respond",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(FS_Respond == RT_NULL)
		LOG_E("dynamic FS_Respond rt_sem_create failed..\n");
	else
		LOG_D("dynamic FS_Respond rt_sem_create successed..\n");
	
	BC28_Respond = rt_sem_create("BC28_Respond",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(BC28_Respond == RT_NULL)
		LOG_E("dynamic BC28_Respond rt_sem_create failed..\n");
	else
		LOG_D("dynamic BC28_Respond rt_sem_create successed..\n");
	
	WaterValve = rt_sem_create("WaterValve",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(WaterValve == RT_NULL)
		LOG_E("dynamic WaterValve rt_sem_create failed..\n");
	else
		LOG_D("dynamic WaterValve rt_sem_create successed..\n");
	
	//静态硬定时创建
	rt_timer_init(&bc28_10ms,"bc28_10ms",bc28_callback,NULL,10,//1s执行一次 
	RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_HARD_TIMER);//一次性 硬定时 
	
	//任务1创建
	ret = rt_thread_init(&th1,"WLAN_INIT",WLAN_INIT,NULL,th1_stack,sizeof(th1_stack),th1_test_priority,5);
	if(ret < 0)
		LOG_E("th1 create failed..\n");
	else
		LOG_D("th1 create successed..\n");
	
	//任务2创建
	ret = rt_thread_init(&th2,"Zigbee_Data_handle",Zigbee_Data_handle,NULL,th2_stack,sizeof(th2_stack),th2_test_priority,5);
	if(ret < 0)
		LOG_E("th2 create failed..\n");
	else
		LOG_D("th2 create successed..\n");
	
	//任务3创建
	ret = rt_thread_init(&th3,"Zigbee_Heartbeat",Zigbee_Heartbeat,NULL,th3_stack,sizeof(th3_stack),th3_test_priority,5);
	if(ret < 0)
		LOG_E("th3 create failed..\n");
	else
		LOG_D("th3 create successed..\n");
	
	//任务4创建
	ret = rt_thread_init(&th4,"Air_quality_takeValue",Air_quality_takeValue,NULL,th4_stack,sizeof(th4_stack),th4_test_priority,5);
	if(ret < 0)
		LOG_E("th4 create failed..\n");
	else
		LOG_D("th4 create successed..\n");
//	
	//任务5创建
	ret = rt_thread_init(&th5,"MQTT_INIT",MQTT_INIT,NULL,th5_stack,sizeof(th5_stack),th5_test_priority,5);
	if(ret < 0)
		LOG_E("th5 create failed..\n");
	else
		LOG_D("th5 create successed..\n");
//	
	//任务6创建
	ret = rt_thread_init(&th6,"MQTT_PUB_DATA",MQTT_PUB_DATA,NULL,th6_stack,sizeof(th6_stack),th6_test_priority,5);
	if(ret < 0)
		LOG_E("th6 create failed..\n");
	else
		LOG_D("th6 create successed..\n");
	
	//任务7创建
	ret = rt_thread_init(&th7,"WaterValveControl",WaterValveControl,NULL,th7_stack,sizeof(th7_stack),th7_test_priority,5);
	if(ret < 0)
		LOG_E("th7 create failed..\n");
	else
		LOG_D("th7 create successed..\n");
	
	//任务启动
	rt_thread_startup(&th1);
	rt_thread_startup(&th4);
	rt_thread_startup(&th5);
	rt_thread_startup(&th7);
}



//信号量
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

