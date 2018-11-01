#include "delay.h"
#include "sys.h"
#include "rc522.h"
#include "lcd.h"			 //显示模块
#include "usart.h"
#include "string.h" 
#include "spi.h" 
#include "led.h" 
#include "adc.h" 
//////////////////////////////////////////////////////////
//M1卡分為16個扇區，每個扇區由4塊（塊0、塊1、塊2、塊3）組成
//我們也將16個扇區的64個塊按絕對地址編號0~63
//第0扇區的塊0（即絕對地址0塊），他用於存放廠商代碼，已經固化，不可更改
//每個扇區的塊0、塊1、塊2為數據塊，可用於存放數據
//每個扇區的塊3為控制塊（絕對地址塊3、7、11....），包括了密碼A，存取控制、密碼B。

/*******************************
*连线说明：
*1--SS  <----->PF0
*2--SCK <----->PB13
*3--MOSI<----->PB15
*4--MISO<----->PB14
*5--悬空
*6--GND <----->GND
*7--RST <----->PF1
*8--VCC <----->VCC
************************************/
/*全局变量*/
unsigned char CT[2];//卡类型
unsigned char SN[4]; //卡号
unsigned char RFID[16];			//存放RFID 
unsigned char lxl_bit=0;
unsigned char card1_bit=0;
unsigned char card2_bit=0;
unsigned char card3_bit=0;
unsigned char card4_bit=0;
unsigned char total=0;
unsigned char GLY[4]={0x05,0x03,0x90,0x3a};
unsigned char card_1[4]={0x70,0x8f,0xaa,0xc4};
unsigned char card_2[4]={6,101,22,253};
unsigned char card_3[4]={150,111,197,189};
unsigned char card_4[4]={54,209,196,189};
u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char RFID1[16]={0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x07,0x80,0x29,0xff,0xff,0xff,0xff,0xff,0xff};
/*函数声明*/
void PutNum(u16 x,u16 y, u32 n1,u8 n0, u16 charColor, u16 bkColor);	//显示余额函数
void Store(u8 *p,u8 store,u8 cash);//最重要的一个函数	
int main(void)
{		
	unsigned char status;
	unsigned char s=0x08;
	//u8 Data[16];
	//u8 i;
	//u8 k;//读写错误重试次数

 	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);
	InitRc522();				//初始化射频卡模块
	//Adc_Init();
	IO_Init();	
	beep=0;
	DOOR=0;
	printf("初始化完成");
  	while(1) 
	{		
			if(PBin(1)==0)
			{
				delay_ms(10);
				if(PBin(1)==0)
				{
					printf("dangerous");//
				}
			}
			//printf("%d \r\n",Get_Adc(9));
			DOOR=0;		//关门
			status = PcdRequest(PICC_REQALL,CT);/*尋卡*/
			if(status==MI_OK)//尋卡成功
			{
		//	 LCD_ShowString(0,30,200,16,16,"PcdRequest_MI_OK");
			 status=MI_ERR;
			 status = PcdAnticoll(SN);/*防冲撞*/
			 
			}
			if (status==MI_OK)//防衝撞成功
			{
	//			LCD_ShowString(150,30,200,16,16,"PcdAnticoll_MI_OK");
				status=MI_ERR;		
				//printf("ID:%02x %02x %02x %02x\n",SN[0],SN[1],SN[2],SN[3]);//发送卡号

				if((SN[0]==GLY[0])&&(SN[1]==GLY[1])&&(SN[2]==GLY[2])&&(SN[3]==GLY[3]))
				{
					lxl_bit=1;
					DOOR=1;
					beep=1;
					delay_ms(2000);
					beep=0;
					delay_ms(2000);
					beep=1;
					delay_ms(2000);
					beep=0;
				}
				if((SN[0]==card_1[0])&&(SN[1]==card_1[1])&&(SN[2]==card_1[2])&&(SN[3]==card_1[3]))
				{
					card1_bit=1;
					beep=1;
					delay_ms(2000);
					beep=0;
					delay_ms(2000);
					beep=1;
					delay_ms(2000);
					beep=0;
					DOOR=1;
					delay_ms(50000);
				}
				total=card1_bit+card2_bit+card3_bit+card4_bit+lxl_bit;
			//	LCD_ShowString(0,16,200,16,16,"total:");
			//	LCD_ShowNum(46,16,total,2,16);
				status =PcdSelect(SN);
				//Reset_RC522();
			}
			else
			{

			}
			if(status==MI_OK)//選卡成功
			{
			 //LCD_ShowString(0,50,200,16,16,"PcdAnticoll_MI_OK");
		//	 LCD_ShowString(0,48,200,16,16,"PcdSelect_MI_OK  ");
			 status=MI_ERR;
			 status =PcdAuthState(0x60,0x09,KEY,SN);
			 }
			 if(status==MI_OK)//驗證成功
			 {
			 // LCD_ShowString(0,64,200,16,16,"PcdAuthState_MI_OK  ");
			  status=MI_ERR;
			  status=PcdRead(s,RFID);
			  //status=PcdWrite(s,RFID1);
			  }

			if(status==MI_OK)//讀卡成功
			 {
			 // LCD_ShowString(0,80,200,16,16,"READ_MI_OK");
			  status=MI_ERR;
			  delay_ms(100);
			 }

	}
			
			
//////////////////////////////		
           }
			




/********************************
*函数功能：求p的n次幂
*/
int power(u8 p,u8 n)
{
	int pow=1;
	u8 i;
	for(i=0;i<n;i++)
	{
		pow*=p;	
	}
	return pow;
}
 
u8 ReadData(u8   addr,u8 *pKey,u8 *pSnr,u8 *dataout)
{
	u8 status,k;
	status=0x02;//
	k=5;
	do
    {
		status=PcdAuthState(PICC_AUTHENT1A,addr,pKey,pSnr);
		k--;
		//printf("AuthState is wrong\n");						      
    }while(status!=MI_OK && k>0);

	status=0x02;//
	k=5;
	do
    {
		status=PcdRead(addr,dataout);
		k--;
		//printf("ReadData is wrong\n");							      
    }while(status!=MI_OK && k>0);
	return status;
}
u8 WriteData(u8   addr,u8 *pKey,u8 *pSnr,u8 *datain)
{
	u8 status,k;
	status=0x02;//
	k=5;
	do
    {
		status=PcdAuthState(PICC_AUTHENT1A,addr,pKey,pSnr);
		k--;
		//printf("AuthState is wrong\n");						      
    }while(status!=MI_OK && k>0);

	status=0x02;//
	k=5;
	do
    {
		status=PcdWrite(addr,datain);
		k--;
		//printf("ReadData is wrong\n");							      
    }while(status!=MI_OK && k>0);
	return status;
}
void PutNum(u16 x,u16 y, u32 n1,u8 n0, u16 charColor, u16 bkColor)
{
//	u8 tmp[13];
//	u8 i;

	//LCD_SetRegion(0,0,239,319,FALSE);
//	tmp[0]=n1/1000000000+'0';
//	for(i=1;i<10;i++)
//	{
///		tmp[i]=n1/(1000000000/power(10,i))%10+'0';
//	}
//	tmp[10]='.';
//	tmp[11]=n0+'0';
//	tmp[12]=0;
	//DisplayString(x,y,tmp,charColor,bkColor);
	//LCD_ShowString(x,y,)

	
}
void Store(u8 *p,u8 store,u8 cash)
{

}

