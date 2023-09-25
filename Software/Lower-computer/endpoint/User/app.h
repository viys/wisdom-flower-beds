#ifndef __APP_H_
#define __APP_H_

#include "gd32f10x.h"
#include <rtthread.h>
#include <rtdbg.h>
#include "string.h"
#include "stdbool.h"
#include "userconfig.h"
#include "drv_usart.h"
#include "drv_sensor.h"
#include "drv_a72.h"
#include "sensor.h"
#include "modbus.h"
#include "zigbee_a72.h"
#include "Power_metering.h"
    



#define th1_test_priority   20
#define th2_test_priority   24
#define th3_test_priority   21
#define th4_test_priority   20
#define th5_test_priority   22
#define th6_test_priority   19
#define th7_test_priority   23

//变量声明
extern struct rt_thread th1;
extern rt_uint8_t th1_stack[512];

extern struct rt_thread th2;
extern rt_uint8_t th2_stack[512];

extern struct rt_thread th3;
extern rt_uint8_t th3_stack[512];

extern struct rt_thread th4;
extern rt_uint8_t th4_stack[512];

extern struct rt_thread th5;
extern rt_uint8_t th5_stack[512];

extern struct rt_thread th6;
extern rt_uint8_t th6_stack[512];

extern struct rt_thread th7;
extern rt_uint8_t th7_stack[1024];

extern rt_sem_t A72_Respond;
extern rt_sem_t A72_Connect;
extern rt_sem_t A72_Data_handle;

extern rt_uint8_t A72_Device_Connect;
extern rt_err_t A72_Cheak;
extern rt_uint8_t Sensor_Connect;
extern STRUCT_ZIGBEETypeDef  zigbee_value;

//函数声明
void rt_hw_us_delay(rt_uint32_t us);
void Sensor_Debug(void *parameter);
void WLAN_INIT(void *parameter);
void Zigbee_Network_Status_Cheak_front(void* paremeter);
void Zigbee_Network_Status_Cheak_after(void *parameter);
void Zigbee_Send_Data(void *parameter);
void Zigbee_Data_handle(void *parameter);
void Power_Metering(void *parameter);

#endif
