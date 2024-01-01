#include "stc8g.h"
#include "delay.h"
#include "ch9329.h"
#include "uart.h"

// ��Ҫ���������򿪹غ͵����ͨ��״̬��ͨ�����ڽ�����ƶ��͵������Ӧָ��͸�ch9329оƬ���ɡ�
// ���򿪹�ʹ����������������Ӳ��������·����Ϊ�˼��ٺ��ӣ������������ʵ��
// ������ɨ��ο� ��ɳ̲�����ҵ�����ʦ���ķ����ڶ�ʱ����������ȡ���ص�״̬��������ȡ��8����ͬ��״̬��Ϊ��û�ж�����״̬
// ch9329���ִ���ο���оƬʹ���ֲ��������Դ�㳡 @yuanzihao �ġ����ӽ̱ޡ���Ŀ���룬��л���û��Ŀ�Դ��
// ���������õ�����Դ�������������籨��Ϊ700Hz
// ���濪ʼ��д����

// ������Դ��ʹ�ã����ڵĲ����ʷ�����Ӧ����Ҫռ��һ����ʱ��������ɨ��ռ��һ����ʱ����
// ��ʱ��2����������1�����ʷ������������������жϣ���ʱ��1��Ϊ����������ʹ�ã�
// Nice����


sbit buzz = P1^0;
sbit key_f1 = P3^0;
sbit key_f2 = P3^1;
sbit key_f3 = P3^3;
sbit key_f4 = P3^4;
sbit key_f5 = P3^5;
sbit key_k1 = P3^2;
sbit key_k2 = P5^4;
bit sta_f1 = 1;
bit sta_f2 = 1;
bit sta_f3 = 1;
bit sta_f4 = 1;
bit sta_f5 = 1;
bit sta_k1 = 1;
bit sta_k2 = 1;
unsigned char longPressBegin = 0;//1 ��ʾ��ʼ��ⳤ�� 2��ʾȷʵ�ڳ���״̬
unsigned int cnt = 0;

void TM1Init()  //1ms
{
	AUXR |= 0x40;	
	TMOD &= 0x0F;	
	TL1 = 0x66;		
	TH1 = 0x7E;	
	TF1 = 0;		
	TR1 = 1;		
	ET1 = 1;       
}

void Timer0Init() //΢��@33.1776MHz 
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
//	TL0 = 0x56;		//���ö�ʱ��ʼֵ  700Hz *2 Hz
//	TH0 = 0xA3;		//���ö�ʱ��ʼֵ
	TL0 = 0x26;		//���ö�ʱ��ʼֵ 22us 700*64Hz
	TH0 = 0xFD;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//1:��ʱ��0��ʼ��ʱ 
//	ET0 = 1;   
}

