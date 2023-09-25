#ifndef __POWER_METERING_H
#define __POWER_METERING_H

#include "gd32f10x.h"
#include <rtthread.h>
#include "systick.h"
#include <stdio.h>


extern unsigned short adc_value[2];


void power_metering_init(void);


#endif /* __POWER_METERING_H */
