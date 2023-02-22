#include "STC15F2K60S2.H"
#include "ds1302.h"
#include "onewire.h"
#include "my.h"

sbit L1 = P0^0;
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

code unsigned char duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};
unsigned char Time[] = {22,5,1,21,23,59,50};
unsigned char Temp[] = {0,0,0,0,0,0,0,0,0,0};
unsigned char Temp_Index = 0;
unsigned char Dis_Mod = 0;
unsigned char Time_Set[] = {1,5,30,60};
unsigned char Set_Index = 0;
unsigned char Set_1s = 1;
unsigned char Count_50ms = 0;
unsigned char Count_1s = 0;

bit Dis_Flag = 0;
bit LED_Flag = 0;
bit Comp_Flag = 0;

void Init_Timer(void)
{
	TMOD = 0x11;
	AUXR = 0x00;
	TH0 = (65536 - 50000) / 256;
	TL0 = (65536 - 50000) % 256;
	TH1 = 0;
	TL1 = 0;
	TR0 = 0;
	TR1 = 0;
	EA = 1;
	ET0 = 1;
	ET1 = 1;
}

void Display(void)
{
	unsigned char i;
	if(Dis_Mod == 0)
	{
		InputP0(6,0x20);
		InputP0(7,0xBF);
		Delay(4);
		InputP0(7,0xFF);
		
		InputP0(6,0x40);
		InputP0(7,duan[Time_Set[Set_Index]/10]);
		Delay(4);
		InputP0(7,0xFF);
		
		InputP0(6,0x80);
		InputP0(7,duan[Time_Set[Set_Index]%10]);
		Delay(4);
		InputP0(7,0xFF);
	}
	else if(Dis_Mod == 1)
	{
		Read_Time_Ds1302(Time);
		for(i = 0; i < 8; i++)
		{
			InputP0(6,0x01<<i);
			switch(i)
			{
				case 0:InputP0(7,duan[Time[4]/10]);break;
				case 1:InputP0(7,duan[Time[4]%10]);break;
				case 2:if(Dis_Flag) InputP0(7,0xBF);break;
				case 3:InputP0(7,duan[Time[5]/10]);break;
				case 4:InputP0(7,duan[Time[5]%10]);break;
				case 5:if(Dis_Flag) InputP0(7,0xBF);break;
				case 6:InputP0(7,duan[Time[6]/10]);break;
				case 7:InputP0(7,duan[Time[6]%10]);break;
			}
			Delay(4);
			InputP0(7,0xFF);
		}
	}
	else if(Dis_Mod == 2)
	{
		for(i = 0; i < 8; i++)
		{
			InputP0(6,0x01<<i);
			switch(i)
			{
				case 0:InputP0(7,0xBF);break;
				case 1:InputP0(7,duan[Temp_Index/10]);break;
				case 2:InputP0(7,duan[Temp_Index%10]);break;
				case 3:InputP0(7,0xFF);break;
				case 4:InputP0(7,0xFF);break;
				case 5:InputP0(7,0xBF);break;
				case 6:InputP0(7,duan[Temp[Temp_Index]/10]);break;
				case 7:InputP0(7,duan[Temp[Temp_Index]%10]);break;
			}
			Delay(4);
			InputP0(7,0xFF);
		}
	}
}

void Key_Scanf(void)
{
	if(S4 == 0 && Dis_Mod == 0)
	{
		Delay(100);
		if(S4 == 0)
		{
			Set_Index = (Set_Index + 1) % 4;
			while(S4 == 0)
				;
		}
	}
	if(S5 == 0 && Dis_Mod == 0)
	{
		Delay(100);
		if(S5 == 0)
		{
			Set_1s = Time_Set[Set_Index];
			Set_Index = 0;
			Dis_Mod = 1;
			TR0 = 1;
			while(S5 == 0)
				;
		}
	}
	if(S6 == 0 && Dis_Mod == 2 && Comp_Flag)
	{
		Delay(100);
		if(S6 == 0)
		{
			InputP0(4,0xFF);
			Comp_Flag = 0;
			while(S6 == 0)
				;
		}
	}
	if(S6 == 0 && Dis_Mod == 2 && !Comp_Flag)
	{
		Delay(100);
		if(S6 == 0)
		{
			Temp_Index = (Temp_Index + 1) % 10;
			while(S6 == 0)
				;
		}
	}
	if(S7 == 0 && Dis_Mod == 2 && !Comp_Flag)
	{
		Delay(100);
		if(S7 == 0)
		{
			Temp_Index = 0;
			Dis_Mod = 0;
			while(S7 == 0)
				;
		}
	}
}

void LED_Work(void)
{
	if(Comp_Flag)
	{
		InitHC138(4);
		L1 = LED_Flag;
	}
	else
	{
		InitHC138(4);
		L1 = 1;
	}
}

void main(void)
{
	InputP0(5,0x00);
	InputP0(4,0xFF);
	Init_Timer();
	Write_Time_Ds1302(Time);
	while(1)
	{
		Display();
		Key_Scanf();
		LED_Work();
	}
}
	
	

void T0_ser(void) interrupt 1
{
	TH0 = (65536 - 50000) / 256;
	TL0 = (65536 - 50000) % 256;
	Count_50ms = Count_50ms + 1;
	if(Count_50ms == 20)
	{
		Dis_Flag = ~Dis_Flag;
		LED_Flag = ~LED_Flag;
		if(!Comp_Flag && Dis_Mod == 1)
			Count_1s = Count_1s + 1;
		if(Count_1s == Set_1s && !Comp_Flag)
		{
			Temp[Temp_Index] = rd_temperature();
			Temp_Index = Temp_Index + 1;
			if(Temp_Index == 10)
			{
				Comp_Flag = 1;
				Dis_Mod = 2;
				Temp_Index = 0;
			}
			Count_1s = 0;
		}
		Count_50ms = 0;
	}
}