#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "pwm.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "key.h"
#include "control.h"
#include "pid.h"
#include "math.h"
#include "DataScope_DP.h"

#define  pi 3.1415926

/*ȫ�ֱ���*/
extern float pitch,roll,yaw; 		//ŷ����
extern u32 TimeCnt;
extern float Basic_Amplitude,Amplitude_x,Amplitude_y,Phase,Alpha;    //���X�����Y����λ�ʱ�����
extern float Height,Measure_X,Measure_Y;          //�뾶��ת��߶�
extern float Target_X,Target_Y;     //���Ŀ��ֵ              
extern float outx;
extern float outy;  //���
u32 timecot=0;
/*ȫ�ֱ���*/
unsigned char i;          //��������
unsigned char Send_Count; //������Ҫ���͵����ݸ���
static float a;

void display(void);

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);                 //��ʱ��ʼ��
	uart_init(128000);           //��ʼ������1
	LED_Init();					                           //��ʼ��LED 
 	LCD_Init();					                           //LCD��ʼ��
	MPU_Init();					                           //��ʼ��MPU6050
	KEY_Init();  
	TIM2_PWM_Init(1000-1,84-1);	                   //84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/1000=1Khz.     
  TIM3_PWM_Init(1000-1,84-1);
	stop();                                    //�����ʼ��
 	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(30,220,200,16,16,"Pitch:    . C");	
 	LCD_ShowString(30,240,200,16,16," Roll:    . C");	 
 	LCD_ShowString(30,260,200,16,16," Yaw :    . C");	      //��Ļ������ʽ
	
	LCD_ShowString(60,0,200,32,16,"swing system!");
	LCD_ShowString(0,96,200,16,16,"Basic:");
	LCD_ShowString(0,112,200,16,16,"angle:");
	LCD_ShowString(0,128,200,16,16,"X_range:");
	LCD_ShowString(0,144,200,16,16,"Y_range:");
	LCD_ShowString(0,16,200,16,16,"Key1    :mode change!");
	LCD_ShowString(0,32,200,16,16,"Key2&3  :range change!");
	LCD_ShowString(0,48,200,16,16,"Key4    :start!");
	LCD_ShowString(0,64,200,16,16,"Key5&6&7:PID change!");
  LCD_ShowString(0,160,200,16,16,"P:");
	LCD_ShowString(80,160,200,16,16,"I:   .");
	LCD_ShowString(160,160,200,16,16,"D:");
 LCD_ShowString(0,80,200,16,16,"mode:");
	
	MPU6050_EXTI_Init();
	while(mpu_dmp_init())               //��ʼ��������
	{
		LCD_ShowString(30,130,200,16,16,"MPU6050 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}
	
	 PID_Init(&left_PID,8,0,0);
   PID_Init(&right_PID,8,0,0);
	
	while(1)
		{
			Lcd_show();
			DataScope_Get_Channel_Data(Target_X, 1 );  //���䲨��ͼ
			DataScope_Get_Channel_Data(Measure_X, 2 );
			DataScope_Get_Channel_Data(Target_Y, 3 );
			DataScope_Get_Channel_Data(Measure_Y, 4 );
			DataScope_Get_Channel_Data(get_error('x'), 5 );
			DataScope_Get_Channel_Data(get_error('y'), 6 );
			DataScope_Get_Channel_Data(outx, 7 );
			DataScope_Get_Channel_Data(outy, 8 );
			Send_Count = DataScope_Data_Generate(10);
			for( i = 0 ; i < Send_Count; i++) 
			{
			while((USART1->SR&0X40)==0);  
			USART1->DR = DataScope_OutPut_Buffer[i];    //����һ����
			}
			delay_ms(50); //20HZ		
		  display();
	}
}


void display()
{
	  int temp=0;					//
	 	POINT_COLOR=BLUE;//��������Ϊ��ɫ  	
    temp=pitch*10;
	  if(temp<0)
		{
			LCD_ShowChar(30+48,220,'-',16,0);		//��ʾ����
			temp=-temp;		//תΪ����
		}
		else
    		LCD_ShowChar(30+48,220,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,220,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,220,temp%10,3,16);		//��ʾС������ 
			
		temp=roll*10;
			if(temp<0)
				{
					LCD_ShowChar(30+48,240,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}
			else
				LCD_ShowChar(30+48,240,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,240,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,240,temp%10,3,16);		//��ʾС������ 	
			
			temp=yaw*10;
			if(temp<0)
				{
					LCD_ShowChar(30+48,260,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,260,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,260,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,260,temp%10,3,16);		//��ʾС������  
				LCD_ShowNum(0,280,TimeCnt,16,16);		//��ʾС������ 	
}







//ALIENTEK ̽����STM32F407������ ʵ��9
//PWM���ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

//  TIM_SetCompare1(TIM2,249);
//	TIM_SetCompare2(TIM2,300);
//	TIM_SetCompare3(TIM2,350);
//	TIM_SetCompare4(TIM2,400);
//	TIM_SetCompare1(TIM3,249);
//	TIM_SetCompare2(TIM3,300);
//	TIM_SetCompare3(TIM3,350);
//	TIM_SetCompare4(TIM3,400);\
//			TIM_SetCompare2(TIM2,0);
//			TIM_SetCompare1(TIM2,500); 
//			seppd1(800,'z');
//		seppd2(800,'q');
//		seppd3(800,'z');
//		seppd4(800,'q');





//int main(void)
//{ 

//	int temp=0;					//�¶�
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
//	delay_init(168);                               //��ʼ����ʱ����
//	uart_init(1000);                               //��ʼ�����ڲ�����Ϊ115200
//	LED_Init();					                           //��ʼ��LED 
// 	LCD_Init();					                           //LCD��ʼ��
//	MPU_Init();					                           //��ʼ��MPU6050
//	KEY_Init();   

//	
//             
//	TIM2_PWM_Init(1000-1,84-1);	                   //84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/1000=1Khz.     
//  TIM3_PWM_Init(1000-1,84-1);
//	MPU6050_EXTI_Init();
//	stop();  //�����ʼ��
// 	POINT_COLOR=RED;//��������Ϊ��ɫ 
//	
//	while(mpu_dmp_init())
//	{
//		LCD_ShowString(30,130,200,16,16,"MPU6050 Error");
//		delay_ms(200);
//		LCD_Fill(30,130,239,130+16,WHITE);
// 		delay_ms(200);
//	}

//	POINT_COLOR=BLUE;//��������Ϊ��ɫ  	
// 		
//	LCD_ShowString(30,220,200,16,16,"Pitch:    . C");	
// 	LCD_ShowString(30,240,200,16,16," Roll:    . C");	 
// 	LCD_ShowString(30,260,200,16,16," Yaw :    . C");	    
//while(1)
//	{	
//	    Lcd_show();
//		
//		
//		
//		
//      temp=pitch*10;
//			if(temp<0)
//				{
//					LCD_ShowChar(30+48,220,'-',16,0);		//��ʾ����
//					temp=-temp;		//תΪ����
//				}else LCD_ShowChar(30+48,220,' ',16,0);		//ȥ������ 
//				LCD_ShowNum(30+48+8,220,temp/10,3,16);		//��ʾ��������	    
//				LCD_ShowNum(30+48+40,220,temp%10,3,16);		//��ʾС������ 
//			temp=roll*10;
//			if(temp<0)
//				{
//					LCD_ShowChar(30+48,240,'-',16,0);		//��ʾ����
//					temp=-temp;		//תΪ����
//				}else LCD_ShowChar(30+48,240,' ',16,0);		//ȥ������ 
//				LCD_ShowNum(30+48+8,240,temp/10,3,16);		//��ʾ��������	    
//				LCD_ShowNum(30+48+40,240,temp%10,3,16);		//��ʾС������ 	
//			temp=yaw*10;
//			if(temp<0)
//				{
//					LCD_ShowChar(30+48,260,'-',16,0);		//��ʾ����
//					temp=-temp;		//תΪ����
//				}else LCD_ShowChar(30+48,260,' ',16,0);		//ȥ������ 
//				LCD_ShowNum(30+48+8,260,temp/10,3,16);		//��ʾ��������	    
//				LCD_ShowNum(30+48+40,260,temp%10,3,16);		//��ʾС������  
//				LCD_ShowNum(0,280,TimeCnt,16,16);		//��ʾС������ 	
//			//	LCD_ShowNum(0,300,timecot,16,16);	
//	//    	LCD_ShowNum(50,300,Measure_X,4,16);	  	//��ʾС������ 	
//	//	    LCD_ShowNum(200,300,Measure_Y,4,16);		//��ʾС������ 
//	}
//}
