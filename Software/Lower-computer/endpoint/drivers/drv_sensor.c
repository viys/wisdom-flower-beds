#include "drv_sensor.h"


rt_device_t Sensor_dev;


bool Flag_AC_DO = true;

//读ph
static void take_ph(void)
{
	bool result = Sensor_TakeAValue(TYPE_PH,REG_PH);
	if(result == true)
	{
		char buf[30];
		sprintf(buf,"PH:%.2fpH",debug_value._PH);
		rt_kprintf("%s\n",buf);
	}
	else
	{
		rt_kprintf("take failed..\n");
	}


}

//读湿度
static void take_hum(void)
{
	bool result = Sensor_TakeAValue(TYPE_HUM,REG_HUM);
	if(result == true)
	{
		char buf[30];
		sprintf(buf,"HUM:%.1f%%RH",debug_value._HUM);
		rt_kprintf("%s\n",buf);
	}
	else
	{
		rt_kprintf("take failed..\n");
	}

}
//读温度
static void take_tem(void)
{
	bool result = Sensor_TakeAValue(TYPE_TEM,REG_TEM);
	if(result == true)
	{
		char buf[30];
		sprintf(buf,"TEM:%.1fC",debug_value._TEM);
		rt_kprintf("%s\n",buf);
	}
	else
	{
		rt_kprintf("take failed..\n");
	}

}

//读电导率
static void take_ec(void)
{
	bool result = Sensor_TakeAValue(TYPE_EC,REG_EC);
	if(result == true)
	{
		rt_kprintf("EC:%dus/cm\n",debug_value._EC);
	}
	else
	{
		rt_kprintf("take failed..\n");
	}

}

//读从机地址
static void take_add(void)
{
	bool result = Sensor_TakeAValue(TYPE_ADD,REG_ADD);
	if(result == true)
	{
		rt_kprintf("adder:%x\n",debug_value._ADD);
	}
	else
	{
		rt_kprintf("take failed..\n");
	}
}

//读波特率
static void take_baud(void)
{
	bool result = Sensor_TakeAValue(TYPE_BAUD,REG_BAUD);
	if(result == true)
	{
		rt_kprintf("baud:%d\n",debug_value._BAUD);
	}
	else
	{
		rt_kprintf("take failed..\n");
	}
}


