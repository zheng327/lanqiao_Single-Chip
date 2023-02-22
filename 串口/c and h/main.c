#include <stc15.h>
#include "uart.h"

typedef unsigned char u8;
typedef unsigned int u16;


u8 code  smg_duan[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
u8 xdata smg_wei[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	

bit tx_flag;

bit key_flag;
u8 key_value;

u8 xdata rx_table[8];

void Timer2Init(void)		//1ms
{
	AUXR |= 0x04;		
	T2L = 0xCD;		
	T2H = 0xD4;		
	AUXR |= 0x10;		
	IE2|=1<<2;
}

void uart_rx_driver()
{
		
}
void display_driver()
{
	rx_table[0]=smg_duan[uart_rx_buffer[0]-'0'];
	rx_table[1]=smg_duan[uart_rx_buffer[1]-'0'];
	rx_table[2]=smg_duan[uart_rx_buffer[2]-'0'];
	rx_table[3]=smg_duan[uart_rx_buffer[3]-'0'];
	rx_table[4]=smg_duan[uart_rx_buffer[4]-'0'];
	rx_table[5]=smg_duan[uart_rx_buffer[5]-'0'];
	rx_table[6]=smg_duan[uart_rx_buffer[6]-'0'];
	rx_table[7]=smg_duan[uart_rx_buffer[7]-'0'];
}
void main()
{
	u8 i;
	UartInit();
	Timer2Init();
	
	while(1)
	{
		display_driver();
		if(tx_flag)
		{
			tx_flag=0;
			for(i=0;i<8;i++)
			{
				SendData(uart_rx_buffer[i]);
			}
		}
	}
}
void Timer2Routine()interrupt 12
{
	static u16 i=0,smg_count,tx_count;
	smg_count++;tx_count++;
	if(smg_count==3)
	{
		smg_count=0;
		P2=0xe0;P0=0;P2=0;
		P2=0xe0;P0=~rx_table[i];P2=0;
		P2=0xc0;P0=smg_wei[i];P2=0;
		i++;
		if(i==8)i=0;
	}
	if(tx_count==1000)
	{
		tx_count=0;
		tx_flag=1;
	}
}










