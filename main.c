#include <reg52.h>    //������Ƭ���Ĵ�����ͷ�ļ�
#include <string.H>
#include "intrins.h"


#define uchar unsigned char
#define uint  unsigned int

//���������51��Ƭ���ľ����С
#define FOSC_110592M



//������GSMģ�鷵������
uchar rec_data[15]={0};
uchar rrec_data[15]={0};
uchar rec_ok[2]={0};    //���ն������ݽ�����־
uchar rec_num;
uchar rrec_num;

uchar dat_num = 0;   //��ȡ���ݵ�λ��

sbit led2=P1^4;    //���ŷ���ָʾ��
sbit JDQ1 = P2^3;
sbit JDQ2 = P2^2;
sbit KEY1 = P2^0;
sbit KEY2 = P2^1;

bit flag=0;

bit rev_start=0;
bit rev_start_num=0;
bit rrev_start=0;
bit rrev_start_num=0;

uchar delete_sm=0;    //��������־
uchar ok_num=0;     //��ȡ���Ž�����־��  Ϊ2ʱ������ȡ

void SerialInti()//��ʼ�����򣨱���ʹ�ã������޷��շ���
{
	TMOD=0x20;//��ʱ��1����ģʽ2:8λ�Զ����ض�ʱ��
#ifdef FOSC_12M		   //��������ݾ����С���ò�ͬ����ֵ��ʼ������
	TH1=0xf3;//װ���ֵ��������2400
	TL1=0xf3;	
#else 	
	TH1=0xfd;//װ���ֵ��������9600
	TL1=0xfd;
#endif //end of SOC_12M
	
	TR1=1;//�򿪶�ʱ��
	SM0=0;//���ô���ͨѶ����ģʽ����10Ϊһ�����ͣ������ʿɱ䣬�ɶ�ʱ��1������ʿ��ƣ�
	SM1=1;//(ͬ��)�ڴ�ģʽ�£���ʱ�����һ�ξͷ���һ��λ������
	REN=1;//���н�������λ��Ҫ������sm0sm1�ٿ���������
	EA=1;//�����ж�
	ES=1;//�����п��ж�	
}


//���п���������char�����飬������ֹ��/0��ֹͣ
void Uart1Sends(uchar *str)
{
	while(*str!='\0')
	{
		SBUF=*str;
		while(!TI);//�ȴ���������źţ�TI=1������
		TI=0;
		str++;
	}
}

void Delay150ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 2;
	j = 13;
	k = 237;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Delay300ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 3;
	j = 26;
	k = 223;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Delay200ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 2;
	j = 103;
	k = 147;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void init_read_sms()            //��η���ָ��ʹGSM��Ӧ9600������    
{
		
    Uart1Sends("AT+IPR?\r\n");
	  Delay300ms();		//@11.0592MHz
	  Uart1Sends("AT+IPR?\r\n");
	  Delay300ms();		//@11.0592MHz
	  Uart1Sends("AT+IPR?\r\n");
	  Delay300ms();		//@11.0592MHz
	  Uart1Sends("AT+IPR?\r\n");
	  Delay300ms();		//@11.0592MHz
	  Uart1Sends("AT+IPR?\r\n");
	  Delay300ms();		//@11.0592MHz
	  Uart1Sends("AT+IPR?\r\n");
	  Delay300ms();		//@11.0592MHz
	  Uart1Sends("AT+IPR?\r\n");
	  Delay300ms();		//@11.0592MHz
	
		Uart1Sends("AT+IPR?\r\n");
	  Delay300ms();		//@11.0592MHz
		Uart1Sends("AT+IPR?\r\n");
	  Delay300ms();		//@11.0592MHz
		Uart1Sends("AT+IPR?\r\n");
	  Delay300ms()	;	//@11.0592MHz
		Uart1Sends("AT+IPR?\r\n");
	  Delay300ms();		//@11.0592MHz
		
		Uart1Sends("AT+IPR?\r\n");
	  Delay300ms();		//@11.0592MHz
		Delay150ms();		//@11.0592MHz
	Uart1Sends("AT+IPR?\r\n");
		Delay150ms();		//@11.0592MHz

	Uart1Sends("AT+IPR?\r\n");
		Delay150ms();		//@11.0592MHz
	Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
	Uart1Sends("AT+CMGF=1\r\n");
		Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
  	Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
			Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
  	Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
		//Uart1Sends("AT+CSCS=\"GSM\"\r\n");	 
		Delay150ms();		//@11.0592MHz
	
		Uart1Sends("AT+CNMI=2,2\r\n");           //�����յ����ţ�ֱ���������
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		 Delay300ms()	;	//@11.0592MHz
		 Uart1Sends("AT+CNMI=2,2\r\n");           //�����յ����ţ�ֱ���������
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		 Delay300ms();		//@11.0592MHz
		 Uart1Sends("AT+CNMI=2,2\r\n");           //�����յ����ţ�ֱ���������
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		 Delay300ms();		//@11.0592MHz
}


