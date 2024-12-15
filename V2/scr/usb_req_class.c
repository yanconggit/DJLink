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

#include "stc.h"
#include "usb.h"
#include "usb_req_class.h"

BYTE bHidIdle;
BYTE bKeyCode = 0;
BYTE bKeyDebounce = 0;
BOOL fKeyOK;

sbit buzz = P5^4;
sbit key_f1 = P3^2;
sbit key_f2 = P3^4;
sbit key_f3 = P3^5;
sbit key_f4 = P3^6;
sbit key_f5 = P3^7;
sbit key_k1 = P1^1;
sbit key_k2 = P1^0;
bit sta_f1 = 1;
bit sta_f2 = 1;
bit sta_f3 = 1;
bit sta_f4 = 1;
bit sta_f5 = 1;
bit sta_k1 = 1;
bit sta_k2 = 1;
unsigned char longPressBegin = 0;//1 ��ʾ��ʼ��ⳤ�� 2��ʾȷʵ�ڳ���״̬
unsigned int cnt = 0;

void usb_req_class()
{
    switch (Setup.bRequest)
    {
    case GET_REPORT:
        usb_get_report();
        break;
    case SET_REPORT:
        usb_set_report();
        break;
    case GET_IDLE:
        usb_get_idle();
        break;
    case SET_IDLE:
        usb_set_idle();
        break;
    case GET_PROTOCOL:
        usb_get_protocol();
        break;
    case SET_PROTOCOL:
        usb_set_protocol();
        break;
    default:
        usb_setup_stall();
        return;
    }
}

void usb_get_report()
{
    if ((DeviceState != DEVSTATE_CONFIGURED) ||
        (Setup.bmRequestType != (IN_DIRECT | CLASS_REQUEST | INTERFACE_RECIPIENT)))
    {
        usb_setup_stall();
        return;
    }

    Ep0State.pData = UsbBuffer;
    Ep0State.wSize = Setup.wLength;

    usb_setup_in();
}

void usb_set_report()
{
    if ((DeviceState != DEVSTATE_CONFIGURED) ||
        (Setup.bmRequestType != (OUT_DIRECT | CLASS_REQUEST | INTERFACE_RECIPIENT)))
    {
        usb_setup_stall();
        return;
    }

    Ep0State.pData = UsbBuffer;
    Ep0State.wSize = Setup.wLength;

    usb_setup_out();
}

void usb_get_idle()
{
    if ((DeviceState != DEVSTATE_CONFIGURED) ||
        (Setup.bmRequestType != (IN_DIRECT | CLASS_REQUEST | INTERFACE_RECIPIENT)))
    {
        usb_setup_stall();
        return;
    }

    Ep0State.pData = &bHidIdle;
    Ep0State.wSize = 1;

    usb_setup_in();
}

void usb_set_idle()
{
    if ((DeviceState != DEVSTATE_CONFIGURED) ||
        (Setup.bmRequestType != (OUT_DIRECT | CLASS_REQUEST | INTERFACE_RECIPIENT)))
    {
        usb_setup_stall();
        return;
    }

    bHidIdle = Setup.wValueH;

    usb_setup_status();
}

void usb_get_protocol()
{
    usb_setup_stall();
}

void usb_set_protocol()
{
    usb_setup_stall();
}

void msClickLeftDown()
{
    bKeyCode=0xfe;
    fKeyOK = 1;
    usb_class_in();
}

void msClickLeftUp()
{
    bKeyCode=0x55;
    fKeyOK = 1;
    usb_class_in();
}

void msMoveLeft()
{
    bKeyCode=0xef;
    fKeyOK = 1;
}

void msMoveRight()
{
    bKeyCode=0xdf;
    fKeyOK = 1;
}

void msMoveUp()
{
    bKeyCode=0xbf;
    fKeyOK = 1;
}

void msMoveDown()
{
    bKeyCode=0x7f;
    fKeyOK = 1;
}

void usb_class_in()
{
    BYTE button[3];
    BYTE i;
        
    if (DeviceState != DEVSTATE_CONFIGURED)
        return;

    if (!UsbInBusy && fKeyOK)
    {
        fKeyOK = 0;

        button[0] = 0;
        button[1] = 0;
        button[2] = 0;

        switch (bKeyCode)
        {
            case 0xfe:  button[0] = 0x01; break;//���
            case 0xfd:  button[0] = 0x04; break;//�м�
            case 0xfb:  button[0] = 0x02; break;//�Ҽ�
            case 0x55:  break;//��갴���ɿ�
            case 0xef:  button[1] = 0xfc; break;//����
            case 0xdf:  button[1] = 0x04; break;//����
            case 0xbf:  button[2] = 0xfc; break;//����
            case 0x7f:  button[2] = 0x04; break;//����
            
        }
        
        IE2 &= ~0x80;   //EUSB = 0;
        UsbInBusy = 1;
        usb_write_reg(INDEX, 1);
        for (i=0; i<3; i++)
        {
            usb_write_reg(FIFO1, button[i]);
        }
        usb_write_reg(INCSR1, INIPRDY);
        IE2 |= 0x80;    //EUSB = 1;
    }
}


void keyHandle()
{
    static bit bak_f1=1,bak_f2=1,bak_f3=1,bak_f4=1,bak_f5=1,bak_k1=1,bak_k2=1;
    if(cnt>=20 && longPressBegin == 2)
    {
        if(!sta_f3)
        {
            msMoveUp();
            bKeyCode=0xbf;
            fKeyOK = 1;
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
            msClickLeftDown();
            msClickLeftUp();
//				CR = 0;     //0��ֹͣ  1������PCA��ʱ��
//				TR0 = 0;		//1:��ʱ��0��ʼ��ʱ 0:ֹͣ
            buzz =0;
//				UartSendStr("msClickLeftUp\r\n");
        }
        else
        {
            
        }
        bak_f1 = sta_f1;
    }
    
    if(sta_k1 != bak_k1) //����ǵ������
    {
        if(!bak_k1) //�������º����ɿ��˱�ʾ�Ѿ����һ�ζ���
        {
            msClickLeftUp();
//				CR = 0;     //0��ֹͣ  1������PCA��ʱ��
        }
        else
        {
            msClickLeftDown();
//				CR = 1;     //0��ֹͣ  1������PCA��ʱ��
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

void timer1_isr() interrupt 3
{
    buzz = ~buzz;
}
void scan_key()
{
    static unsigned int f1buf=0xff,f2buf=0xff,f3buf=0xff,f4buf=0xff,f5buf=0xff,k1buf=0xff,k2buf=0xff;

	if(longPressBegin) //��ʱ�����ڼ�ⳤ��
	{
		cnt ++;
	}
	if(cnt>700 && longPressBegin==1) //ȷʵ���ڳ���
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
//    BYTE key;
//    
//    P0M0 = 0x00;
//    P0M1 = 0x00;
//    
//    key = 0;
//    P0 = 0xff;
//    P06 = 0;
//    _nop_();
//    _nop_();
//    key |= P0 & 0x0f;
//    P06 = 1;
//    P07 = 0;
//    _nop_();
//    _nop_();
//    key |= (P0 & 0x0f) << 4;
//    P07 = 1;
//    
//    if (key != bKeyCode)
//    {
//        bKeyCode = key;
//        bKeyDebounce = 20;
//    }
//    else
//    {
//        if (bKeyDebounce)
//        {
//            bKeyDebounce--;
//            if (bKeyDebounce == 0)
//            {
//                
//            }
//        }
//    }
    
    

}

