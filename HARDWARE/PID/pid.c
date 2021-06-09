#include "pid.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

extern float  pitch,roll,yaw; 		//欧拉角
extern float  Measure_X,Measure_Y;          //半径，转点高度
extern float  Target_X,Target_Y;     //电机目标值    

PID_struct forward_PID;
PID_struct back_PID;
PID_struct left_PID;
PID_struct right_PID;

float get_error(u8 mode)
{
	float error;
	 if(mode == 'x')
	 {
		 error=Target_X-Measure_X;
		 return error;
	 }
	 
  else if(mode == 'y')
	 {
		 error=Target_Y-Measure_Y;
		 return error;
	 }
	 
	 
	else return 0;
}

void PID_Init(PID_struct *PID,float kp,float ki,float kd)//初始化PID结构体参数
{
	PID->err = 0;    //当前误差
	PID->err_last = 0;   //前一次误差
	PID->integral = 0;  //累计错误
	PID->Kp = kp;
	PID->Ki = ki;
	PID->Kd = kd;
	PID->out = 0;
}

float PID_calc(PID_struct *PID,float e)
{
	PID->err =e;
	PID->integral += PID->err;
	
	PID->out = PID->Kp * PID->err + 
							PID->Ki * PID->integral + 
							PID->Kd *( PID->err - PID->err_last );
	PID->err_last = PID->err;
	return PID->out;
}




