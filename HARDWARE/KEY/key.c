#include "key.h"
#include "delay.h" 
#include "lcd.h"
#include "math.h"
#include "pid.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//����������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
#define pi 3.1415926

//������ʼ������
void KEY_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOA,GPIOEʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //KEY0 ��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE4
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP��Ӧ����PA0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//WK_UP��Ӧ����PA0   Key1
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIOA0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//WK_UP��Ӧ����PA0   Key2 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIOA0
 
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//WK_UP��Ӧ����PA0   Key3
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOA0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//WK_UP��Ӧ����PA0   Key4
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOA0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//WK_UP��Ӧ����PA0   Key5 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA0
 
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//WK_UP��Ӧ����PA0   Key6
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//WK_UP��Ӧ����PA0   Key7
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIOA0
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	
	if(key_up&&(KEY1==1||KEY2==1||KEY3==1||KEY4==1||KEY5==1||KEY6==1||KEY7==1))
	{
		delay_ms(20);//ȥ���� 
		key_up=0;
		if(KEY1==1)return 1;
		else if(KEY2==1)return 2;
		else if(KEY3==1)return 3;
		else if(KEY4==1)return 4;
		else if(KEY5==1)return 5;
		else if(KEY6==1)return 6;
		else if(KEY7==1)return 7;
	} else if(KEY1==0&&KEY2==0&&KEY3==0&&KEY4==0&&KEY5==0&&KEY6==0&&KEY7==0)key_up=1; 	 
 	return 0;// �ް�������
}

u8 temp=0;
u8 zz=0;
u8 oled_mode=0;
int angle=0;
float Kp=8.0,Ki=0.0,Kd=0.0;
int ki_range;
extern int step,all_step;
extern float Basic_Amplitude,Phase,Amplitude_x,Amplitude_y;  

u8 pid_mode=1;
void Lcd_show()
{
	temp=KEY_Scan(0);
	switch(temp)
	{
		case 1: oled_mode=3;
      			zz++; if(zz>5)zz=0;
						break;	
		
		case 2:oled_mode=3;
			      if(zz==0||zz==2)
						{
						Phase = 0;
      			Basic_Amplitude+=5;
		        if(Basic_Amplitude>60)
						{Basic_Amplitude=20;}
						LCD_ShowNum(64,96,Basic_Amplitude,3,16);  //0 96
						Amplitude_x=Basic_Amplitude;
						Amplitude_y=Basic_Amplitude;
					  }
						
						if(zz==1)   //���ڽǶ�
						{
							angle+=15;						 
							if(angle>90)
						    {angle=0;}
							LCD_ShowNum(64,112,angle,3,16);
							if(angle<45)
							{
								Amplitude_x=Basic_Amplitude;
								Amplitude_y=Basic_Amplitude*tan(angle*pi/180);
							}
							else
							{
								Amplitude_x=Basic_Amplitude*tan((90-angle)*pi/180);
								Amplitude_y=Basic_Amplitude;		
							}
							}
						break;
						
		case 3:oled_mode=3;
			      if(zz==0||zz==2)
						{
						Phase = 0;
			      Basic_Amplitude-=5;
            if(Basic_Amplitude<20)
						{Basic_Amplitude=60;}
						 LCD_ShowNum(64,96,Basic_Amplitude,3,16);	
						 Amplitude_x=Basic_Amplitude;
						 Amplitude_y=Basic_Amplitude;
					  }			
	         if(zz==1)
						{
							angle-=15;
								if(angle<0)
						    {angle=90;}
									if(angle<=45)
									{
										Amplitude_x=Basic_Amplitude;
										Amplitude_y=Basic_Amplitude*tan(angle*pi/180);
									}
									else
									{
										Amplitude_x=Basic_Amplitude*tan((90-angle)*pi/180);
										Amplitude_y=Basic_Amplitude;		
									}
									LCD_ShowNum(64,112,angle,3,16);
						}						
 						break;
		case 4:oled_mode=zz; 
					if(oled_mode==1)   //ֱ��ģʽ
						{
							Phase = 0;
	           	if(angle<=45)
									{
										Amplitude_x=Basic_Amplitude;
										Amplitude_y=Basic_Amplitude*tan(angle*pi/180);
									}
							else
									{
										Amplitude_x=Basic_Amplitude*tan((90-angle)*pi/180);
										Amplitude_y=Basic_Amplitude;		
									}
						}				
						
						 if(oled_mode==4)
						{PID_Init(&left_PID,Kp,Ki,Kd);	}			
             if(oled_mode==5)						
						{PID_Init(&right_PID,Kp,Ki,Kd); }
			     if(oled_mode==2)
		        {
							Amplitude_x=Basic_Amplitude;
							Amplitude_y=Basic_Amplitude;
							Phase = pi/2;
						}
					
			      all_step=0;step=0;break;
		case 5: if(zz==4)      {pid_mode++;if(pid_mode > 3) pid_mode=1; }          break;
		case 6: if(pid_mode==1){ Kp++;	    }          
		        else if(pid_mode==2){ Ki+=0.1;	} 
						else if(pid_mode==3){ Kd++;	}       break;
		case 7: if (pid_mode==1){ Kp--;     }
	        	else if(pid_mode==2){ Ki-=0.1;	} 
		        else if(pid_mode==3){ Kd--;	} 
		                                                                            break;
	}
       if(zz==1){LCD_ShowString(80,80,200,16,16,"swing_angle OK");	}
	else if(zz==2){LCD_ShowString(80,80,200,16,16,"swing_round OK");	}
	else if(zz==3){LCD_ShowString(80,80,200,16,16,"    stop    OK");}
	else if(zz==4){LCD_ShowString(80,80,200,16,16,"pidx change OK");}
	else if(zz==5){LCD_ShowString(80,80,200,16,16,"pidy change OK");}
  else          {LCD_ShowString(80,80,200,16,16,"            NO!");}
	     if(pid_mode==1){LCD_ShowString(0,176,200,16,16,"P change  OK !");}
  else if(pid_mode==2){LCD_ShowString(0,176,200,16,16,"I change  OK !");}
	else if(pid_mode==3){LCD_ShowString(0,176,200,16,16,"D change  OK !");}
	else                 LCD_ShowString(0,176,200,16,16,"NO pid change!");
  LCD_ShowNum(16,160,(int)Kp,3,16);
	ki_range=Ki*10;
	if(ki_range<0)
	{	LCD_ShowChar(96,160,'-',16,0);		//��ʾ����
			ki_range=-ki_range;		//תΪ����
			}else LCD_ShowChar(96,160,'+',16,0);		//ȥ������ 
			LCD_ShowNum(102,160,ki_range/10,2,16);		//��ʾ��������	    
			LCD_ShowNum(130,160,ki_range%10,3,16);		//��ʾС������  	
	LCD_ShowNum(176,160,(int)Kd,3,16);
	//void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
  LCD_ShowNum(64,128,Amplitude_x,3,16);	
	LCD_ShowNum(64,144,(int)Amplitude_y,3,16);	
	LCD_ShowNum(48,80,zz,3,16);	
}














