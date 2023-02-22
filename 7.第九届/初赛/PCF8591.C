#include "iic.h"
#include "PCF8591.H"

unsigned char AD_PCF8591(unsigned char channel)
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