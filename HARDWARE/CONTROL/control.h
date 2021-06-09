#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

void all(void);

//1000以内  00停止
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
////本程序只供学习使用，未经作者许可，不得用于其它任何用途
////ALIENTEK STM32F407开发板
////MPU6050 驱动代码	   
////正点原子@ALIENTEK
////技术论坛:www.openedv.com
////创建日期:2014/5/9
////版本：V1.0
////版权所有，盗版必究。
////Copyright(C) 广州市星翼电子科技有限公司 2014-2024
////All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////// 	

////初始化MPU6050
////返回值:0,成功
////    其他,错误代码
//#define pi 3.1415926

//float pitch = 3.0,roll = 3.0,yaw = 3.0; 		//欧拉角
//short gyro[3], accel[3], sensors;
//float ZHONGZHI_A,ZHONGZHI_B;                           //MPU6050传感器平衡位置时对应的初始值
//u8 Flag_Zero=1,count_Zero=0; //停止标志位和 显示标志位 零点读取标志位 默认停止 显示打开 读取零点

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

