#include "STC15F2K60S2.H"
#include "my.h"
#include "PCF8591.H"

sbit RELAY = P0^4;
sbit BUZZ = P0^6;
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit L1 = P0^0;

code unsigned char duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};

unsigned char Dis_Mod = 0;
unsigned int Price = 50;	//每升价格（分）
unsigned int Water_All = 0;	//10ml
unsigned int Price_All = 0;	//分
unsigned char Count_10ms = 0;

bit Relay_Flag = 0;	//出水状态，1出水，0停止出水
bit LED1_Flag = 1;


void Init_T0(void)
{
	TMOD = 0x01;
	AUXR = 0x00;
	TH0 = (65536 - 10000) / 256;
	TL0 = (65536 - 10000) % 256;
	TR0 = 0;
	EA = 1;
	ET0 = 1;
}


void Display(void)
{
	unsigned char i;
	if(Dis_Mod == 0)
	{
		for(i = 0; i < 8; i++)
		{
			InputP0(6,0x01<<i);
			switch(i)
			{
				case 1:InputP0(7,duan[Price/100]&0x7F);break;
				case 2:InputP0(7,duan[Price%100/10]);break;
				case 3:InputP0(7,duan[Price%10]);break;
				case 4:InputP0(7,duan[Water_All/1000]);break;
				case 5:InputP0(7,duan[Water_All%1000/100]&0x7F);break;
				case 6:InputP0(7,duan[Water_All%100/10]);break;
				case 7:InputP0(7,duan[Water_All%10]);break;
			}
			Delay(8);
			InputP0(7,0xFF);
		}
	}
	if(Dis_Mod == 1)
	{
		for(i = 0; i < 8; i++)
		{
			InputP0(6,0x01<<i);
			switch(i)
			{
				case 1:InputP0(7,duan[Price/100]&0x7F);break;
				case 2:InputP0(7,duan[Price%100/10]);break;
				case 3:InputP0(7,duan[Price%10]);break;
				case 4:InputP0(7,duan[Price_All/1000]);break;
				case 5:InputP0(7,duan[Price_All%1000/100]&0x7F);break;
				case 6:InputP0(7,duan[Price_All%100/10]);break;
				case 7:InputP0(7,duan[Price_All%10]);break;
			}
			Delay(8);
			InputP0(7,0xFF);
		}
	}
}

void Key_Scanf(void)
{
	if(Dis_Mod == 0 && S7 == 0 && TR0 == 0)
	{
		Delay(10);
		if(S7 == 0)
		{
			if(Water_All != 0)
				Water_All = 0;
			TR0 = 1;
			Relay_Flag = 1;
			InitHC138(5);
			RELAY = Relay_Flag;
			BUZZ = 0;
			while(S7 == 0)
				Display();
		}
	}
	if(Dis_Mod == 0 && S6 == 0 && TR0 == 1)
	{
		Delay(10);
		if(S6 == 0)
		{
			TR0 = 0;
			Relay_Flag = 0;
			InitHC138(5);
			RELAY = Relay_Flag;
			BUZZ = 0;
			Dis_Mod = 1;
			while(S6 == 0)
				Display();
		}
	}
	if(Dis_Mod == 1 && S7 == 0 && TR0 == 0)
	{
		Delay(10);
		if(S7 == 0)
		{
			Dis_Mod = 0;
			while(S7 == 0)
				Display();
		}
	}
}

void Work(void)
{
	if(AD_Rd1() >= 64)
		LED1_Flag = 1;
	else 
		LED1_Flag = 0;
	InitHC138(4);
	L1 = LED1_Flag;
}

void main(void)
{
	InputP0(5,0x00);
	InputP0(4,0xFF);
	Init_T0();
	while(1)
	{
		Display();
		Work();
		Key_Scanf();
	}
}

void T0_ser(void) interrupt 1
{
	TH0 = (65536 - 10000) / 256;
	TL0 = (65536 - 10000) % 256;
	++Count_10ms;
	if(Count_10ms == 10)
	{
		++Water_All;
		if(Water_All == 9999)
		{
			TR0 = 0;
			Relay_Flag = 0;
			InitHC138(5);
			RELAY = Relay_Flag;
			BUZZ = 0;
			Dis_Mod = 1;
		}
		Price_All = (unsigned int)((double)Water_All * Price / 100 + 0.5);
		Count_10ms = 0;
	}
}