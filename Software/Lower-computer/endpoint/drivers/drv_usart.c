#include "drv_usart.h"

rt_device_t u1_dev;
rt_device_t u2_dev;

rt_uint8_t USART2_RX_BUF[64] = {0};
rt_uint16_t rs485_data_len = 0;

void usart0_init(uint32_t brr)
{
	//时钟配置
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART0);
	
	//GPIO模式配置
	gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);//TX-PA9
	gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_10);//RX-PA10
	
	//串口工作模式配置
	usart_deinit(USART0);//复位外设USART
	usart_baudrate_set(USART0,brr);//配置波特率
	usart_word_length_set(USART0,USART_WL_8BIT);//8位数据位
	usart_stop_bit_set(USART0,USART_STB_1BIT);//1个停止位
	usart_parity_config(USART0,USART_PM_NONE);//无奇偶校验
	usart_hardware_flow_cts_config(USART0,USART_CTS_DISABLE);//无硬件控制流
	usart_hardware_flow_rts_config(USART0,USART_RTS_DISABLE);//无硬件控制流
	usart_receive_config(USART0,USART_RECEIVE_ENABLE);//接收使能
	usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);//发送使能
	
	
	
	//使能串口
	usart_enable(USART0);
	
}

void usart1_init(uint32_t brr)
{
	//时钟配置
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART1);
	
	//GPIO模式配置
	gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_2);//TX-PA2
	gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_3);//RX-PA3
	
	//串口工作模式配置
	usart_deinit(USART1);//复位外设USART
	usart_baudrate_set(USART1,brr);//配置波特率
	usart_word_length_set(USART1,USART_WL_8BIT);//8位数据位
	usart_stop_bit_set(USART1,USART_STB_1BIT);//1个停止位
	usart_parity_config(USART1,USART_PM_NONE);//无奇偶校验
	usart_hardware_flow_cts_config(USART1,USART_CTS_DISABLE);//无硬件控制流
	usart_hardware_flow_rts_config(USART1,USART_RTS_DISABLE);//无硬件控制流
	usart_receive_config(USART1,USART_RECEIVE_ENABLE);//接收使能
	usart_transmit_config(USART1,USART_TRANSMIT_ENABLE);//发送使能
	
	//中断配置
	nvic_irq_enable(USART1_IRQn,0,0);//中断使能
	usart_interrupt_enable(USART1,USART_INT_RBNE);//接收中断使能
	usart_interrupt_enable(USART1,USART_INT_IDLE);//IDLE中断使能
	
	//使能串口
	usart_enable(USART1);
	
}


void USART0_Send_Data(uint8_t *data,uint16_t len)
{
	rt_uint16_t i;
	for(i=0;i<len;i++)
	{
		usart_data_transmit(USART0, data[i]);
		while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
	}
}

void Debug_printf(char *fmt,...)
{
	static __align(8) unsigned char USART0_buffer[128];
	va_list ap;
	va_start(ap,fmt);
	vsnprintf((char *)USART0_buffer,2000,fmt,ap);
	va_end(ap);
	USART0_Send_Data(USART0_buffer,strlen((const char*)USART0_buffer));

}

void USART1_Send_Data(uint8_t *data,uint16_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		usart_data_transmit(USART1, data[i]);
		while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
	}
}


//串口1中断服务函数
void USART1_IRQHandler(void)
{
	uint8_t res;
	static uint8_t count = 0;
	if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_RBNE) != RESET)
	{
		usart_interrupt_flag_clear(USART1,USART_INT_FLAG_RBNE);//清标志位
		res = usart_data_receive(USART1);//读取数据
		A72_RX_BUF[count++] = res;//存入数组
//		usart_data_transmit(USART0,res);//Debug
	}
	if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_IDLE) != RESET)
	{
		res = usart_data_receive(USART1);
		count = 0;//计数值复位
		if(A72_RX_BUF[0] == 0x55 && A72_RX_BUF[1] == 0xFF && A72_RX_BUF[2] == 0x01)
			rt_sem_release(A72_Data_handle);//接收回应完成标志 释放处理数据信号量
		else
			rt_sem_release(A72_Respond);//接收回应完成标志 释放回应信号量
		
	}


}

int rt_uart1_init(void)
{
	rt_device_t U1_dev;
	U1_dev = rt_device_create(RT_Device_Class_Char,64);
	if(U1_dev == RT_NULL)
	{
		LOG_E("u1_dev rt_device_create failed..\n");
		return -RT_ENOMEM;
	}	
	usart1_init(115200);
	rt_device_register(U1_dev,"uart1",RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);//注册  读写
	return 0;
}

INIT_DEVICE_EXPORT(rt_uart1_init);



void usart2_init(uint32_t brr)
{
	//时钟配置
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_USART2);
	
	//GPIO模式配置
	gpio_init(GPIOB,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);//TX-PB10
	gpio_init(GPIOB,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_11);//RX-PB11
	
	//串口工作模式配置
	usart_deinit(USART2);//复位外设USART
	usart_baudrate_set(USART2,brr);//配置波特率
	usart_word_length_set(USART2,USART_WL_8BIT);//8位数据位
	usart_stop_bit_set(USART2,USART_STB_1BIT);//1个停止位
	usart_parity_config(USART2,USART_PM_NONE);//无奇偶校验
	usart_hardware_flow_cts_config(USART2,USART_CTS_DISABLE);//无硬件控制流
	usart_hardware_flow_rts_config(USART2,USART_RTS_DISABLE);//无硬件控制流
	usart_receive_config(USART2,USART_RECEIVE_ENABLE);//接收使能
	usart_transmit_config(USART2,USART_TRANSMIT_ENABLE);//发送使能
	
	//中断配置
	nvic_irq_enable(USART2_IRQn,0,0);//中断使能
	usart_interrupt_enable(USART2,USART_INT_RBNE);//接收中断使能
	
	//使能串口
	usart_enable(USART2);
	
}
//串口2printf重定向
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART2, (uint8_t)ch);
    while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
    return ch;
}

void USART2_Send_Data(uint8_t *data,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		usart_data_transmit(USART2, data[i]);
		while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
	}
}

//串口2中断服务函数
void USART2_IRQHandler(void)
{
	uint8_t res;
	if(usart_interrupt_flag_get(USART2,USART_INT_FLAG_RBNE) != RESET)
	{
		usart_interrupt_flag_clear(USART2,USART_INT_FLAG_RBNE);//清标志位
		
		modbus_rx_falg = modbus_rx_on;
		modbus_time = 0;
		res = usart_data_receive(USART2);//读取数据
		modbus_rx_buf[modbus_rxdata_len++] = res;
		
	}
}

int rt_usart2_init(void)
{
	rt_device_t U2_dev;
	U2_dev = rt_device_create(RT_Device_Class_Char,64);
	if(U2_dev == RT_NULL)
	{
		LOG_E("u2_dev rt_device_create failed..\n");
		return -RT_ENOMEM;
	}	
	usart2_init(modbus_uart_baud);
	rt_device_register(U2_dev,"usart2",RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);//注册  读写
	return 0;
}

INIT_DEVICE_EXPORT(rt_usart2_init);




