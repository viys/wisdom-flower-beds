#ifndef __DRV_DEMO_H_
#define __DRV_DEMO_H_

#include "gd32f10x.h"
#include <rtthread.h>
#include <rtdbg.h>
#include <app.h>

extern rt_device_t dev;

int rt_demo_init(void);
rt_err_t demo_init(rt_device_t dev);
rt_err_t demo_open(rt_device_t dev, rt_uint16_t oflag);
rt_err_t demo_close(rt_device_t dev);

#endif
