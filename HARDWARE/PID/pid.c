#include "pid.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

extern float  pitch,roll,yaw; 		//ŷ����
extern float  Measure_X,Measure_Y;          //�뾶��ת��߶�
extern float  Target_X,Target_Y;     //���Ŀ��ֵ    

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

void PID_Init(PID_struct *PID,float kp,float ki,float kd)//��ʼ��PID�ṹ�����
{
	PID->err = 0;    //��ǰ���
	PID->err_last = 0;   //ǰһ�����
	PID->integral = 0;  //�ۼƴ���
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




