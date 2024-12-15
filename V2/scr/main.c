/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����        */
/*---------------------------------------------------------------------*/

/*************  ����˵��    **************

�����̻���STC8H8K64UΪ����оƬ��ʵ����9���б�д����.

����HIDЭ���USB��귶��

���������ص�������󼴿�ʵ��USB���Ļ�������
���󰴼��е�KEY0Ϊ��������KEY1Ϊ����м���KEY2Ϊ����Ҽ�
���󰴼��е�KEY4Ϊ���ƣ�KEY5Ϊ���ƣ�KEY6Ϊ���ƣ�KEY7Ϊ����

����ʱ, ����IRC����Ƶ��Ϊ 24MHz.

******************************************/

#include "stc.h"
#include "usb.h"
#include "usb_req_class.h"
#include "timer.h"

void sys_init();



void main()
{
	P_SW2 |= 0x80;  //��չ�Ĵ���(XFR)����ʹ��

    usb_init();
    timer_init();
    Timer1_Init();
    P1M0 = 0x01;
	P1M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
    P5M0 |= 0x10; P5M1 &= ~0x10; 

    EA = 1;
    
    while (1)
    {
        usb_class_in();
        keyHandle();
        if (f1ms)
        {
            f1ms = 0;
            scan_key();
            
        }
    }
}

