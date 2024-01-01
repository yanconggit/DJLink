#include "stc8g.h"
#include "intrins.h"

bit busy;
char wptr;
char rptr;
char buffer[16];

void UartIsr() interrupt 4
{
    if (TI)
    {
        TI = 0;
        busy = 0;
    }
    if (RI)
    {
        RI = 0;
        buffer[wptr++] = SBUF;
        wptr &= 0x0f;
    }
}

void UartInit() // 波特率 9600
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xA0;		//设置定时初始值
	T2H = 0xFC;		//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	P_SW1 = 0x40; 	//切换串口引脚，RXD_2/P3.6, TXD_2/P3.7
	wptr = 0x00;
	rptr = 0x00;
	busy = 0;
}
//void UartInit() //波特率 115200
//{
//	SCON = 0x50;		
//	AUXR |= 0x01;		
//	AUXR |= 0x04;		
//	T2L = 0xB8;		
//	T2H = 0xFF;		
//	AUXR |= 0x10;	
////	P_SW1 = 0x40; 	
//	wptr = 0x00;
//	rptr = 0x00;
//	busy = 0;
//}

void UartSend(char dat)
{
    while (busy);
    busy = 1;
    SBUF = dat;
}

void UartSendStr(char *p)
{
    while (*p)
    {
        UartSend(*p++);
    }
}

void UartSendCh9329Cmd(char p[])
{
	int i = 0,j=0;
	i = sizeof(p);
	UartSend(i+'0');
	UartSendStr("\r\n");
	
	for(j=0;j<i;j++)
	{
		UartSend(p[j]);
	}
	
}



