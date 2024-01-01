#include "stc8g.h"
#include "delay.h"
#include "ch9329.h"
#include "uart.h"

// 主要任务检测五向开关和电键的通断状态，通过串口将鼠标移动和点击的相应指令发送给ch9329芯片即可。
// 五向开关使用软件消抖，电键有硬件消抖电路，但为了减少焊接，先用软件消抖实现
// 按键的扫描参考 金沙滩工作室的宋老师讲的方法在定时器中连续读取开关的状态，连续读取到8次相同的状态认为是没有抖动的状态
// ch9329部分代码参考该芯片使用手册和立创开源广场 @yuanzihao 的“电子教鞭”项目代码，感谢该用户的开源。
// 蜂鸣器采用的是无源蜂鸣器，常见电报声为700Hz
// 下面开始编写程序

// 关于资源的使用，串口的波特率发生器应该需要占用一个定时器，按键扫描占用一个定时器，
// 定时器2用来做串口1波特率发生器，并开启串口中断，定时器1作为按键的消抖使用，
// Nice！！


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
unsigned char longPressBegin = 0;//1 表示开始检测长按 2表示确实在长按状态
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

void Timer0Init() //微秒@33.1776MHz 
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
//	TL0 = 0x56;		//设置定时初始值  700Hz *2 Hz
//	TH0 = 0xA3;		//设置定时初始值
	TL0 = 0x26;		//设置定时初始值 22us 700*64Hz
	TH0 = 0xFD;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//1:定时器0开始计时 
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
	CMOD = 0x04;      //定时器0的溢出脉冲
	CL = 0x00;
	CH = 0x00;
	CCAPM1 = 0x42;         //PCA模块1为PWM工作模式,无中断
	PCA_PWM1 = 0x80;       //PCA模块1输出6位PWM  6位 频率除以64
	CCAP1H = 0x20;
	CCAP1L = 0x20;         //PWM占空50%
	CR = 0;     //0：停止  1：启动PCA计时器
	
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
		if(sta_f1 != bak_f1) //说明按键有动作，这个是五向开关按下的动作，用来模拟鼠标左键点击。
		{
			if(!bak_f1) //按键按下后又松开了表示已经完成一次动作
			{
				msClickLeftUp();
				CR = 0;     //0：停止  1：启动PCA计时器
//				TR0 = 0;		//1:定时器0开始计时 0:停止
				buzz =0;
//				UartSendStr("msClickLeftUp\r\n");
			}
			else
			{
				msClickLeftDown();
				CR = 1;     //0：停止  1：启动PCA计时器
//				TR0 = 1;		//1:定时器0开始计时 0:停止
//				UartSendStr("msClickLeftDown\r\n");	
			}
			bak_f1 = sta_f1;
		}
		
		if(sta_k1 != bak_k1) //这个是电键动作
		{
			if(!bak_k1) //按键按下后又松开了表示已经完成一次动作
			{
				msClickLeftUp();
				CR = 0;     //0：停止  1：启动PCA计时器
			}
			else
			{
				msClickLeftDown();
				CR = 1;     //0：停止  1：启动PCA计时器
			}
			bak_k1 = sta_k1;
		}

		if(sta_f3 != bak_f3) //这个是五向开关向右的动作，用来模拟鼠标指针下移
		{
			if(!bak_f3) //按键按下后又松开了表示已经完成一次动作
			{
				msMoveUp();			
				longPressBegin = 0;		
				cnt = 0;
			}
			else
				longPressBegin = 1;
			bak_f3 = sta_f3;
		}
		
		if(sta_f4 != bak_f4) //这个是五向开关向右的动作，用来模拟鼠标指针上移
		{
			if(!bak_f4) //按键按下后又松开了表示已经完成一次动作
			{
				msMoveDown();
				longPressBegin = 0;
			}
			else
				longPressBegin = 1;
			bak_f4 = sta_f4;
		}
		
		if(sta_f5 != bak_f5) //这个是五向开关向右的动作，用来模拟鼠标指针左移
		{
			if(!bak_f5) //按键按下后又松开了表示已经完成一次动作
			{				
				msMoveRight();
				longPressBegin = 0;
			}
			else
				longPressBegin = 1;
			bak_f5 = sta_f5;
		}
		
		if(sta_f2 != bak_f2) //说明按键有动作，这个是五向开关向右的动作，用来模拟鼠标指针右移
		{
			if(!bak_f2) //按键按下后又松开了表示已经完成一次动作
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

	if(longPressBegin) //计时，用于检测长按
	{
		cnt ++;
	}
	if(cnt>600 && longPressBegin==1) //确实是在长按
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





