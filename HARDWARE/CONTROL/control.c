#include "control.h"
#include "pwm.h"
#include "pid.h"
#include "lcd.h"
#include "math.h"

#define Period 1670       //单摆周期（ms）,一般在1.3~1.6s之间
#define pi 3.1415926

int delaytime=0,time=0;
int step=5,all_step=5;

float Basic_Amplitude=30,Amplitude_x=20,Amplitude_y=20,Phase=pi/2,Alpha;    //振幅X，振幅Y，相位差，时间变量
float Height=88,Measure_X=0,Measure_Y=0;          //半径，转点高度
float Target_X,Target_Y;     //电机目标值              
float outx;
float outy;  //输出

extern float pitch,roll,yaw; 		//欧拉角
extern u8 oled_mode;
extern u32 TimeCnt;//运动时间计数器
int mydelay(int time)
{
		delaytime++;
		if(delaytime>=time)
			return 1;
		else
			return
				0;
}
void time_reset()
{
	delaytime=0;
	time=0;
}

//A15 A1 A2 A3
void stop()
{
	TIM_SetCompare1(TIM2,0);
	TIM_SetCompare2(TIM2,0);
	TIM_SetCompare3(TIM2,0);
	TIM_SetCompare4(TIM2,0);
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,0);
	TIM_SetCompare3(TIM3,0);
	TIM_SetCompare4(TIM3,0);
}

void seppd1(u16 speed,u8 mode) //风力向右 1号电机 A15 A1
{
	if(mode == 'z')
		{
			TIM_SetCompare2(TIM2,0);
			TIM_SetCompare1(TIM2,speed); 
		}
	else if(mode == 'y')
	  {
			TIM_SetCompare1(TIM2,0);
	    TIM_SetCompare2(TIM2,speed);
		}
	else
		{
			TIM_SetCompare1(TIM2,0);
	    TIM_SetCompare2(TIM2,0);
		}
}

void seppd2(u16 speed,u8 mode) //风力向右 1号电机 A15 A1
{
	if(mode == 'q')
		{
			TIM_SetCompare3(TIM2,0);
			TIM_SetCompare4(TIM2,speed); 
		}
	else if(mode == 'h')
	  {
			TIM_SetCompare4(TIM2,0);
	    TIM_SetCompare3(TIM2,speed);
		}
	else
		{
			TIM_SetCompare4(TIM2,0);
	    TIM_SetCompare3(TIM2,0);
		}
}

void seppd3(u16 speed,u8 mode) //风力向右 1号电机 A15 A1
{
	if(mode == 'z')
		{
			TIM_SetCompare1(TIM3,0);
			TIM_SetCompare2(TIM3,speed); 
		}
	else if(mode == 'y')
	  {
			TIM_SetCompare2(TIM3,0);
	    TIM_SetCompare1(TIM3,speed);
		}
	else
		{
			TIM_SetCompare1(TIM3,0);
	    TIM_SetCompare2(TIM3,0);
		}
}

void seppd4(u16 speed,u8 mode) //风力向右 1号电机 A15 A1
{
	if(mode == 'q')
		{
		TIM_SetCompare4(TIM3,0);
	    TIM_SetCompare3(TIM3,speed);
		}
	else if(mode == 'h')
	  {
			
				TIM_SetCompare3(TIM3,0);
			TIM_SetCompare4(TIM3,speed); 
		}
	else
		{
			TIM_SetCompare3(TIM3,0);
	    TIM_SetCompare4(TIM3,0);
		}
}
void PID_control()
{
	 	outx=PID_calc(&left_PID,get_error('x'));
	  outy=PID_calc(&right_PID,get_error('y'));
	
    if(outx<-1000) outx=-1000;	
		if(outx>1000)  outx= 1000;	
	  if(outy<-1000) outy=-1000;	
		if(outy>1000)  outy= 1000;	
 //  else {seppd1(0,0);seppd3(0,0);seppd2(0,0);seppd4(0,0);}		
}
	

void all()
{
	switch(oled_mode)
	{
		case 0: swing_angle();break;
		case 1: swing_angle();break;
		case 2: swing_angle();break;
		case 3: stop();break;
	}
}

