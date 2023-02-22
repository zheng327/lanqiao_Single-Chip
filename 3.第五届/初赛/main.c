#include "STC15F2K60S2.H"
#include "onewire.h"
#include "my.h"

sbit L1 = P0^0;
sbit L2 = P0^1;

code unsigned char duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};

unsigned char Dis_Mod = 0;
unsigned char Temp_now = 0;
unsigned char Temp_lev = 0;
unsigned char T_max = 30,T_min = 20;
unsigned char Max_shi = 10,Max_ge = 10,Min_shi = 10,Min_ge = 10;
unsigned char Set_num = 0;
unsigned char Dtime_10ms = 0;
unsigned char Count_10ms = 0;
bit L1_Flag = 1;
bit L2_Flag = 1;

void Init_T0(void)
{
	TMOD = 0x01;
	AUXR = 0x00;
	TH0 = (65536 - 10000) / 256;
	TL0 = (65536 - 10000) % 256;
	TR0 = 1;
	EA = 1;
	ET0 = 1;
}


void Display(void)
{
	Temp_now = rd_temperature();
	if(Temp_now < T_min)
	{
		Temp_lev = 0;
		Dtime_10ms = 80;
		InputP0(5,0x00);
	}
	else if(T_min <= Temp_now && Temp_now <= T_max)
	{
		Temp_lev = 1;
		Dtime_10ms = 40;
		InputP0(5,0x00);
	}
	else
	{
		Temp_lev = 2;
		Dtime_10ms = 20;
		InputP0(5,0x10);
	}
	if(Dis_Mod == 0)
	{
		InputP0(6,0x01);
		InputP0(7,0xBF);
		Delay(4);
		InputP0(7,0xFF);
		
		InputP0(6,0x02);
		InputP0(7,duan[Temp_lev]);
		Delay(4);
		InputP0(7,0xFF);
		
		InputP0(6,0x04);
		InputP0(7,0xBF);
		Delay(4);
		InputP0(7,0xFF);
		
		InputP0(6,0x40);
		InputP0(7,duan[Temp_now/10]);
		Delay(4);
		InputP0(7,0xFF);
		
		InputP0(6,0x80);
		InputP0(7,duan[Temp_now%10]);
		Delay(4);
		InputP0(7,0xFF);
	}
	else if(Dis_Mod == 1)
	{
		InputP0(6,0x01);
		InputP0(7,0xBF);
		Delay(4);
		InputP0(7,0xFF);
		
		if(Set_num >= 1)
		{
			InputP0(6,0x02);
			InputP0(7,duan[Max_shi]);
			Delay(4);
			InputP0(7,0xFF);
		}
		
		if(Set_num >= 2)
		{
			InputP0(6,0x04);
			InputP0(7,duan[Max_ge]);
			Delay(4);
			InputP0(7,0xFF);
		}
		
		InputP0(6,0x20);
		InputP0(7,0xBF);
		Delay(4);
		InputP0(7,0xFF);
		
		if(Set_num >= 3)
		{
			InputP0(6,0x40);
			InputP0(7,duan[Min_shi]);
			Delay(4);
			InputP0(7,0xFF);
		}
		
		
		if(Set_num >= 4)
		{
			InputP0(6,0x80);
			InputP0(7,duan[Min_ge]);
			Delay(4);
			InputP0(7,0xFF);
		}
	}
}

unsigned char Key_Scanf(void)
{
	unsigned char row,col,key_value;
	P30 = 0; P31 = 0; P32 = 0; P33 = 0; P34 = 1; P35 = 1; P42 = 1; P44 = 1;
	if(!P34)
		row = 4;
	else if(!P35)
		row = 3;
	else if(!P42)
		row = 2;
	else if(!P44)
		row = 1;
	else 
		row = 0;
	
	P30 = 1; P31 = 1; P32 = 1; P33 = 1; P34 = 0; P35 = 0; P42 = 0; P44 = 0;
	if(!P30)
		col = 4;
	else if(!P31)
		col = 3;
	else if(!P32)
		col = 2;
	else if(!P33)
		col = 1;
	else 
		col = 0;
	if(row && col)
		key_value = row * 4 + col - 1;
	else
		key_value = 0;
	
	P30 = 1; P31 = 1; P32 = 1; P33 = 1; P34 = 1; P35 = 1; P42 = 1; P44 = 1;
	
	return key_value;
}

