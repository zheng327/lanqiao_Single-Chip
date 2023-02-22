#include "STC15F2K60S2.H"
#include "onewire.h"

sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;
sbit FAN = P3^4;

code unsigned char duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};

unsigned int Temp = 0;	//温度数据
unsigned char Dis_Mod = 0;	//显示模式，0为倒计时，1为室温显示
unsigned char Dis_Wei = 0;	//位选
unsigned char Dis_Buffer[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char Fan_Mod = 1;	//风扇模式，1为睡眠风，2为自然风，3为常风
unsigned char Stime_s = 0;	//设置时间，单位s
unsigned char Rtime_s = 0;	//剩余时间，单位s
unsigned char Pwm_Duty = 0;	//PWM信号占空比
unsigned char Count_10us = 0;
unsigned char Count_50ms = 0;
bit Fan_Flag = 0;	//风扇输出标志位，0为低电平，1为高电平

void Delay10ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 117;
	j = 184;
	do
	{
		while (--j);
	} while (--i);
}

void Timer0Init(void)		//10微秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xF6;		//设置定时初始值
	TH0 = 0xFF;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0暂停计时
	EA = 1;
	ET0 = 1;
}

void Timer1Init(void)		//50毫秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xB0;		//设置定时初始值
	TH1 = 0x3C;		//设置定时初始值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;		//定时器1暂停计时
	ET1 = 1;
}

void Timer2Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x20;		//设置定时初始值
	T2H = 0xD1;		//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 |= 0x04;
	EA = 1;
}

void All_Init(void)
{
	P2 &= 0x1F;
	P0 = 0x00;
	P2 |= 0xA0;
	
	P2 &= 0x1F;
	P0 = 0xFF;
	P2 |= 0x80;
	
	Timer0Init();
	Timer1Init();
	Timer2Init();
}

unsigned int DS18B20_Work(void)
{
	double dat;
	unsigned int temp;
	unsigned char lsb,msb;
	
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0x44);
	
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0xBE);
	
	lsb = Read_DS18B20();
	msb = Read_DS18B20();
	
	temp = msb;
	temp = temp << 8;
	temp = temp | lsb;
	
	dat = temp * 0.0625 + 0.5;
	
	temp = (unsigned int)dat;
	return temp;
}

void Fan_Work(void)
{
	switch(Fan_Mod)
	{
		case 1:Pwm_Duty = 20;break;
		case 2:Pwm_Duty = 30;break;
		case 3:Pwm_Duty = 70;break;
	}
	if(Rtime_s != 0)
	{
		TR0 = 1;
		TR1 = 1;
		FAN = Fan_Flag;
	}
}

void LED_Work(void)
{
	P2 &= 0x1F;
	if(Rtime_s)
	{
		switch(Fan_Mod)
		{
			case 1:P0 = 0xFE;break;
			case 2:P0 = 0xFD;break;
			case 3:P0 = 0xFB;break;
		}
	}
	else
		P0 = 0xFF;
	P2 |= 0x80;
}

void Dis_Work(void)
{
	if(Dis_Mod == 0)
	{
		Dis_Buffer[0] = 0xBF;
		Dis_Buffer[1] = duan[Fan_Mod];
		Dis_Buffer[2] = 0xBF;
		Dis_Buffer[3] = 0xFF;
		Dis_Buffer[4] = duan[Rtime_s / 1000];
		Dis_Buffer[5] = duan[Rtime_s % 1000 / 100];
		Dis_Buffer[6] = duan[Rtime_s % 100 / 10];
		Dis_Buffer[7] = duan[Rtime_s % 10];
	}
	if(Dis_Mod == 1)
	{
		Dis_Buffer[0] = 0xBF;
		Dis_Buffer[1] = duan[4];
		Dis_Buffer[2] = 0xBF;
		Dis_Buffer[3] = 0xFF;
		Dis_Buffer[4] = 0xFF;
		Dis_Buffer[5] = duan[Temp / 10];
		Dis_Buffer[6] = duan[Temp % 10];
		Dis_Buffer[7] = duan[0x0C];
	}
}

void All_Work(void)
{
	Temp = DS18B20_Work();
	Fan_Work();
	LED_Work();
	Dis_Work();
	
}
void Key_Scanf(void)
{
	if(S4 == 0)
	{
		Delay10ms();
		if(S4 == 0)
		{
			Fan_Mod = Fan_Mod + 1;
			if(Fan_Mod == 4)
				Fan_Mod = 1;
			while(S4 == 0)
				All_Work();
		}
	}
	if(S5 == 0)
	{
		Delay10ms();
		if(S5 == 0)
		{
			Stime_s = Stime_s + 60;
			if(Stime_s == 180)
				Stime_s = 0;
			Rtime_s = Stime_s;
			while(S5 == 0)
				All_Work();
		}
	}
	if(S6 == 0)
	{
		Delay10ms();
		if(S6 == 0)
		{
			Rtime_s = 0;
			Stime_s = 0;
			while(S6 == 0)
				All_Work();
		}
	}
	if(S7 == 0)
	{
		Delay10ms();
		if(S7 == 0)
		{
			Dis_Mod = (Dis_Mod + 1) % 2;
			while(S7 == 0)
				All_Work();
		}
	}
}


void main(void)
{
	All_Init();
	Temp = DS18B20_Work();
	while(1)
	{
		All_Work();
		Key_Scanf();
	}
}


void T0_ser(void) interrupt 1
{
	Count_10us = Count_10us + 1;
	if(Count_10us >= 0 && Count_10us < Pwm_Duty) //PWM高电平
	{
		Fan_Flag = 1;
	}
	else if(Count_10us >= Pwm_Duty && Count_10us < 100) //PWM低电平
	{
		Fan_Flag = 0;
	}
	else if(Count_10us == 100) //一个周期为100个10us，即1ms
	{
		Count_10us = 0;
	}
}

void T1_ser(void) interrupt 3
{
	Count_50ms = Count_50ms + 1;
	if(Count_50ms == 20)
	{
		if(Rtime_s > 0)	//倒计时s
			Rtime_s = Rtime_s - 1;
		else if(Rtime_s == 0) //计时结束，停止定时器T0和T1
		{
			Stime_s = 0;
			TR0 = 0;
			TH0 = (65536 - 10) / 256;
			TL0 = (65536 - 10) % 256;
			Count_10us = 0;
			TR1 = 0;
			TH1 = (65536 - 50000) / 256;
			TL1 = (65536 - 50000) % 256;
			Count_50ms = 0;
		}
		Count_50ms = 0;
	}
}

void T2_ser(void) interrupt 12
{
	P2 &= 0x1F;
	P0 = 0xFF;
	P2 |= 0xE0;
	
	P2 &= 0x1F;
	P0 = 0x01 << Dis_Wei;
	P2 |= 0xC0;
	
	P2 &= 0x1F;
	P0 = Dis_Buffer[Dis_Wei];
	P2 |= 0xE0;
	
	if(++Dis_Wei >= 8)
		Dis_Wei = 0;
}