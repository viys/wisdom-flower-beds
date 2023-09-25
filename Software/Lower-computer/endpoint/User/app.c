#define DBG_TAG "app"
#define DBG_LVL DBG_LOG



#include "app.h"

//任务1
struct rt_thread th1;
rt_uint8_t th1_stack[512] = {0};

//任务2
struct rt_thread th2;
rt_uint8_t th2_stack[512] = {0};

//任务3
struct rt_thread th3;
rt_uint8_t th3_stack[512] = {0};

//任务4
struct rt_thread th4;
rt_uint8_t th4_stack[512] = {0};

//任务5
struct rt_thread th5;
rt_uint8_t th5_stack[512] = {0};

//任务6
struct rt_thread th6;
rt_uint8_t th6_stack[512] = {0};

//任务7
struct rt_thread th7;
rt_uint8_t th7_stack[1024] = {0};


rt_sem_t A72_Respond;//A72回应信号量
rt_sem_t A72_Connect;//A72连接信号量
rt_sem_t A72_Data_handle;

rt_uint8_t A72_Device_Connect = 0;//ZIGBEE硬件状况标志位 0：正常 1：异常 

rt_err_t A72_Cheak;

rt_uint8_t Sensor_Connect = 1;//默认为1

STRUCT_ZIGBEETypeDef  zigbee_value;

//任务1函数--Debug
void Sensor_Debug(void *parameter)
{
	char buf[30];
	
	rt_uint8_t flag = 0;

#if RS485_CONTROL_TX_RX
	
	//RS485CON初始化
	RS485_CON_Init();
	
#endif
	
	while(1)
	{
		
		if(Flag_AC_DO)
		{
			if(Sensor_TakeAValue(TYPE_PH,REG_PH) == false)
			{
				rt_kprintf("take ph failed..\n");
				flag++;
			}
				
		
			if(Sensor_TakeAValue(TYPE_HUM,REG_HUM) == false)
			{
				rt_kprintf("take hum failed..\n");
				flag++;
			}
				
			
			if(Sensor_TakeAValue(TYPE_TEM,REG_TEM) == false)
			{
				rt_kprintf("take tem failed..\n");
				flag++;
			}
				
			
			if(Sensor_TakeAValue(TYPE_EC,REG_EC) == false)
			{
				rt_kprintf("take ec failed..\n");
				flag++;
			}
			
			if(flag == 4)
				Sensor_Connect = 0;
			else 
				Sensor_Connect = 1;
			
			flag = 0;
				
			
			rt_kprintf("\n");
			
			sprintf(buf,"PH:%.2fpH",debug_value._PH);
			rt_kprintf("%s\n",buf);
			
			sprintf(buf,"HUM:%.1f%%RH",debug_value._HUM);
			rt_kprintf("%s\n",buf);
			
			sprintf(buf,"TEM:%.1fC",debug_value._TEM);
			rt_kprintf("%s\n",buf);
			
			rt_kprintf("EC:%dus/cm\n\n",debug_value._EC);
			
		}
		
		rt_thread_mdelay(5000);//5s
		
	}
	
}

//任务2--ZIGBEE初始化
void WLAN_INIT(void *parameter)
{
	rt_err_t _flag;
	
	while(1)
	{
		if(A72_Init() == ERROR)
			rt_kprintf("A72_Init create failed..\n");
		else
		{
			rt_kprintf("A72_Init create successed..\n");
			A72_Device_Connect = 1;//设备连接成功标志位置1
			
			rt_thread_startup(&th3);
			rt_thread_startup(&th4);
			rt_thread_startup(&th5);
			rt_thread_startup(&th6);
			
			_flag = rt_thread_suspend(&th2);//挂起自身
				
			if(_flag == RT_EOK)
				rt_kprintf("th2 rt_thread_suspend successed..\n");
			else
				rt_kprintf("th2 rt_thread_suspend failed..\n");
		}
		rt_thread_mdelay(10000);
	}
}

//任务3--ZIGBEE检测入网状态(入网前开启)
void Zigbee_Network_Status_Cheak_front(void* paremeter)
{

	while(1)
    {
		if(A72_SEND_ORDER(&Connect_status,A72_Read_Connect_status) == ERROR)
		{
			
			rt_kprintf("A72_Read_Connect_status failed..\n");
			
			rt_thread_mdelay(5000);
		}
		else
		{
			
			rt_kprintf("A72_Read_Connect_status successed..\n");
			
			if(_value.S_Network == 0)//如果未连接 5秒后再次尝试
			{
				rt_kprintf("A72_Connect_status: OFF\n");
				
				rt_thread_mdelay(5000);
			}
			else//若连接了 挂起自身 同时开启发送函数
			{
				rt_kprintf("A72_Connect_status: ON\n");
				rt_kprintf("A72_Connect successed..\n");
				
				rt_sem_release(A72_Connect);
				
				
				A72_Cheak = rt_thread_suspend(&th3);//挂起自身
				
				if(A72_Cheak == RT_EOK)
					rt_kprintf("th3 rt_thread_suspend successed..\n");
				else
					rt_kprintf("th3 rt_thread_suspend failed..\n");
					
			 }
		}	
    }
}

