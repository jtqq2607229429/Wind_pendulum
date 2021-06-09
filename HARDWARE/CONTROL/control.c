#include "control.h"
#include "pwm.h"
#include "pid.h"
#include "lcd.h"
#include "math.h"

#define Period 1670       //�������ڣ�ms��,һ����1.3~1.6s֮��
#define pi 3.1415926

int delaytime=0,time=0;
int step=5,all_step=5;

float Basic_Amplitude=30,Amplitude_x=20,Amplitude_y=20,Phase=pi/2,Alpha;    //���X�����Y����λ�ʱ�����
float Height=88,Measure_X=0,Measure_Y=0;          //�뾶��ת��߶�
float Target_X,Target_Y;     //���Ŀ��ֵ              
float outx;
float outy;  //���

extern float pitch,roll,yaw; 		//ŷ����
extern u8 oled_mode;
extern u32 TimeCnt;//�˶�ʱ�������
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

void seppd1(u16 speed,u8 mode) //�������� 1�ŵ�� A15 A1
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

void seppd2(u16 speed,u8 mode) //�������� 1�ŵ�� A15 A1
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

void seppd3(u16 speed,u8 mode) //�������� 1�ŵ�� A15 A1
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

void seppd4(u16 speed,u8 mode) //�������� 1�ŵ�� A15 A1
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
		case 0:swing_start();break;   //ʱ�����
		case 1:break;
	}
}

//	 outx =1000;
//	 seppd1(outx*0.84,'y');
//   seppd3(outx,'z');

void swing_start()  //��ڿ���
{
	  Alpha=(double)TimeCnt/Period*2*pi;      //float����ʡ�ԣ��������ڴ�������Ǻ���2������
		Target_X=Amplitude_x*sin(Alpha);       //X����Ŀ��ֵ����
	 	Target_Y=Amplitude_y*sin(Alpha+Phase); //Y����Ŀ��ֵ����
		Measure_X=(double)(tan((pitch)/180*2*pi)*Height);        //�ǶȻ��Ȼ�����������
		Measure_Y=(double)(tan((roll)/180*2*pi)*Height);       //�ǶȻ��Ȼ�����������
	  
	
    PID_control();
//	
		if(outx>0)		{ seppd1(outx,'y');	} //70����
	  else if(outx<0)   {	seppd3(-outx,'z');}//seppd2(0,0);seppd4(0,0);}   	
		else {seppd3(0,0);seppd1(0,0);}
//		
	  if(outy>0)		{	seppd2(outy,'q');}//seppd1(0,0);seppd3(0,0);} 100����
		else if(outy<0)	  {seppd4(-outy,'h');	}//seppd1(0,0);seppd3(0,0);}   
		else {seppd2(0,0);seppd4(0,0);}                                       //������
}




//
//				if((int)Target_Y<0)
//				{
//					LCD_ShowChar(0,300,'-',16,0);		//��ʾ����
//					LCD_ShowNum(16,300,-(int)Target_Y,3,16);		//��ʾ��������	
//				}else 
//				{LCD_ShowChar(0,300,'+',16,0);		//ȥ������ 
//				LCD_ShowNum(16,300,(int)Target_Y,3,16);		//��ʾ��������	    
//        }
//				if((int)Measure_Y<0)
//				{
//					LCD_ShowChar(80,300,'-',16,0);		//��ʾ����
//					LCD_ShowNum(96,300,-(int)Measure_Y,3,16);		//��ʾ��������	
//				}else 
//				{LCD_ShowChar(80,300,'+',16,0);		//ȥ������ 
//				LCD_ShowNum(96,300,(int)Measure_Y,3,16);		//��ʾ��������	    
//        }
//				
	  //seppd4(300,'q');
	  //seppd3(1000,'z');
	  //seppd1(1000,'z');
	 //seppd4(600,'q');
	//	LCD_ShowNum(0,300,(int)Measure_X,4,16);	  	//��ʾС������ 	
//		LCD_ShowNum(50,300,(int)Measure_Y,4,16);		//��ʾС������	

//							
//				
				
				
//				if((int)Target_X<0)
//				{
//					LCD_ShowChar(32,300,'-',16,0);		//��ʾ����
//					LCD_ShowNum(48,300,-(int)Target_X,3,16);		//��ʾ��������	
//				}else 
//				{LCD_ShowChar(32,300,'+',16,0);		//ȥ������ 
//				LCD_ShowNum(48,300,(int)Target_X,3,16);		//��ʾ��������	    
//        }
//						if((int)Target_Y<0)
//				{
//					LCD_ShowChar(120,300,'-',16,0);		//��ʾ����
//					LCD_ShowNum(136,300,-(int)Target_Y,3,16);		//��ʾ��������	
//				}else 
//				{LCD_ShowChar(120,300,'+',16,0);		//ȥ������ 
//				LCD_ShowNum(136,300,(int)Target_Y,3,16);		//��ʾ��������	    
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
