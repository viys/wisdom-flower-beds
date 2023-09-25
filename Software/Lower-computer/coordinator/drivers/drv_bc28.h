#ifndef __DRV_BC28_H
#define __DRV_BC28_H


#include <app.h>

/*变量声明*/
extern rt_device_t BC28_dev;
extern struct rt_timer bc28_10ms;

/*函数声明*/
void bc28_callback(void *parameter);
void BC28_SEND_DATA(char *fmt,...);


#endif