//任务4--ZIGBEE检测入网状态(入网后开启)
void Zigbee_Network_Status_Cheak_after(void *parameter)
{
	while(1)
	{
		
		if(_value.S_Network == 1)//如果入网
		{
			
			rt_thread_mdelay(45000);//45s一检测
			
			rt_kprintf("Connection status Cheaking...\nOnce every 45s\n");
			
			if(A72_SEND_ORDER(&Connect_status,A72_Read_Connect_status) == ERROR)
			{
				rt_kprintf("Cheak Connect failed..\n");
			}
			else
			{
				rt_kprintf("Cheak Connect successed..\n");
				
				if(_value.S_Network == 0)//如果A72断开
				{
					rt_thread_resume(&th3);//恢复th3 A72入网线程
				}
				
			}
		}
		else
		{
			rt_thread_mdelay(3000);//位入网时3秒检测一次
		}
	}
}

//任务5--ZIGBEE发送数据
void Zigbee_Send_Data(void *parameter)
{
	
	rt_uint8_t zigbee_filed_number = 0;//zigbee异常次数
	
	rt_uint8_t send_buf[10] = {0}; //0x8a传数据
	send_buf[0] = NODE_BIT;        //数据首字节未节点未
	send_buf[1] = Zigbee_data;     //发送数据命令位

	while(1)
    {
		
		rt_sem_take(A72_Connect,RT_WAITING_FOREVER);//阻塞等待
		
		 rt_thread_mdelay(5000);//发送频率3s
		
		while(!Sensor_Connect)//如果传感器异常
		{
			rt_thread_mdelay(1000);//则延时等待
		}
		
		send_buf[2] = zigbee_value.PH >> 8;
		send_buf[3] = zigbee_value.PH & 255;
		send_buf[4] = zigbee_value.TEM >> 8;
		send_buf[5] = zigbee_value.TEM & 255;
		send_buf[6] = zigbee_value.HUM >> 8;
		send_buf[7] = zigbee_value.HUM & 255;
		send_buf[8] = zigbee_value.EC >> 8;
		send_buf[9] = zigbee_value.EC & 255;
		
		if(A72_SEND_DATA(_COORDINATOR,&A72_SEND_MODE_IEEE,send_buf,10) == SUCCESS)
		{
			rt_kprintf("A72_Send_Data successed..\n");
		}
		else
		{
			rt_kprintf("A72_Send_Data fialed..\n");
			zigbee_filed_number++;
		}

        rt_thread_mdelay(5000);//发送频率3s
		
		if(zigbee_filed_number > 5)
		{
			zigbee_filed_number = 0;
			/*
			如果zigbee发送失败次数大于5次 
			则认为zigbee异常 将重新初始化zigbee
			*/
			ZIGBEE_DAMAGE();
			
			rt_kprintf("\nZigbee Reinitialization\n");
		}
		
		if(_value.S_Network == 1)//如果Zigbee已经入网
			rt_sem_release(A72_Connect);//解锁
    }
}

//任务6--ZIGBEE(数据处理)
void Zigbee_Data_handle(void *parameter)
{
	while(1)
    {
		rt_sem_take(A72_Data_handle,RT_WAITING_FOREVER);//阻塞等待
		
		A72_HANDLE_DATA();
		
		rt_thread_mdelay(100);
		
    }
}
//任务7--功率检测
void Power_Metering(void *parameter)
{
	
	char buf[70];
	
	float voltage = 0;
	float current = 0;
	float power = 0;
	
	power_metering_init();//功率计量配置
	
	while(1)
	{
		
		voltage = (float)(adc_value[0]) / 4096 * 3.3 * 2;//电压转换
		
		current = (float)(adc_value[1]) / 4096 * 3.3 / 6;//电流转换
		
		power = voltage * current;//功率计算
		
		zigbee_value.POWER = (rt_uint16_t)(power * 1000);//以16位无符号数据存入Zigbee待发区
		
		sprintf(buf,"voltage:%.2f V\ncurrent:%.2f mA\npower  :%.2f W",voltage,current * 1000,power);
		
		rt_kprintf("\n\nPower_Metering..\n");
		
		rt_kprintf("%s\n\n",buf);//Debug
		
		rt_thread_mdelay(10000);//10s一检
		
	}
}
