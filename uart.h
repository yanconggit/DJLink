#ifndef _UART_H_
#define _UART_H_

void UartInit();
void UartSend(char dat);
void UartSendStr(char *p);	
void UartSendCh9329Cmd(char p[]);

#endif
