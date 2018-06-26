#include <reg52.h>    //包含单片机寄存器的头文件
#include <string.H>
#include "intrins.h"


#define uchar unsigned char
#define uint  unsigned int

//以下是你的51单片机的晶振大小
#define FOSC_110592M



//以下是GSM模块返回数据
uchar rec_data[15]={0};
uchar rrec_data[15]={0};
uchar rec_ok[2]={0};    //接收短信内容结束标志
uchar rec_num;
uchar rrec_num;

uchar dat_num = 0;   //读取数据的位置

sbit led2=P1^4;    //短信发送指示灯
sbit JDQ1 = P2^3;
sbit JDQ2 = P2^2;
sbit KEY1 = P2^0;
sbit KEY2 = P2^1;

bit flag=0;

bit rev_start=0;
bit rev_start_num=0;
bit rrev_start=0;
bit rrev_start_num=0;

uchar delete_sm=0;    //口令错误标志
uchar ok_num=0;     //读取短信结束标志符  为2时结束读取

void SerialInti()//初始化程序（必须使用，否则无法收发）
{
	TMOD=0x20;//定时器1操作模式2:8位自动重载定时器
#ifdef FOSC_12M		   //在这里根据晶振大小设置不同的数值初始化串口
	TH1=0xf3;//装入初值，波特率2400
	TL1=0xf3;	
#else 	
	TH1=0xfd;//装入初值，波特率9600
	TL1=0xfd;
#endif //end of SOC_12M
	
	TR1=1;//打开定时器
	SM0=0;//设置串行通讯工作模式，（10为一部发送，波特率可变，由定时器1的溢出率控制）
	SM1=1;//(同上)在此模式下，定时器溢出一次就发送一个位的数据
	REN=1;//串行接收允许位（要先设置sm0sm1再开串行允许）
	EA=1;//开总中断
	ES=1;//开串行口中断	
}


//串行口连续发送char型数组，遇到终止号/0将停止
void Uart1Sends(uchar *str)
{
	while(*str!='\0')
	{
		SBUF=*str;
		while(!TI);//等待发送完成信号（TI=1）出现
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

void init_read_sms()            //多次发送指令使GSM适应9600波特率    
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
	
		Uart1Sends("AT+CNMI=2,2\r\n");           //设置收到短信，直接输出内容
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		 Delay300ms()	;	//@11.0592MHz
		 Uart1Sends("AT+CNMI=2,2\r\n");           //设置收到短信，直接输出内容
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		Delay150ms();		//@11.0592MHz
		Uart1Sends("AT+CMGF=1\r\n");
		 Delay300ms();		//@11.0592MHz
		 Uart1Sends("AT+CNMI=2,2\r\n");           //设置收到短信，直接输出内容
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
bit red_ok=0;     //接收到短信
void main()
{
	 
	 Delay1000ms();		//@11.0592MHz          初始化系统，使GSM适应9600波特率
	 Delay200ms();		//@11.0592MHz
	 Delay150ms();		//@11.0592MHz
	 SerialInti();    //串行中断初始化
	 Delay300ms();		//@11.0592MHz
	 init_read_sms();
	 Delay1000ms();		//@11.0592MHz
	 SerialInti();    //串行中断初始化
	 Delay300ms();		//@11.0592MHz
	 init_read_sms();
	 Delay300ms();		//@11.0592MHz
		led2=0;
  while(1)
  {
		if(!KEY1)
		{
			Delay50ms();   //按键延时消抖
				if(!KEY1)
				{
					while(!KEY1);
						JDQ1=!JDQ1;
				}
		}
		if(!KEY2)
			{
				Delay50ms();   //按键延时消抖
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


/*串行通讯中断，收发完成将进入该中断*/
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
	RI=0;//接收中断信号清零，表示将继续接收
	ES=1;
	
}