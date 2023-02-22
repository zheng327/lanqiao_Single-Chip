#include "PCF8591.H"
#include "iic.h"

unsigned char PCF8591_AD(unsigned char channel)
{
	unsigned char dat;
	
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	
	IIC_SendByte(channel);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	
	dat = IIC_RecByte();
	IIC_SendAck(1);
	
	IIC_Stop();
	
	return dat;
}

void PCF8591_DA(unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	
	IIC_SendByte(0x40);
	IIC_WaitAck();
	
	IIC_SendByte(dat);
	IIC_WaitAck();
	
	IIC_Stop();
}