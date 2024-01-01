#include "CH9329.h"
#include "delay.h"
#include "uart.h"

char msClickLeftDownCmd[] = {0x57, 0xAB, 0x00, 0x05, 0x05, 0x01, 0x01, 0x00, 0x00, 0x00, 0x0E};
char msClickLeftUpCmd[] = {0x57, 0xAB, 0x00, 0x05, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0D};
char msMoveLeftCmd[] = {0x57, 0xAB, 0x00, 0x05, 0x05, 0x01, 0x00, 0xF6, 0x00, 0x00, 0x03};   
char msMoveRightCmd[] = {0x57, 0xAB, 0x00, 0x05, 0x05, 0x01, 0x00, 0x0A, 0x00, 0x00, 0x17};  
char msMoveUpCmd[] = {0x57, 0xAB, 0x00, 0x05, 0x05, 0x01, 0x00, 0x00, 0xF6, 0x00, 0x03};
char msMoveDownCmd[] = {0x57, 0xAB, 0x00, 0x05, 0x05, 0x01, 0x00, 0x00, 0x0A, 0x00, 0x17};

void msClickLeftDown()
{
		int i = 0,j=0;
		i = sizeof(msClickLeftDownCmd);
		
		for(j=0;j<i;j++)
		{
			UartSend(msClickLeftDownCmd[j]);
		}	
}

void msClickLeftUp()
{
		int i = 0,j=0;
		i = sizeof(msClickLeftUpCmd);
		
		for(j=0;j<i;j++)
		{
			UartSend(msClickLeftUpCmd[j]);
		}
}

void msMoveLeft()
{
		int i = 0,j=0;
		i = sizeof(msMoveLeftCmd);
		
		for(j=0;j<i;j++)
		{
			UartSend(msMoveLeftCmd[j]);
		}
}

void msMoveRight()
{
		int i = 0,j=0;
		i = sizeof(msMoveRightCmd);
		
		for(j=0;j<i;j++)
		{
			UartSend(msMoveRightCmd[j]);
		}
}

void msMoveUp()
{
		int i = 0,j=0;
		i = sizeof(msMoveUpCmd);
		
		for(j=0;j<i;j++)
		{
			UartSend(msMoveUpCmd[j]);
		}
}

void msMoveDown()
{
		int i = 0,j=0;
		i = sizeof(msMoveDownCmd);
		
		for(j=0;j<i;j++)
		{
			UartSend(msMoveDownCmd[j]);
		}
}





