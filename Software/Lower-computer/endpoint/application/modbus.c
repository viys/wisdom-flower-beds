/*                                                    
	                                                    使用说明
												单片机作主机，传感器作从机
								该程序中的MODBUS协议并不完善，也并不严谨，其实现的功能只能适用于本程序
								用户不可直接调用本程序中的MODBUS协议去执行其他工程，否则会导致各种错误

*/

#include "modbus.h"


rt_sem_t modbus_respond;
rt_timer_t modbus_timer;

modbus_u8 modbus_rx_buf[modbus_rx_max_len] = {0};
modbus_u8 modbus_rx_falg = modbus_rx_off;
modbus_u16 modbus_rxdata_len = 0;
modbus_u32 modbus_time = 0;
modbus_u32 modbus_time_interval = (modbus_u32)(modbus_ms_interval + 1);

//用户自定义
modbus_u16 Modbus_DataBuff[8] = {0};

/*
函数功能：主机向从机查询寄存器数据
参数：slave_address：查询的从机地址 start_address：查询寄存器的开始地址 reg_number：查询的寄存器个数
返回值：void
备注：功能码：0x03
*/
void MODBUS_HostToSlave_H03(modbus_u8 slave_address,modbus_u16 start_address,modbus_u16 reg_number)
{
	modbus_u8 tx_buf[8] = {0};
	modbus_u16 crc = 0;
	
	tx_buf[0] = slave_address;
	tx_buf[1] = 0x03;
	tx_buf[2] = start_address >> 8;
	tx_buf[3] = start_address & 255;
	tx_buf[4] = reg_number >> 8;
	tx_buf[5] = reg_number & 255;
	crc = CRC_Verification(tx_buf,6);
	tx_buf[6] = crc >> 8;
	tx_buf[7] = crc & 255;
	
	modbus_send_data(tx_buf,8);
	
}

/*
函数功能：主机修改从机单个寄存器数据
参数：slave_address：修改的从机地址 start_address：修改的寄存器的开始地址 data：修改的数据内容
返回值：void
备注：功能码：0x06
*/
void MODBUS_HostToSlave_H06(modbus_u8 slave_address,modbus_u16 start_address,modbus_u16 data)
{
	modbus_u8 tx_buf[8] = {0};
	modbus_u16 crc = 0;
	
	
	tx_buf[0] = slave_address;
	tx_buf[1] = 0x06;
	tx_buf[2] = start_address >> 8;
	tx_buf[3] = start_address & 255;
	tx_buf[4] = data >> 8;
	tx_buf[5] = data & 255;
	crc = CRC_Verification(tx_buf,6);
	tx_buf[6] = crc >> 8;
	tx_buf[7] = crc & 255;
	
	modbus_send_data(tx_buf,8);
	
	
}
/*
函数功能：主机处理从机的回应
参数：slave_address：处理的从机的地址 
返回值：modbus_32
备注：
modbus_reply_h03：03功能码查找操作成功 且返回了数据
modbus_reply_h06：06功能码修改寄存器成功
modbus_reply_noreg：03功能码查找操作成功 但并无查找的寄存器
modbus_reply_cfailed：06功能码修改失败 原因可能为:该寄存器只读或不存在的寄存器等
modbus_reply_addr_err：从机地址错误
modbus_reply_data_err：数据错误
modbus_reply_err：未知错误
*/
modbus_32 MODBUS_Handle_Reply(modbus_u8 slave_address)
{
	
	modbus_u8 len = 0;
	
	//crc校验
	modbus_u16 crc = (modbus_rx_buf[modbus_rxdata_len - 2] << 8) | modbus_rx_buf[modbus_rxdata_len - 1];
	if(crc != CRC_Verification(modbus_rx_buf,modbus_rxdata_len - 2))
		return modbus_reply_data_err;
	
	//地址位判断
	if(modbus_rx_buf[0] != slave_address)
		return modbus_reply_addr_err;
	
	//功能码判断
	switch(modbus_rx_buf[1])
	{
		case 0x03:
			memset(Modbus_DataBuff,0,8);
			len = modbus_rx_buf[2] / 2; 
			for(modbus_u8 i = 0;i < len;i ++)
			{
				Modbus_DataBuff[i] = (modbus_rx_buf[3 + (i * 2)] << 8) | modbus_rx_buf[4 + (i * 2)];
			}
			return modbus_reply_h03;
		case 0x06:
			return modbus_reply_h06;
		case 0x83:
			return modbus_reply_noreg;
		case 0x86:
			return modbus_reply_cfailed;
		default:
			return modbus_reply_err;
	
	}
	
}


/*
函数功能：CRC校验
参数：buff：数据帧除去CRC校验后剩余的部分 len：剩余部分的长度
返回值：16位的CRC校验值
备注：
*/
int CRC_Verification(uint8_t *buff,int len)
{
	unsigned short tmp = 0xffff;
    unsigned short ret1 = 0;
   

    for (int n = 0; n < len; n++) {/*此处的n -- 要校验的位数为n个*/
        tmp = buff[n] ^ tmp;
        for (int i = 0; i < 8; i++) {  /*此处的8 -- 指每一个char类型又8bit，每bit都要处理*/
            if (tmp & 0x01) {
                tmp = tmp >> 1;
                tmp = tmp ^ 0xa001;
            }
            else {
                tmp = tmp >> 1;
            }
        }
    }
    /*CRC校验后的值*/
    //printf("%X\n", tmp);
    /*将CRC校验的高低位对换位置*/
    ret1 = tmp >> 8;
    ret1 = ret1 | (tmp << 8);
    //printf("ret: %X\n", ret1);
	return ret1;

}

/*
函数功能：modbus_tmer 回调函数
参数：  void *parameter
返回值：void
备注：
以帧间隔为3.5个字节来判断上一帧数据传输的完成
*/
void modbus_callback(void *parameter)
{
	modbus_time ++ ;
	if(modbus_time > modbus_time_interval && modbus_rx_falg == modbus_rx_on)
	{
		modbus_rx_falg = modbus_rx_ok;
		rt_sem_release(modbus_respond);
		
	}
		
}
