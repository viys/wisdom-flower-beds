#include "drv_demo.h"

rt_device_t dev;

rt_err_t demo_init(rt_device_t dev)
{
	
	rcu_periph_clock_enable(RCU_GPIOC);//开GPIOA时钟
	
	gpio_init(GPIOC,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_7);//GPIO工作模式配置
	gpio_init(GPIOC,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_0);//GPIO工作模式配置
	
	gpio_bit_reset(GPIOC,GPIO_PIN_0);
	
	//gpio_bit_reset(GPIOC,GPIO_PIN_7);
	
	rt_kprintf("demo_init..\n");
	return 0;
}
rt_err_t demo_open(rt_device_t dev, rt_uint16_t oflag)
{
	rt_kprintf("demo_open..\n");
	return 0;
}
rt_err_t demo_close(rt_device_t dev)
{
	rt_kprintf("demo_close..\n");
	return 0;
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

static void pb14(void)
{
	if(gpio_input_bit_get(GPIOB,GPIO_PIN_14) == 0)
	{
		gpio_bit_set(GPIOB,GPIO_PIN_14);
		rt_kprintf("pin14 on!\n");
	}
		
	else if(gpio_input_bit_get(GPIOB,GPIO_PIN_14) == 1)
	{
		gpio_bit_reset(GPIOB,GPIO_PIN_14);
		rt_kprintf("pin14 off!\n");
	}
		
}

static void pb13(void)
{
	if(gpio_input_bit_get(GPIOB,GPIO_PIN_13) == 0)
	{
		gpio_bit_set(GPIOB,GPIO_PIN_13);
		rt_kprintf("pin13 on!\n");
	}
		
	else if(gpio_input_bit_get(GPIOB,GPIO_PIN_13) == 1)
	{
		gpio_bit_reset(GPIOB,GPIO_PIN_13);
		rt_kprintf("pin13 off!\n");
	}
		
}

static void pb12(void)
{
	if(gpio_input_bit_get(GPIOB,GPIO_PIN_12) == 0)
	{
		gpio_bit_set(GPIOB,GPIO_PIN_12);
		rt_kprintf("pin12 on!\n");
	}
		
	else if(gpio_input_bit_get(GPIOB,GPIO_PIN_12) == 1)
	{
		gpio_bit_reset(GPIOB,GPIO_PIN_12);
		rt_kprintf("pin12 off!\n");
	}
		
}

static void pb1(void)
{
	if(gpio_input_bit_get(GPIOB,GPIO_PIN_1) == 0)
	{
		gpio_bit_set(GPIOB,GPIO_PIN_1);
		rt_kprintf("pin1 on!\n");
	}
		
	else if(gpio_input_bit_get(GPIOB,GPIO_PIN_1) == 1)
	{
		gpio_bit_reset(GPIOB,GPIO_PIN_1);
		rt_kprintf("pin1 off!\n");
	}
		
}

static void pb0(void)
{
	if(gpio_input_bit_get(GPIOB,GPIO_PIN_0) == 0)
	{
		gpio_bit_set(GPIOB,GPIO_PIN_0);
		rt_kprintf("pin0 on!\n");
	}
		
	else if(gpio_input_bit_get(GPIOB,GPIO_PIN_0) == 1)
	{
		gpio_bit_reset(GPIOB,GPIO_PIN_0);
		rt_kprintf("pin0 off!\n");
	}
		
}

static void pc5(void)
{
	if(gpio_input_bit_get(GPIOC,GPIO_PIN_5) == 0)
	{
		gpio_bit_set(GPIOC,GPIO_PIN_5);
		rt_kprintf("pin5 on!\n");
	}
		
	else if(gpio_input_bit_get(GPIOC,GPIO_PIN_5) == 1)
	{
		gpio_bit_reset(GPIOC,GPIO_PIN_5);
		rt_kprintf("pin5 off!\n");
	}
		
}



static void rs485_test(void)
{
	uint8_t buf[8] = {0x01,0x03,0x00,0x06,0x00,0x01,0x64,0x0B};
	UART3_Send_Data(buf,8);
}

int rt_demo_init(void)
{
	rt_device_t demo_dev;
	demo_dev = rt_device_create(RT_Device_Class_Char,32);
	if(demo_dev == RT_NULL)
	{
		LOG_E("rt_device_create failed..\n");
		return -RT_ENOMEM;
	}
	demo_dev->init = demo_init;
	demo_dev->open = demo_open;
	demo_dev->close = demo_close;
	
	rt_device_register(demo_dev,"demo",RT_DEVICE_FLAG_RDWR);//注册  读写
	
	return 0;
}

INIT_DEVICE_EXPORT(rt_demo_init);

MSH_CMD_EXPORT(led_fan,!! dodododood);

MSH_CMD_EXPORT(pb14,!! dodododood);
MSH_CMD_EXPORT(pb13,!! dodododood);
MSH_CMD_EXPORT(pb12,!! dodododood);
MSH_CMD_EXPORT(pb1,!! dodododood);
MSH_CMD_EXPORT(pb0,!! dodododood);
MSH_CMD_EXPORT(pc5,!! dodododood);

MSH_CMD_EXPORT(rs485_test,!! rs485 test);

