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

void UartInit() // ������ 9600
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xA0;		//���ö�ʱ��ʼֵ
	T2H = 0xFC;		//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	P_SW1 = 0x40; 	//�л��������ţ�RXD_2/P3.6, TXD_2/P3.7
	wptr = 0x00;
	rptr = 0x00;
	busy = 0;
}
//void UartInit() //������ 115200
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



