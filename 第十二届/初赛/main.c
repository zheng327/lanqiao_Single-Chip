#include "STC15F2K60S2.H"
#include "onewire.h"
#include "iic.h"
#include "my.h"

sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
sbit L4 = P0^3;

code unsigned char duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};
unsigned char Dis_Mod = 0;
unsigned char DAC_Mod = 1;
unsigned int Ds18b20_Temp = 0;
unsigned int Set_Temp = 25;
unsigned int Temp_Set = 25;
unsigned int A_Out = 500;

void Display(void)
{
	unsigned char i;
	if(Dis_Mod == 0)
	{
		//Ds18b20_Temp = rd_temperature();
		for(i = 0; i < 8; i++)
		{
			InputP0(6,0x01<<i);
			switch(i)
			{
				case 0:InputP0(7,duan[0x0C]);break;
				
				case 1:InputP0(7,0xFF);break;
				case 2:InputP0(7,0xFF);break;
				case 3:InputP0(7,0xFF);break;
				
				case 4:InputP0(7,duan[Ds18b20_Temp/1000]);break;
				case 5:InputP0(7,duan[Ds18b20_Temp%1000/100]&0x7F);break;
				case 6:InputP0(7,duan[Ds18b20_Temp%100/10]);break;
				case 7:InputP0(7,duan[Ds18b20_Temp%10]);break;
			}
			Delay(10);
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
				case 0:InputP0(7,0x8C);break;
				
				case 1:InputP0(7,0xFF);break;
				case 2:InputP0(7,0xFF);break;
				case 3:InputP0(7,0xFF);break;
				
				case 4:InputP0(7,0xFF);break;
				case 5:InputP0(7,0xFF);break;
				case 6:InputP0(7,duan[Set_Temp/10]);break;
				case 7:InputP0(7,duan[Set_Temp%10]);break;
			}
			Delay(10);
			InputP0(7,0xFF);
		}
	}
	if(Dis_Mod == 2)
	{
		for(i = 0; i < 8; i++)
		{
			InputP0(6,0x01<<i);
			switch(i)
			{
				case 0:InputP0(7,duan[0x0A]);break;
				
				case 1:InputP0(7,0xFF);break;
				case 2:InputP0(7,0xFF);break;
				case 3:InputP0(7,0xFF);break;
				
				case 4:InputP0(7,0xFF);break;
				case 5:InputP0(7,duan[A_Out/100]&0x7F);break;
				case 6:InputP0(7,duan[A_Out%100/10]);break;
				case 7:InputP0(7,duan[A_Out%10]);break;
			}
			Delay(10);
			InputP0(7,0xFF);
		}
	}
}

unsigned char Key_Scanf(void)
{
	unsigned char row = 0,col = 0;
	P44 = 1; P42 = 1; P35 = 1; P34 = 1; P33 = 0; P32 = 0; P31 = 0; P30 = 0;
	Delay(10);
	if(P44 == 0)
		row = 1;
	else if(P42 == 0)
		row = 2;
	else if(P35 == 0)
		row = 3;
	else if(P34 == 0)
		row = 4;
	else
		row = 0;
	
	P44 = 0; P42 = 0; P35 = 0; P34 = 0; P33 = 1; P32 = 1; P31 = 1; P30 = 1;
	Delay(10);
	if(P33 == 0)
		col = 1;
	else if(P32 == 0)
		col = 2;
	else if(P31 == 0)
		col = 3;
	else if(P30 == 0)
		col = 4;
	else
		col = 0;
	
	if(col != 0 && row != 0)
		return row * 4 + col - 1;
	else
		return 0;
}

void Key_Work(void)
{
	if(Key_Scanf() == 4)
	{
		Delay(10);
		if(Key_Scanf() == 4)
		{
			while(Key_Scanf() == 4)
				Display();
			Dis_Mod = (Dis_Mod + 1) % 3;
			if(Dis_Mod == 1)
				Temp_Set = Set_Temp;
		}
	}
	if(Key_Scanf() == 8 && Dis_Mod == 1)
	{
		Delay(10);
		if(Key_Scanf() == 8)
		{
			while(Key_Scanf() == 8)
				Display();
			if(Set_Temp >= 1)
				Set_Temp = Set_Temp - 1;
		}
	}
	if(Key_Scanf() == 9 && Dis_Mod == 1)
	{
		Delay(10);
		if(Key_Scanf() == 9)
		{
			while(Key_Scanf() == 9)
				Display();
			if(Set_Temp <= 98)
				Set_Temp = Set_Temp + 1;
		}
	}
	if(Key_Scanf() == 5)
	{
		Delay(10);
		if(Key_Scanf() == 5)
		{
			while(Key_Scanf() == 5)
				Display();
			DAC_Mod = DAC_Mod + 1;
			if(DAC_Mod == 3)
				DAC_Mod = 1;
		}
	}
}

void Work(void)
{
	Ds18b20_Temp = rd_temperature();
	if(DAC_Mod == 1)
	{
		if(Ds18b20_Temp < Temp_Set * 100)
			A_Out = 0;
		else
			A_Out = 500;
	}
	if(DAC_Mod == 2)
	{
		if(Ds18b20_Temp < 20 * 100)
			A_Out = 100;
		else if(20 * 100 <= Ds18b20_Temp && Ds18b20_Temp <= 40 * 100)
			A_Out = 0.15 * Ds18b20_Temp - 200;
		else
			A_Out = 400;
	}
	PCF8591_DAC(A_Out);
}

void LED(void)
{
	InitHC138(4);
	if(DAC_Mod == 1)
		L1 = 0;
	else
		L1 = 1;
	if(Dis_Mod == 0)
		L2 = 0;
	else
		L2 = 1;
	if(Dis_Mod == 1)
		L3 = 0;
	else
		L3 = 1;
	if(Dis_Mod == 2)
		L4 = 0;
	else
		L4 = 1;
}

void main(void)
{
	InputP0(5,0x00);
	InputP0(4,0xFF);
	while(1)
	{
		LED();
		Work();
		Key_Work();
		Display();
	}
}