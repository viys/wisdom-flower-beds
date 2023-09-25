#ifndef __USERCONFIG_H
#define __USERCONFIG_H

/*
节点选择
使用不一样的节点时 需更改此宏定义
节点：_NODE1
      _NODE2
	  _NODE3
	  _NODE4
	  _NODE5
	  _NODE6
*/
#define NODE_BIT            _NODE2         

#define Zigbee_data            0x8A        /*ZIGBEE数据命令*/
#define Zigbee_check           0x7A        /*ZIGBEE心跳命令*/

/*
如果485模块不支持自收发
需要手动控制CON口控制收发
则开启以下宏定义
*/
#define RS485_CONTROL_TX_RX       0           


/*ZIGBEE传输参数*/
typedef struct
{
	unsigned short PH;    //PH
	unsigned short HUM;   //湿度
	unsigned short TEM;   //温度
	unsigned short EC;    //电导率
	unsigned short POWER; //功率
}STRUCT_ZIGBEETypeDef;


#endif

