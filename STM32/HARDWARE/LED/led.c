#include "led.h"
   
//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void IO_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);	 //ʹ��PB,PC,PE�˿�ʱ��
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
// ????????? GPIO_Remap_SWJ_Disable SWJ ????(JTAG+SW-DP)
GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
// ????????? GPIO_Remap_SWJ_JTAGDisable ,JTAG-DP ?? + SW-DP ??
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOC,GPIO_Pin_13); 						 //PE.5 ����� 

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //LED0-->PB.9 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ; 	
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 
// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //beep
// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
// GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOB,GPIO_Pin_4); 						 	  
 //GPIO_SetBits(GPIOB,GPIO_Pin_4); 
}

 