void Key_Work(void)
{
	unsigned char x;
	TR0 = 0;
	x = Key_Scanf();
	//进入设置页面
	if(Dis_Mod == 0 && x == 8)
	{
		Delay(10);
		if(Dis_Mod == 0 && x == 8)
		{
			Dis_Mod = 1;
		}
		while(x == 8)
			x = Key_Scanf();
	}
	//清除
	else if(Dis_Mod == 1 && x == 12)
	{
		Delay(10);
		if(Dis_Mod == 1 && x == 12)
		{
			Max_shi = 0;
			Max_ge = 0;
			Min_shi = 0;
			Min_ge = 0;
			Set_num = 0;
		}
		while(x == 12)
			x = Key_Scanf();
	}
	//
	else if(Dis_Mod == 1 && Set_num == 0 && x)
	{
		Delay(10);
		if(Dis_Mod == 1 && Set_num == 0 && x)
		{
			switch(x)
			{
				case 4:Max_shi = 9;break;
				case 5:Max_shi = 6;break;
				case 6:Max_shi = 3;break;
				case 7:Max_shi = 0;break;
				case 9:Max_shi = 7;break;
				case 10:Max_shi = 4;break;
				case 11:Max_shi = 1;break;
				case 13:Max_shi = 8;break;
				case 14:Max_shi = 5;break;
				case 15:Max_shi = 2;break;
				default:Max_shi = 10;break;
			}
			if(Max_shi != 10)
				Set_num = 1;
			while(x)
				x = Key_Scanf();
		}
	}
	//
	else if(Dis_Mod == 1 && Set_num == 1 && x)
	{
		Delay(10);
		if(Dis_Mod == 1 && Set_num == 1 && x)
		{
			switch(x)
			{
				case 4:Max_ge = 9;break;
				case 5:Max_ge = 6;break;
				case 6:Max_ge = 3;break;
				case 7:Max_ge = 0;break;
				case 9:Max_ge = 7;break;
				case 10:Max_ge = 4;break;
				case 11:Max_ge = 1;break;
				case 13:Max_ge = 8;break;
				case 14:Max_ge = 5;break;
				case 15:Max_ge = 2;break;
				default:Max_ge = 10;break;
			}
			if(Max_ge != 10)
				Set_num = 2;
			while(x)
				x = Key_Scanf();
		}
	}
	//
	else if(Dis_Mod == 1 && Set_num == 2 && x)
	{
		Delay(10);
		if(Dis_Mod == 1 && Set_num == 2 && x)
		{
			switch(x)
			{
				case 4:Min_shi = 9;break;
				case 5:Min_shi = 6;break;
				case 6:Min_shi = 3;break;
				case 7:Min_shi = 0;break;
				case 9:Min_shi = 7;break;
				case 10:Min_shi = 4;break;
				case 11:Min_shi = 1;break;
				case 13:Min_shi = 8;break;
				case 14:Min_shi = 5;break;
				case 15:Min_shi = 2;break;
				default:Min_shi = 10;break;
			}
			if(Min_shi != 10)
				Set_num = 3;
			while(x)
				x = Key_Scanf();
		}
	}
	//
	else if(Dis_Mod == 1 && Set_num == 3 && x)
	{
		Delay(10);
		if(Dis_Mod == 1 && Set_num == 3 && x)
		{
			switch(x)
			{
				case 4:Min_ge = 9;break;
				case 5:Min_ge = 6;break;
				case 6:Min_ge = 3;break;
				case 7:Min_ge = 0;break;
				case 9:Min_ge = 7;break;
				case 10:Min_ge = 4;break;
				case 11:Min_ge = 1;break;
				case 13:Min_ge = 8;break;
				case 14:Min_ge = 5;break;
				case 15:Min_ge = 2;break;
				default:Min_ge = 10;break;
			}
			if(Min_ge != 10)
				Set_num = 4;
			while(x)
				x = Key_Scanf();
		}
	}
	//
	else if(Set_num == 4 && x == 8)
	{
		Delay(10);
		if(Set_num == 4 && x == 8)
		{
			if(Max_shi * 10 + Max_ge < Min_shi * 10 + Min_ge)
			{
				L2_Flag = 0;
			}
			else
			{
				T_max = Max_shi * 10 + Max_ge;
				T_min = Min_shi * 10 + Min_ge;
				Max_shi = 0;
				Max_ge = 0;
				Min_shi = 0;
				Min_ge = 0;
				Set_num = 0;
				L2_Flag = 1;
				Dis_Mod = 0;
			}
			while(x == 8)
				x = Key_Scanf();
		}
	}
	while(x)
	{
		x = Key_Scanf();
		Display();
	}
	TR0 = 1;
}

void LED_Work(void)
{
	TR0 = 0;
	InitHC138(4);
	L1 = L1_Flag;
	L2 = L2_Flag;
	TR1 = 1;
}

void main(void)
{
	InputP0(5,0x00);
	InputP0(4,0xFF);
	Init_T0();
	while(1)
	{
		LED_Work();
		Key_Work();
		Display();
	}
}

void T0_ser(void) interrupt 1
{
	TH0 = (65536 - 10000) / 256;
	TL0 = (65536 - 10000) % 256;
	++Count_10ms;
	if(Count_10ms == Dtime_10ms)
	{
		L1_Flag = ~L1_Flag;
		Count_10ms = 0;
	}
}