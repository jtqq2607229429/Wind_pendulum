//#include "timer.h"
//#include "stdio.h"
//#include "led.h"
//#include "control.h"

//#include "mpu6050.h"
//#include "inv_mpu.h"
//#include "inv_mpu_dmp_motion_driver.h" 

//extern float pitch,roll ,yaw ; 		//ŷ����
//extern u32 timecot;

//void TIM7_Int_Initl(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///ʹ��TIM3ʱ��
//	
//  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
//	
//	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
//	TIM_Cmd(TIM7,ENABLE); //ʹ�ܶ�ʱ��3
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //��ʱ��3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}

////��ʱ��3�жϷ�����
//void TIM7_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //����ж�
//	{
////		mpu_dmp_get_data(&pitch, &roll, &yaw);
////		roll = 180.0 + roll;
////		all();
//	  if(mpu_dmp_get_data(&pitch, &roll, &yaw)==0)
//	  pitch = -pitch;
//  	(roll >= 0) ? (roll = 180 - roll) : (roll = -(180 + roll));
//		all();
//	  TimeCnt+=10;
//	
//		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
//	}
//}


//void TIM1_Int_Initl(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrecture;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

//	TIM_TimeBaseInitStrecture.TIM_Period = arr;
//	TIM_TimeBaseInitStrecture.TIM_Prescaler = psc;
//	TIM_TimeBaseInitStrecture.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInitStrecture.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInitStrecture.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStrecture);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
//	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
//	TIM_Cmd(TIM1,ENABLE);

//}
//void TIM1_UP_TIM10_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //����ж�
//	{
////		USART3_RX_STA|=1<<15;	//��ǽ������
////		TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //���TIM7�����жϱ�־
////		TIM_Cmd(TIM1, DISABLE);  //�ر�TIM7 
//	}
//}

