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

/*全局变量*/
extern float pitch,roll,yaw; 		//欧拉角
extern u32 TimeCnt;
extern float Basic_Amplitude,Amplitude_x,Amplitude_y,Phase,Alpha;    //振幅X，振幅Y，相位差，时间变量
extern float Height,Measure_X,Measure_Y;          //半径，转点高度
extern float Target_X,Target_Y;     //电机目标值              
extern float outx;
extern float outy;  //输出
u32 timecot=0;
/*全局变量*/
unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
static float a;

void display(void);

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);                 //延时初始化
	uart_init(128000);           //初始化串口1
	LED_Init();					                           //初始化LED 
 	LCD_Init();					                           //LCD初始化
	MPU_Init();					                           //初始化MPU6050
	KEY_Init();  
	TIM2_PWM_Init(1000-1,84-1);	                   //84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/1000=1Khz.     
  TIM3_PWM_Init(1000-1,84-1);
	stop();                                    //电机初始化
 	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(30,220,200,16,16,"Pitch:    . C");	
 	LCD_ShowString(30,240,200,16,16," Roll:    . C");	 
 	LCD_ShowString(30,260,200,16,16," Yaw :    . C");	      //屏幕基本显式
	
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
	while(mpu_dmp_init())               //初始化陀螺仪
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
			DataScope_Get_Channel_Data(Target_X, 1 );  //传输波形图
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
			USART1->DR = DataScope_OutPut_Buffer[i];    //串口一传输
			}
			delay_ms(50); //20HZ		
		  display();
	}
}


void display()
{
	  int temp=0;					//
	 	POINT_COLOR=BLUE;//设置字体为蓝色  	
    temp=pitch*10;
	  if(temp<0)
		{
			LCD_ShowChar(30+48,220,'-',16,0);		//显示负号
			temp=-temp;		//转为正数
		}
		else
    		LCD_ShowChar(30+48,220,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,220,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,220,temp%10,3,16);		//显示小数部分 
			
		temp=roll*10;
			if(temp<0)
				{
					LCD_ShowChar(30+48,240,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}
			else
				LCD_ShowChar(30+48,240,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,240,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,240,temp%10,3,16);		//显示小数部分 	
			
			temp=yaw*10;
			if(temp<0)
				{
					LCD_ShowChar(30+48,260,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else LCD_ShowChar(30+48,260,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,260,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,260,temp%10,3,16);		//显示小数部分  
				LCD_ShowNum(0,280,TimeCnt,16,16);		//显示小数部分 	
}







//ALIENTEK 探索者STM32F407开发板 实验9
//PWM输出实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

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

//	int temp=0;					//温度
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
//	delay_init(168);                               //初始化延时函数
//	uart_init(1000);                               //初始化串口波特率为115200
//	LED_Init();					                           //初始化LED 
// 	LCD_Init();					                           //LCD初始化
//	MPU_Init();					                           //初始化MPU6050
//	KEY_Init();   

//	
//             
//	TIM2_PWM_Init(1000-1,84-1);	                   //84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/1000=1Khz.     
//  TIM3_PWM_Init(1000-1,84-1);
//	MPU6050_EXTI_Init();
//	stop();  //电机初始化
// 	POINT_COLOR=RED;//设置字体为红色 
//	
//	while(mpu_dmp_init())
//	{
//		LCD_ShowString(30,130,200,16,16,"MPU6050 Error");
//		delay_ms(200);
//		LCD_Fill(30,130,239,130+16,WHITE);
// 		delay_ms(200);
//	}

//	POINT_COLOR=BLUE;//设置字体为蓝色  	
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
//					LCD_ShowChar(30+48,220,'-',16,0);		//显示负号
//					temp=-temp;		//转为正数
//				}else LCD_ShowChar(30+48,220,' ',16,0);		//去掉负号 
//				LCD_ShowNum(30+48+8,220,temp/10,3,16);		//显示整数部分	    
//				LCD_ShowNum(30+48+40,220,temp%10,3,16);		//显示小数部分 
//			temp=roll*10;
//			if(temp<0)
//				{
//					LCD_ShowChar(30+48,240,'-',16,0);		//显示负号
//					temp=-temp;		//转为正数
//				}else LCD_ShowChar(30+48,240,' ',16,0);		//去掉负号 
//				LCD_ShowNum(30+48+8,240,temp/10,3,16);		//显示整数部分	    
//				LCD_ShowNum(30+48+40,240,temp%10,3,16);		//显示小数部分 	
//			temp=yaw*10;
//			if(temp<0)
//				{
//					LCD_ShowChar(30+48,260,'-',16,0);		//显示负号
//					temp=-temp;		//转为正数
//				}else LCD_ShowChar(30+48,260,' ',16,0);		//去掉负号 
//				LCD_ShowNum(30+48+8,260,temp/10,3,16);		//显示整数部分	    
//				LCD_ShowNum(30+48+40,260,temp%10,3,16);		//显示小数部分  
//				LCD_ShowNum(0,280,TimeCnt,16,16);		//显示小数部分 	
//			//	LCD_ShowNum(0,300,timecot,16,16);	
//	//    	LCD_ShowNum(50,300,Measure_X,4,16);	  	//显示小数部分 	
//	//	    LCD_ShowNum(200,300,Measure_Y,4,16);		//显示小数部分 
//	}
//}
