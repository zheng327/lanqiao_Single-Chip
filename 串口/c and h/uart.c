#include <stc15.h>
#include "uart.h"

unsigned char uart_rx_buffer[8];

bit busy;
void UartInit(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8???,?????
	AUXR |= 0x40;		//???1???Fosc,?1T
	AUXR &= 0xFE;		//??1?????1???????
	TMOD &= 0x0F;		//?????1?16???????
	TL1 = 0xE0;		//??????
	TH1 = 0xFE;		//??????
	ET1 = 0;		//?????1??
	TR1 = 1;		//?????1
	ES=1;
	EA=1;
}
void Uart() interrupt 4 using 1
{
	static unsigned char  rx_count=0;
    if (RI)
    {
        RI = 0;   
				if(rx_count<8)
				{					
					uart_rx_buffer[rx_count] = SBUF;
					rx_count++;
				}
				else
					rx_count=0;
    }
    if (TI)
    {
        TI = 0;                 
        busy = 0;               
    }
}
void SendData(unsigned char dat)
{
    while (busy);               
    busy = 1;
    SBUF = dat;                 
}

void SendString(char *s)
{
    while (*s)                 
    {
        SendData(*s++);         
    }
}







