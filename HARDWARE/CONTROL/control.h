#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

void all(void);

//1000����  00ֹͣ
void seppd1(u16 speed,u8 mode);
void seppd2(u16 speed,u8 mode);
void seppd3(u16 speed,u8 mode);
void seppd4(u16 speed,u8 mode);
void stop(void);

void PID_control();

void swing_start(void);
void swing_angle(void);
#endif

//#include "mpu6050.h"
//#include "inv_mpu.h"
//#include "inv_mpu_dmp_motion_driver.h" 
//#include "sys.h"
//#include "delay.h"
//#include "usart.h"  
//#include "math.h"
//#include "control.h"
////////////////////////////////////////////////////////////////////////////////////	 
////������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
////ALIENTEK STM32F407������
////MPU6050 ��������	   
////����ԭ��@ALIENTEK
////������̳:www.openedv.com
////��������:2014/5/9
////�汾��V1.0
////��Ȩ���У�����ؾ���
////Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
////All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////// 	

////��ʼ��MPU6050
////����ֵ:0,�ɹ�
////    ����,�������
//#define pi 3.1415926

//float pitch = 3.0,roll = 3.0,yaw = 3.0; 		//ŷ����
//short gyro[3], accel[3], sensors;
//float ZHONGZHI_A,ZHONGZHI_B;                           //MPU6050������ƽ��λ��ʱ��Ӧ�ĳ�ʼֵ
//u8 Flag_Zero=1,count_Zero=0; //ֹͣ��־λ�� ��ʾ��־λ ����ȡ��־λ Ĭ��ֹͣ ��ʾ�� ��ȡ���

//void MPU6050_EXTI_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
//	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}


//void EXTI3_IRQHandler(void)
//{
//  mpu_dmp_get_data(&pitch, &roll, &yaw);
//  roll = 180.0 + roll;
//	all();
//	EXTI_ClearITPendingBit(EXTI_Line3); 
//}

