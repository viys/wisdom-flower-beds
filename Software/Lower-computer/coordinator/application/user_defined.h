#ifndef __USER_DEFINED_H
#define __USER_DEFINED_H


#include "app.h"


/*
功能：对指针偏移地址后对应地址的值进行操作 
参数：type:偏移后地址对应的数据类型 pinter:指针 offsetaddr:偏移地址量
备注：该操作为危险操作
偏移量必须符合偏移前地址对应数据在内存中存储的地址偏移量
(不同的硬件控制器或操作系统对应的偏移量可能不同),
且必须保证偏移后的地址是有意义的
*/
#define REG_OFFSET(type,pinter,offsetaddr)  (*(type *)((uint32_t)(pinter) + offsetaddr))


#define Zigbee_data            0x8A        /*ZIGBEE数据命令*/
#define Zigbee_check           0x7A        /*ZIGBEE心跳命令*/


#define ZIGBEE_ERR_NUMBER      0x00        /*ZIGBEE序号错误*/
#define ZIGBEE_ERR_ORDER       0x01        /*ZIGBEE命令错误*/
#define ZIGBEE_SUCCESS         0x02        /*ZIGBEE数据正确*/




typedef struct 
{
	rt_uint16_t _Tem;
	rt_uint16_t _Hum;
	rt_uint16_t _Ph;
	rt_uint16_t _Ec;
}NODE_DATA;



/*节点状态*/
typedef struct
{
	rt_uint8_t NODE1;
	rt_uint8_t NODE2;
	rt_uint8_t NODE3;
	rt_uint8_t NODE4;
	rt_uint8_t NODE5;
	rt_uint8_t NODE6;
}ZIGBEE_NODE_STATE;

struct ZIGBEE_NODE_REPLY
{
	unsigned char state;       //数据状态
	unsigned char node;        //节点标号
	unsigned char type_reply;  //回应类型
	NODE_DATA *node_data;      //节点数据
};


//extern ALI_JSON ali_json;
extern char Json_Buf[1024];
extern ZIGBEE_NODE_STATE STATE_NODE;
extern NODE_DATA node1_data;
extern NODE_DATA node2_data;
extern NODE_DATA node3_data;
extern NODE_DATA node4_data;
extern NODE_DATA node5_data;
extern NODE_DATA node6_data;


void JSON_Upper(void);
void JSON_Model(void);
void SHUIfa_init(void);
struct ZIGBEE_NODE_REPLY ZIGBEE_USER_DEFINE(unsigned char *node_flag);



#endif

