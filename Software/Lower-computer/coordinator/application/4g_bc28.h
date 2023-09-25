#ifndef __4G_BC28_H_
#define __4G_BC28_H_

#include "app.h"

//#define ProductKey "i3uoHQmkf3Q"						
//#define DeviceName "HPU_JS_huatan"					
//#define DeviceSecret "5f54cfea6086cc4afddfac2c3f7155ce" 

//#define Domain "139.196.135.135"
//#define Port "1883"														                    
//#define ConnectID "HPU_JS_huatan"	
//#define PubTopic "/sys/i3uoHQmkf3Q/HPU_JS_huatan/thing/event/property/post"			      
//#define SubTopic "/i3uoHQmkf3Q/HPU_JS_huatan/user/get" 	


#define ProductKey "i3uo702HuDV"						
#define DeviceName "demo1"					
#define DeviceSecret "2ab0756a50b13e6244abfd24a27cb73e" 

#define Domain "139.196.135.135"
#define Port "1883"														                    
#define ConnectID "demo1"	
#define PubTopic "/i3uo702HuDV/demo1/user/update"			      
#define SubTopic "/i3uo702HuDV/demo1/user/get" 


#define Param_Post1  "airTem"			                            
#define Param_Post2  "airHum"	                                
#define Param_Post3  "PM1"
#define Param_Post4  "PM25"
#define Param_Post5  "PM10"
#define Param_Post6  "atmospheric_PM1"
#define Param_Post7  "atmospheric_PM25"
#define Param_Post8  "atmospheric_PM10"
#define Param_Post9  "TSP_0_3um"
#define Param_Post10 "TSP_0_5um"
#define Param_Post11 "TSP_1_0um"
#define Param_Post12 "TSP_2_5um"
#define Param_Post13 "TSP_5_0um"
#define Param_Post14 "TSP_10um"
#define Param_Post15 "TVOC"
#define Param_Post16 "co2"
#define Param_Post17 "HCHO"

#define Param_Post18 "NODE1:soilPH"
#define Param_Post19 "NODE1:soilHumidity"
#define Param_Post20 "NODE1:soilTemperature"
#define Param_Post21 "NODE1:EC"

#define Param_Post22 "NODE2:soilPH"
#define Param_Post23 "NODE2:soilHumidity"
#define Param_Post24 "NODE2:soilTemperature"
#define Param_Post25 "NODE2:EC"


#define BC28_RX_ON    0
#define BC28_RX_OFF   1
#define BC28_RX_OK    2


//变量声明
extern rt_uint8_t BC28_RX_BUF[200];
extern rt_uint8_t BC28_RX_FLAG;
extern rt_uint16_t BC28_RX_DATA_LEN;


//函数声明
int BC28_Init(void);
int MQTT_Init(void);
int MQTT_PUB(char *data);

#endif