void swing_angle()
{
	switch(all_step)
	{
		case 0:swing_start();break;   //时间计数
		case 1:break;
	}
}

//	 outx =1000;
//	 seppd1(outx*0.84,'y');
//   seppd3(outx,'z');

void swing_start()  //起摆控制
{
	  Alpha=(double)TimeCnt/Period*2*pi;      //float不可省略，单摆周期处理成三角函数2π周期
		Target_X=Amplitude_x*sin(Alpha);       //X方向目标值函数
	 	Target_Y=Amplitude_y*sin(Alpha+Phase); //Y方向目标值函数
		Measure_X=(double)(tan((pitch)/180*2*pi)*Height);        //角度弧度换算与距离计算
		Measure_Y=(double)(tan((roll)/180*2*pi)*Height);       //角度弧度换算与距离计算
	  
	
    PID_control();
//	
		if(outx>0)		{ seppd1(outx,'y');	} //70差速
	  else if(outx<0)   {	seppd3(-outx,'z');}//seppd2(0,0);seppd4(0,0);}   	
		else {seppd3(0,0);seppd1(0,0);}
//		
	  if(outy>0)		{	seppd2(outy,'q');}//seppd1(0,0);seppd3(0,0);} 100差速
		else if(outy<0)	  {seppd4(-outy,'h');	}//seppd1(0,0);seppd3(0,0);}   
		else {seppd2(0,0);seppd4(0,0);}                                       //电机输出
}




//
//				if((int)Target_Y<0)
//				{
//					LCD_ShowChar(0,300,'-',16,0);		//显示负号
//					LCD_ShowNum(16,300,-(int)Target_Y,3,16);		//显示整数部分	
//				}else 
//				{LCD_ShowChar(0,300,'+',16,0);		//去掉负号 
//				LCD_ShowNum(16,300,(int)Target_Y,3,16);		//显示整数部分	    
//        }
//				if((int)Measure_Y<0)
//				{
//					LCD_ShowChar(80,300,'-',16,0);		//显示负号
//					LCD_ShowNum(96,300,-(int)Measure_Y,3,16);		//显示整数部分	
//				}else 
//				{LCD_ShowChar(80,300,'+',16,0);		//去掉负号 
//				LCD_ShowNum(96,300,(int)Measure_Y,3,16);		//显示整数部分	    
//        }
//				
	  //seppd4(300,'q');
	  //seppd3(1000,'z');
	  //seppd1(1000,'z');
	 //seppd4(600,'q');
	//	LCD_ShowNum(0,300,(int)Measure_X,4,16);	  	//显示小数部分 	
//		LCD_ShowNum(50,300,(int)Measure_Y,4,16);		//显示小数部分	

//							
//				
				
				
//				if((int)Target_X<0)
//				{
//					LCD_ShowChar(32,300,'-',16,0);		//显示负号
//					LCD_ShowNum(48,300,-(int)Target_X,3,16);		//显示整数部分	
//				}else 
//				{LCD_ShowChar(32,300,'+',16,0);		//去掉负号 
//				LCD_ShowNum(48,300,(int)Target_X,3,16);		//显示整数部分	    
//        }
//						if((int)Target_Y<0)
//				{
//					LCD_ShowChar(120,300,'-',16,0);		//显示负号
//					LCD_ShowNum(136,300,-(int)Target_Y,3,16);		//显示整数部分	
//				}else 
//				{LCD_ShowChar(120,300,'+',16,0);		//去掉负号 
//				LCD_ShowNum(136,300,(int)Target_Y,3,16);		//显示整数部分	    
//        }
//	Motor_X=Position_PID_X(Measure_X,Target_X);
//	Motor_Y=Position_PID_Y(Measure_Y ,Target_Y);
//	switch(step)
//	{
//		case 0: seppd1(1000,'z');seppd3(1000,'z');;step++;break;
//		case 1: time=mydelay(10);if(time == 1){time_reset();step++;}break; 
//		case 2: seppd1(0,0);seppd3(0,0);step++;break;
//	  case 3: time=mydelay(100);if(time == 1){time_reset();step++;}break;
//    case 4: step=0;	all_step++;	break;
//	}
