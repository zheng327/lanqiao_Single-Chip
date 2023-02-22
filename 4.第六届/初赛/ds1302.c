/*
  程序说明: DS1302驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台 8051，12MHz
  日    期: 2011-8-9
*/

#include <reg52.h>
#include <intrins.h>
#include "ds1302.h"

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;   // DS1302复位												

void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK=0;
		SDA=temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}

void Write_Time_Ds1302(unsigned char *p)
{
	Write_Ds1302_Byte(0x8E,0x00);
	Write_Ds1302_Byte(0x8C,*p/10*16 + *p%10);
	Write_Ds1302_Byte(0x8A,*(p+1)/10*16 + *(p+1)%10);
	Write_Ds1302_Byte(0x88,*(p+2)/10*16 + *(p+2)%10);
	Write_Ds1302_Byte(0x86,*(p+3)/10*16 + *(p+3)%10);
	Write_Ds1302_Byte(0x84,*(p+4)/10*16 + *(p+4)%10);
	Write_Ds1302_Byte(0x82,*(p+5)/10*16 + *(p+5)%10);
	Write_Ds1302_Byte(0x80,*(p+6)/10*16 + *(p+6)%10);
	Write_Ds1302_Byte(0x8E,0x80);
}

void Read_Time_Ds1302(unsigned char *p)
{
	*p = Read_Ds1302_Byte(0x8D)/16*10 + Read_Ds1302_Byte(0x8D)%16;
	*(p+1) = Read_Ds1302_Byte(0x8B)/16*10 + Read_Ds1302_Byte(0x8B)%16;
	*(p+2) = Read_Ds1302_Byte(0x89)/16*10 + Read_Ds1302_Byte(0x89)%16;
	*(p+3) = Read_Ds1302_Byte(0x87)/16*10 + Read_Ds1302_Byte(0x87)%16;
	*(p+4) = Read_Ds1302_Byte(0x85)/16*10 + Read_Ds1302_Byte(0x85)%16;
	*(p+5) = Read_Ds1302_Byte(0x83)/16*10 + Read_Ds1302_Byte(0x83)%16;
	*(p+6) = Read_Ds1302_Byte(0x81)/16*10 + Read_Ds1302_Byte(0x81)%16;
}