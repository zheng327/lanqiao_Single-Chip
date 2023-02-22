#ifndef uart
#define uart


extern unsigned char uart_rx_buffer[8];

void UartInit(void)	;


void SendData(unsigned char dat);

void SendString(char *s);


#endif

