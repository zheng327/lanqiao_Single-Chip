#include "PCF8591.H"
#include "STC15F2K60S2.H"
#include "iic.h"

unsigned char PCF8591_AD(unsigned char channel)
{
	unsigned char dat;
	
	EA = 0;
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	
	IIC_SendByte(channel);
	IIC_WaitAck();
	
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	
	dat = IIC_RecByte();
	IIC_SendAck(1);
	
	IIC_Stop();
	EA = 1;
	
	return dat;
}