void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 1;
	k = 243;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Delay50ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 90;
	j = 163;
	do
	{
		while (--j);
	} while (--i);
}
bit rev_start_cx=0;
bit rrev_start_cx=0;
bit red_send=0;
bit red_ok=0;     //���յ�����
void main()
{
	 
	 Delay1000ms();		//@11.0592MHz          ��ʼ��ϵͳ��ʹGSM��Ӧ9600������
	 Delay200ms();		//@11.0592MHz
	 Delay150ms();		//@11.0592MHz
	 SerialInti();    //�����жϳ�ʼ��
	 Delay300ms();		//@11.0592MHz
	 init_read_sms();
	 Delay1000ms();		//@11.0592MHz
	 SerialInti();    //�����жϳ�ʼ��
	 Delay300ms();		//@11.0592MHz
	 init_read_sms();
	 Delay300ms();		//@11.0592MHz
		led2=0;
  while(1)
  {
		if(!KEY1)
		{
			Delay50ms();   //������ʱ����
				if(!KEY1)
				{
					while(!KEY1);
						JDQ1=!JDQ1;
				}
		}
		if(!KEY2)
			{
				Delay50ms();   //������ʱ����
				if(!KEY2)
				{
					while(!KEY2);
						JDQ2=!JDQ2;
				}
		}
	if(rrev_start_cx==1)
	{	
		dat_num++;
		
		if(rrec_data[1]=='S')
		{	
			
			if(rrec_data[2]=='1')
			{
				
				switch(rrec_data[3])
				{
					case 'K' :  
											
											JDQ1 = 0;  
											//rrev_start_cx=0;
											break;
					case 'G' :  
											JDQ1 = 1;
											//rrev_start_cx=0;
											break;
					default  :  break;
				}	
			}
			 if(rrec_data[5]=='2')
			{
				
				switch(rrec_data[6])
				 {
					case 'K' :  
											JDQ2 = 0;
											rrev_start_cx=0; dat_num=0;
											break;
					case 'G' :  
											JDQ2 = 1;  
											rrev_start_cx=0; dat_num=0;
											break;
					default  :  break;
				 }	
			 }
			 rrev_start=0;rrev_start_cx=0; dat_num=0;
			}
		 if(dat_num>10)
		 {
			 rrev_start_cx=0;
			 dat_num=0;
			
		}
		rrev_start=0;rrev_start_cx=0; dat_num=0;
  }
 }
}


/*����ͨѶ�жϣ��շ���ɽ�������ж�*/
void Serial_interrupt() interrupt 4 
{
  uchar temp;
	ES=0;
	temp=SBUF;
if((temp=='$')&&(rrev_start==0))
	 {
		 	rrev_start_cx=0;
      rrev_start=1;
		  rrec_num=0;
	    rrec_data[7]=0;
    }
		if(rrev_start==1)
		{	
			rrec_data[rrec_num++]=temp;
	
				if(rrec_data[7]=='$')
				//if(rrec_data[rrec_num-1]=='$')
				{ 
					led2=1;
					rrec_num=0;
					rrev_start_cx=1;
				}
			if(rrec_num>12)
				{rrec_num=0;
					 dat_num=0;
					rrev_start=0;
					ok_num=2;
					rrev_start_cx=0;			
				}
		}
	RI=0;//�����ж��ź����㣬��ʾ����������
	ES=1;
	
}