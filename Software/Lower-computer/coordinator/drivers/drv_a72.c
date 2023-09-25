#include "drv_a72.h"


/*ZIGBEE失效执行函数*/
void ZIGBEE_Disconnect(void)
{
	
	A72_Device_Connect = 0;//标志位置0
					
	rt_thread_resume(&th1);//恢复WLAN_INIT()
					
	rt_thread_suspend(&th2);//挂起Zigbee_Data_handle()
					
	rt_thread_suspend(&th3);//挂起Zigbee_Heartbeat()
}



/*ZIGBEE打印数据*/
static void a72_information(void)
{
	
#if ZIGBEE_A72_NODE
	
	A72_SEND_ORDER(&Sleep_time,A72_Read_Sleep_time);
	
#endif
	
	A72_Print_Information();
}


#if ZIGBEE_A72_NODE
/*ZIGBEE节点修改睡眠时间*/
static void node_set_sleeptime(int argc,char**argv)
{
	
	if(argc < 2)
	{
		rt_kprintf("please enter a value less than 10s\n");
		return;
	}
	
	int len = strlen(argv[1]);
	
	rt_uint16_t time = (rt_uint16_t)(argv[1][0] - '0');
	
	if((len != 1) || time >9 || time <= 0)
	{
		rt_kprintf("Error! Please input correctly!(number && <10 && >0)\n");
		return;
	}
	
	if(A72_SET_TIME(time) == ERROR)
		rt_kprintf("node_set_sleeptime failed..\n");
	else
		rt_kprintf("node_set_sleeptime successed..\n");
	
}
#endif


/*ZIGBEE复位*/
static void a72_reset(void)
{
	A72_SEND_ORDER(&_RESET,A72_NoReply);
}

static void a72_rfs(void)
{
	A72_SEND_ORDER(&_RFS,A72_Read_RFS);//恢复出场设置,,,
	
}
static void a72_init(void)
{
	A72_Init();
}

/*ZIGBEE设备初始化*/
int rt_A72_init(void)
{
	rt_device_t a72_dev;
	a72_dev = rt_device_create(RT_Device_Class_Block,128);
	if(a72_dev == RT_NULL)
	{
		LOG_E("a72_dev rt_device_create failed..\n");
		return -RT_ENOMEM;
	}	
	rt_device_register(a72_dev,"A72",RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);//注册  读写
	return 0;
}

INIT_DEVICE_EXPORT(rt_A72_init);

MSH_CMD_EXPORT(a72_information,Print A72 Information);
MSH_CMD_EXPORT(a72_reset,!! A72 RESET);
MSH_CMD_EXPORT(a72_rfs,!! A72 RFS);
MSH_CMD_EXPORT(a72_init,!! A72 INIT);
#if ZIGBEE_A72_NODE
MSH_CMD_EXPORT(node_set_sleeptime,SET Sleep Time （ < 10 s）);
#endif
