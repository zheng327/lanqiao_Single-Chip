#include "AT24C02.H"
#include "iic.h"
#include "my.h"
#include "STC15F2K60S2.H"

void AT24C02_Write_Byte(unsigned char addr,unsigned char dat)
{
	EA = 0;
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	
	IIC_SendByte(addr);
	IIC_WaitAck();
	
	IIC_SendByte(dat);
	IIC_WaitAck();
	
	IIC_Stop();
	EA = 1;
	Delay(20);
}

unsigned char AT24C02_Read_Byte(unsigned char addr)
{
	unsigned char dat;
	EA = 0;
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
	EA = 1;
	
	return dat;
}