#include "24C02.H"
#include "iic.h"

void Write_Byte_24C02(unsigned char addr,unsigned  char dat)
{
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	
	IIC_SendByte(addr);
	IIC_WaitAck();
	
	IIC_SendByte(dat);
	IIC_SendAck(1);
	
	IIC_Stop();
}

unsigned char Read_Byte_24C02(unsigned char addr)
{
	unsigned char dat;
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	
	IIC_SendByte(addr);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xA1);
	IIC_WaitAck();
	
	dat = IIC_RecByte();
	IIC_SendAck(1);
	
	IIC_Stop();
	return dat;
}