//读任意寄存器（例：take_ 0013 则表示读0013寄存器）
static void take_(int argc,char**argv)
{
	if(argc < 2)
	{
		rt_kprintf("Please enter the starting address(4 bit) for example:0001\n");
		return;
	}
	
	int len = strlen(argv[1]);
	
	if(len != 4)
	{
		rt_kprintf("erron!\nPlease enter the starting address(4 bit) for example:0001\n");
		return;
	}
	
	
    rt_uint16_t numer[4] = {0};
	rt_uint16_t address = 0;
	for(rt_uint8_t i = 0;i < 4;i ++)
	{
		numer[i] = (rt_uint16_t)(argv[1][i] - '0');
		if(numer[i] <= 9)
		{
			address |= (numer[i] << (4 * (4 - 1 - i)));
		}	
		else if(numer[i] >= 49 && numer[i] <= 54)
		{
			numer[i] -= 39;
			address |= (numer[i] << (4 * (4 - 1 - i)));
		}
		else
		{
			rt_kprintf("erron!\nPlease enter the starting address(4 bit) for example:0001\n");
			return;
		}
		
		
	}
	
	
	if(Sensor_TakeAValue(TYPE_FREE,address) == true)
	{
		rt_kprintf("%xreg:value:%d\n",address,debug_value._FREE);
	}
	else
	{
		rt_kprintf("take failed..\n");
	}
}
//修改单个寄存器的值（例：make_ 0001 0005 把0001寄存器的值修改为0x0005）
static void make_(int argc,char**argv)
{
	
	if(argc < 2)
	{
		rt_kprintf("Please enter address & data for example:0001 0007\n");
		return;
	}
	
	int len1 = strlen(argv[1]);
	int len2 = strlen(argv[2]);
	
	
	if((len1 != 4) || (len2 != 4))
	{
		rt_kprintf("erron:number\nPlease enter address & data for example:0001 0007\n");
		return;
	}
	
	
    rt_uint16_t numer[8] = {0};
	rt_uint16_t address = 0;
	rt_uint16_t data = 0;
	for(rt_uint8_t i = 0;i < 8;i ++)
	{
		if(i < 4)
			numer[i] = (rt_uint16_t)(argv[1][i] - '0');
		else
			numer[i] = (rt_uint16_t)(argv[2][i - 4] - '0');
		
		if(numer[i] <= 9)
		{
			if(i < 4)
				address |= (numer[i] << (4 * (4 - 1 - i)));
			else
				data |= (numer[i] << (4 * (8 - 1 - i)));
		}	
		else if(numer[i] >= 49 && numer[i] <= 54)
		{
			numer[i] -= 39;
			if(i < 4)
				address |= (numer[i] << (4 * (4 - 1 - i)));
			else
				data |= (numer[i] << (4 * (8 - 1 - i)));
		}
		else
		{
			rt_kprintf("erron:format\nPlease enter address & data for example:0001 0007\n");
			return;
		}
		
		
	}
	
	int result;
	result = Sensor_MakeAValue(address,data);
	switch(result)//返回类型判断
	{
		case -999:
			rt_kprintf("make failed..(no reply!!!)\n");
			return;
		case modbus_reply_h06:
			rt_kprintf("make successed..\n");
		    return;
		case modbus_reply_cfailed:
			rt_kprintf("make failed..\n");
			rt_kprintf("The reason may be that the register is read-only or does not exist\n");
			return;
		case modbus_reply_data_err:
			rt_kprintf("make failed..\n");
			rt_kprintf("Data error, possibly due to CRC verification error or other reasons\n");
			return;
		default:
			rt_kprintf("make failed..(unknown error!!!)\n");
			return;
	}
	
}

//启动和停止  th1(自动采集传感器数值)
static void AC_DO(void)
{
	
	if(!Flag_AC_DO)
	{
		Flag_AC_DO = true;
		
	}
	else
	{
		Flag_AC_DO = false;
	}
		
}

static void led_fan(void)
{
	if(gpio_input_bit_get(GPIOC,GPIO_PIN_7) == 0)
	{
		gpio_bit_set(GPIOC,GPIO_PIN_7);
		rt_kprintf("pin7 on!\n");
	}
		
	else if(gpio_input_bit_get(GPIOC,GPIO_PIN_7) == 1)
	{
		gpio_bit_reset(GPIOC,GPIO_PIN_7);
		rt_kprintf("pin7 off!\n");
	}
		
}



int rt_sensor_init(void)
{
	rt_device_t sensor_dev;
	sensor_dev = rt_device_create(RT_Device_Class_Block,128);
	if(sensor_dev == RT_NULL)
	{
		LOG_E("sensor_dev rt_device_create failed..\n");
		return -RT_ENOMEM;
	}	
	rt_device_register(sensor_dev,"sensor",RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);//注册  读写
	return 0;
}

INIT_DEVICE_EXPORT(rt_sensor_init);

MSH_CMD_EXPORT(take_ph,only take ph);
MSH_CMD_EXPORT(take_hum,only take hum);
MSH_CMD_EXPORT(take_tem,only take tem);
MSH_CMD_EXPORT(take_ec,only take ec);
MSH_CMD_EXPORT(take_add,take address);
MSH_CMD_EXPORT(take_baud,take uart baud);
MSH_CMD_EXPORT(take_,Please enter the starting address 4 bit);
MSH_CMD_EXPORT(make_,Please enter address & data);
MSH_CMD_EXPORT(AC_DO,Whether to enable AC);
MSH_CMD_EXPORT(led_fan,!! dodododood);
