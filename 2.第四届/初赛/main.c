#include "STC15F2K60S2.H"
#include "iic.h"
#include "ds1302.h"
#include "24C02.H"
#include "PCF8591.H"
#include "my.h"

sbit L1 = P0^0;
sbit L2 = P0^1;
sbit S4 = P3^3;
sbit S5 = P3^2;
sbit S6 = P3^1;
sbit S7 = P3^0;
sbit RELAY = P0^4;
sbit BUZZ = P0^6;

code unsigned char duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};
unsigned char Dis_Mod = 0;	//显示模式，0代表时间湿度，1代表湿度阈值设定
unsigned char Work_Mod = 0; //工作模式，0代表自动，1代表手动
unsigned char Time[] = {22,7,1,23,8,30,0};
unsigned char Wet_Now = 0;
unsigned char Wet_Set = 50;
unsigned char Set_Temp = 50;
bit Relay_Flag = 0;
bit Buzz_Flag = 1;

void Display(void)
{
	unsigned char i;
	if(Dis_Mod == 0)
	{
		Ds1302_Read_Time(Time);
		Wet_Now = (unsigned char)((double)AD_Rb2_Vol()/255*100 - 0.5);
		for(i = 0; i < 8; i++)
		{
			InputP0(6,0x01<<i);
			switch(i)
			{
				case 0:InputP0(7,duan[Time[4]/10]);break;
				case 1:InputP0(7,duan[Time[4]%10]);break;
				case 2:InputP0(7,0xBF);break;
				case 3:InputP0(7,duan[Time[5]/10]);break;
				case 4:InputP0(7,duan[Time[5]%10]);break;
				case 6:InputP0(7,duan[Wet_Now/10]);break;
				case 7:InputP0(7,duan[Wet_Now%10]);break;
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
				case 0:InputP0(7,0xBF);break;
				case 1:InputP0(7,0xBF);break;
				case 6:InputP0(7,duan[Set_Temp/10]);break;
				case 7:InputP0(7,duan[Set_Temp%10]);break;
			}
			Delay(8);
			InputP0(7,0xFF);
		}
	}
}

void Wet_Work()
{
	Wet_Now = (unsigned char)((double)AD_Rb2_Vol()/255*100 - 0.5);
	if(Work_Mod == 0)
	{
		InitHC138(4);
		L1 = 0;
		L2 = 1;
		if(Wet_Now < Wet_Set)
		{
			InitHC138(5);
			BUZZ = 0;
			RELAY = 1;
		}
		if(Wet_Now >= Wet_Set)
		{
			InitHC138(5);
			BUZZ = 0;
			RELAY = 0;
		}
	}
	if(Work_Mod == 1)
	{
		InitHC138(4);
		L1 = 1;
		L2 = 0;
		if(Wet_Now < Wet_Set && Buzz_Flag == 1)
		{
			InitHC138(5);
			BUZZ = 1;
		}
		if(Buzz_Flag == 0 || Wet_Now >= Wet_Set)
		{
			InitHC138(5);
			BUZZ = 0;
		}
		if(Relay_Flag == 1)
		{
			InitHC138(5);
			RELAY = 1;
		}
		else
		{
			InitHC138(5);
			RELAY = 0;
		}
	}
}

void Key_Scanf(void)
{
	if(S7 == 0)
	{
		Delay(10);
		if(S7 == 0)
		{
			Work_Mod = (Work_Mod + 1) % 2;
			while(S7 == 0)
			{
				Display();
				Wet_Work();
			}
		}
	}
	
	if(Work_Mod == 0)
	{
		if(Dis_Mod == 0 && S6 == 0)
		{
			Delay(10);
			if(S6 == 0)
			{
				Dis_Mod = 1;
				while(S6 == 0)
				{
					Display();
					Wet_Work();
				}
			}
		}
		if(Dis_Mod == 1 && S6 == 0)
		{
			Delay(10);
			if(S6 == 0)
			{
				Wet_Set = Set_Temp;
				Write_Byte_24C02(0x00,Set_Temp);
				Dis_Mod = 0;
				while(S6 == 0)
				{
					Display();
					Wet_Work();
				}
			}
		}
		if(Dis_Mod == 1 && S5 == 0)
		{
			Delay(10);
			if(S5 == 0)
			{
				if(Set_Temp <= 98)
					Set_Temp = Set_Temp + 1;
				while(S5 == 0)
				{
					Display();
					Wet_Work();
				}
			}
		}
		if(Dis_Mod == 1 && S4 == 0)
		{
			Delay(10);
			if(S4 == 0)
			{
				if(Set_Temp >= 1)
					Set_Temp = Set_Temp - 1;
				while(S4 == 0)
				{
					Display();
					Wet_Work();
				}
			}
		}
	}
	if(Work_Mod == 1)
	{
		if(S6 == 0)
		{
			Delay(10);
			if(S6 == 0)
			{
				Buzz_Flag = ~Buzz_Flag;
				while(S6 == 0)
				{
					Display();
					Wet_Work();
				}
			}
		}
		if(S5 == 0)
		{
			Delay(10);
			if(S5 == 0)
			{
				Relay_Flag = 1;
				while(S5 == 0)
				{
					Display();
					Wet_Work();
				}
			}
		}
		if(S4 == 0)
		{
			Delay(10);
			if(S4 == 0)
			{
				Relay_Flag = 0;
				while(S4 == 0)
				{
					Display();
					Wet_Work();
				}
			}
		}
	}
}

void main(void)
{
	InputP0(5,0x00);
	InputP0(4,0xFF);
	Ds1302_Write_Time(Time);
	Wet_Set = Read_Byte_24C02(0x00);
	Set_Temp = Wet_Set;
	while(1)
	{
		Display();
		Wet_Work();
		Key_Scanf();
	}
}