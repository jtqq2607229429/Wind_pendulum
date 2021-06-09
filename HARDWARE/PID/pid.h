#ifndef __PID_H
#define __PID_H
#include "sys.h"

typedef struct pid{
    float err;                //����ƫ��ֵ
    float err_last;            //������һ��ƫ��ֵ
    float Kp,Ki,Kd;            //������������֡�΢��ϵ��
    float out;          //�����ѹֵ������ִ�����ı�����
    float integral;            //�������ֵ
}PID_struct;


extern PID_struct left_PID;
extern PID_struct right_PID;
extern PID_struct forward_PID;
extern PID_struct back_PID;

float get_error(u8 mode);
float pid(float kp,float ki,float kd,float e);
void PID_Init(PID_struct *PID,float kp,float ki,float kd);
float PID_calc(PID_struct *PID,float e);
#endif