void main()
{
	bit bak_f1=1,bak_f2=1,bak_f3=1,bak_f4=1,bak_f5=1,bak_k1=1,bak_k2=1;
	P1M0 = 0x01;
	P1M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	
	UartInit();
	TM1Init();
	Timer0Init();
	ES = 1;
	EA = 1;
	
	key_k2 = 0;

	
	CCON = 0x00;			//
	CMOD = 0x04;      //��ʱ��0���������
	CL = 0x00;
	CH = 0x00;
	CCAPM1 = 0x42;         //PCAģ��1ΪPWM����ģʽ,���ж�
	PCA_PWM1 = 0x80;       //PCAģ��1���6λPWM  6λ Ƶ�ʳ���64
	CCAP1H = 0x20;
	CCAP1L = 0x20;         //PWMռ��50%
	CR = 0;     //0��ֹͣ  1������PCA��ʱ��
	
	while(1)
	{
		if(cnt>=40 && longPressBegin == 2)
		{
			if(!sta_f3)
			{
				msMoveUp();
				cnt = 0;
			}
			else if(!sta_f2)
			{
				msMoveLeft();
				cnt = 0;
			}
			else if(!sta_f4)
			{
				msMoveDown();
				cnt = 0;
			}
			else if(!sta_f5)
			{
				msMoveRight();
				cnt = 0;
			}
		}
//		UartSendStr("Uart test send str");
		if(sta_f1 != bak_f1) //˵�������ж�������������򿪹ذ��µĶ���������ģ�������������
		{
			if(!bak_f1) //�������º����ɿ��˱�ʾ�Ѿ����һ�ζ���
			{
				msClickLeftUp();
				CR = 0;     //0��ֹͣ  1������PCA��ʱ��
//				TR0 = 0;		//1:��ʱ��0��ʼ��ʱ 0:ֹͣ
				buzz =0;
//				UartSendStr("msClickLeftUp\r\n");
			}
			else
			{
				msClickLeftDown();
				CR = 1;     //0��ֹͣ  1������PCA��ʱ��
//				TR0 = 1;		//1:��ʱ��0��ʼ��ʱ 0:ֹͣ
//				UartSendStr("msClickLeftDown\r\n");	
			}
			bak_f1 = sta_f1;
		}
		
		if(sta_k1 != bak_k1) //����ǵ������
		{
			if(!bak_k1) //�������º����ɿ��˱�ʾ�Ѿ����һ�ζ���
			{
				msClickLeftUp();
				CR = 0;     //0��ֹͣ  1������PCA��ʱ��
			}
			else
			{
				msClickLeftDown();
				CR = 1;     //0��ֹͣ  1������PCA��ʱ��
			}
			bak_k1 = sta_k1;
		}

		if(sta_f3 != bak_f3) //��������򿪹����ҵĶ���������ģ�����ָ������
		{
			if(!bak_f3) //�������º����ɿ��˱�ʾ�Ѿ����һ�ζ���
			{
				msMoveUp();			
				longPressBegin = 0;		
				cnt = 0;
			}
			else
				longPressBegin = 1;
			bak_f3 = sta_f3;
		}
		
		if(sta_f4 != bak_f4) //��������򿪹����ҵĶ���������ģ�����ָ������
		{
			if(!bak_f4) //�������º����ɿ��˱�ʾ�Ѿ����һ�ζ���
			{
				msMoveDown();
				longPressBegin = 0;
			}
			else
				longPressBegin = 1;
			bak_f4 = sta_f4;
		}
		
		if(sta_f5 != bak_f5) //��������򿪹����ҵĶ���������ģ�����ָ������
		{
			if(!bak_f5) //�������º����ɿ��˱�ʾ�Ѿ����һ�ζ���
			{				
				msMoveRight();
				longPressBegin = 0;
			}
			else
				longPressBegin = 1;
			bak_f5 = sta_f5;
		}
		
		if(sta_f2 != bak_f2) //˵�������ж�������������򿪹����ҵĶ���������ģ�����ָ������
		{
			if(!bak_f2) //�������º����ɿ��˱�ʾ�Ѿ����һ�ζ���
			{
				msMoveLeft();
				longPressBegin = 0;
			}
			else
				longPressBegin = 1;
			bak_f2 = sta_f2;
		}
	}	

}

void tm0Isr() interrupt 1
{
		buzz = ~buzz;
}

void TM1_Isr() interrupt 3 //1ms
{
	static unsigned int f1buf=0xff,f2buf=0xff,f3buf=0xff,f4buf=0xff,f5buf=0xff,k1buf=0xff,k2buf=0xff;

	if(longPressBegin) //��ʱ�����ڼ�ⳤ��
	{
		cnt ++;
	}
	if(cnt>600 && longPressBegin==1) //ȷʵ���ڳ���
		longPressBegin = 2;

	
//	buzz = ~buzz;
	f1buf = (f1buf<<1) | key_f1;
	if(f1buf == 0x0000)
		sta_f1 = 0;
	else if (f1buf == 0xffff)
		sta_f1 = 1;
	
	f2buf = (f2buf<<1) | key_f2;
	if(f2buf == 0x0000)
		sta_f2 = 0;
	else if (f2buf == 0xffff)
		sta_f2 = 1;
	
	f3buf = (f3buf<<1) | key_f3;
	if(f3buf == 0x0000)
		sta_f3 = 0;
	else if (f3buf == 0xffff)
		sta_f3 = 1;
	
	f4buf = (f4buf<<1) | key_f4;
	if(f4buf == 0x0000)
		sta_f4 = 0;
	else if (f4buf == 0xffff)
		sta_f4 = 1;
	
	f5buf = (f5buf<<1) | key_f5;
	if(f5buf == 0x0000)
		sta_f5 = 0;
	else if (f5buf == 0xffff)
		sta_f5 = 1;
	
	k1buf = (k1buf<<1) | key_k1;
	if(k1buf == 0x0000)
		sta_k1 = 0;
	else if (k1buf == 0xffff)
		sta_k1 = 1;
	
//	k2buf = (k2buf<<1) | key_k2;
//	if(k2buf == 0x0000)
//		sta_k2 = 0;
//	else if (k2buf == 0xffff)
//		sta_k2 = 1;	
}





