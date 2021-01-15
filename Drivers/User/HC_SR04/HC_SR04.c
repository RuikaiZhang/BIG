#include "HC_SR04.h"
//PA15 TIM 2_CH3 ECHO_Reci1
//PB3  TIM 2_CH2 ECHO_Reci2





//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
u8 TIM2CH3_CAPTURE_STA=0; //���벶��״̬
u32 TIM2CH3_CAPTURE_VAL;//���벶��ֵ(TIM2/TIM5 �� 32 λ)

u8 TIM2CH4_CAPTURE_STA=0; //���벶��״̬
u32 TIM2CH4_CAPTURE_VAL;//���벶��ֵ(TIM2/TIM5 �� 32 λ)

//void TIM2_IRQHandler(void)
//{ 		   
//	//printf("TIM2_IRQHandler\r\n");
//	HAL_TIM_IRQHandler(&htim2);
//}




//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
////�����жϣ����������ʱִ��
//{
//	if(htim->Instance == TIM2)
//	{
//		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
//		{
//			if((TIM2CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
//			{
//				if(TIM2CH3_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
//				{
//					if((TIM2CH3_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
//					{
//						TIM2CH3_CAPTURE_STA|=0X80; //��ǳɹ�������һ��
//						TIM2CH3_CAPTURE_VAL=0XFFFFFFFF;
//					}else TIM2CH3_CAPTURE_STA++;
//				}
//			}
//		}
//		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
//		{
//			if((TIM2CH4_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
//			{
//				if(TIM2CH4_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
//				{
//					if((TIM2CH4_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
//					{
//						TIM2CH4_CAPTURE_STA|=0X80; //��ǳɹ�������һ��
//						TIM2CH4_CAPTURE_VAL=0XFFFFFFFF;
//					}else TIM2CH4_CAPTURE_STA++;
//				}
//			}
//		}
//	}
//}
//��ʱ�����벶���жϴ���ص��������ú����� HAL_TIM_IRQHandler �лᱻ����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//�����жϷ���ʱִ��
{
	if(htim->Instance == TIM2)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			if((TIM2CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM2CH3_CAPTURE_STA&0X40) //����һ���½���
				{
					printf("�����½���\r\n");
					TIM2CH3_CAPTURE_STA|=0X80;//��ǳɹ�����һ�θߵ�ƽ����
					 TIM2CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(
					&htim2,TIM_CHANNEL_3);//��ȡ��ǰ�Ĳ���ֵ.
					 TIM_RESET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_3); //һ��Ҫ�����ԭ�������ã���
					 TIM_SET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);
					//���� TIM2 ͨ�� 1 �����ز���
				}else //��δ��ʼ,��һ�β���������
				{
					printf("����������\r\n");
					TIM2CH3_CAPTURE_STA=0; //���
					
					TIM2CH3_CAPTURE_STA|=0X40; //��ǲ�����������
					TIM2CH3_CAPTURE_VAL=0;
					__HAL_TIM_DISABLE(&htim2);//�رն�ʱ�� 5
					__HAL_TIM_SET_COUNTER(&htim2,0);
					TIM_RESET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_3); //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);
					//��ʱ�� 5 ͨ�� 1 ����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&htim2);//ʹ�ܶ�ʱ�� 1
				}
			}
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
			if((TIM2CH4_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM2CH4_CAPTURE_STA&0X40) //����һ���½���
				{
					TIM2CH4_CAPTURE_STA|=0X80;//��ǳɹ�����һ�θߵ�ƽ����
					TIM2CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(
					&htim2,TIM_CHANNEL_4);//��ȡ��ǰ�Ĳ���ֵ.
					TIM_RESET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_4); //һ��Ҫ�����ԭ�������ã���
					 TIM_SET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);
					//���� TIM2 ͨ�� 4 �����ز���
				}else //��δ��ʼ,��һ�β���������
				{
					TIM2CH4_CAPTURE_STA=0; //���
					TIM2CH4_CAPTURE_VAL=0;
					TIM2CH4_CAPTURE_STA|=0X40; //��ǲ�����������
					__HAL_TIM_DISABLE(&htim2);//�رն�ʱ�� 5
					__HAL_TIM_SET_COUNTER(&htim2,0);
					TIM_RESET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_4); //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);
					//��ʱ�� 1 ͨ�� 4 ����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&htim2);//ʹ�ܶ�ʱ�� 1
				}
			}
		}
	}
}


//��ʱ��2�жϷ������	 
//void TIM2_IRQHandler(void)
//{ 		   
//	//printf("TIM2_IRQHandler\r\n");
// 	if((TIM2CH_CAPTURE_STA[0]&0X80)==0)//��δ�ɹ�����	
//	{
//		//printf("��δ�ɹ�����\r\n");
//		if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//���
//		{	     
//			if(TIM2CH_CAPTURE_STA[0]&0X40)//�Ѿ����񵽸ߵ�ƽ��
//			{
//				if((TIM2CH_CAPTURE_STA[0]&0X3F)==0X3F)//�ߵ�ƽ̫����
//				{
//					//printf("�ߵ�ƽ̫���� \r\n");
//					TIM2CH_CAPTURE_STA[0]|=0X80;		//��ǳɹ�������һ��
//					TIM2CH_CAPTURE_VAL[0]=0XFFFFFFFF;
//				}else TIM2CH_CAPTURE_STA[0]++;
//			}	 
//		}
//		if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//����1���������¼�
//		{	
//			//printf("����1���������¼�\r\n");
//			if(TIM2CH_CAPTURE_STA[0]&0X40)		//����һ���½��� 		
//			{	  	
//				//printf("����һ���½��� \r\n");
//				TIM2CH_CAPTURE_STA[0]|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
//			  TIM2CH_CAPTURE_VAL[0]=TIM_GetCapture1(TIM2);//��ȡ��ǰ�Ĳ���ֵ.
//				//printf("TIM2CH_CAPTURE_VAL:%d us\r\n",TIM2CH_CAPTURE_VAL[0]); 
//	 			TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
//			}else  								//��δ��ʼ,��һ�β���������
//			{
//				//printf("����һ�������� \r\n");
//				TIM2CH_CAPTURE_STA[0]=0;			//���
//				TIM2CH_CAPTURE_VAL[0]=0;
//				TIM2CH_CAPTURE_STA[0]|=0X40;		//��ǲ�����������
//				TIM_Cmd(TIM2,DISABLE ); 	//�رն�ʱ��5
//	 			TIM_SetCounter(TIM2,0);
//	 			TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
//				TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��5
//			}		    
//		}			     	    					   
// 	}
//	
//	if((TIM2CH_CAPTURE_STA[1]&0X80)==0)//��δ�ɹ�����	
//	{
//		if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//���
//		{	     
//			if(TIM2CH_CAPTURE_STA[1]&0X40)//�Ѿ����񵽸ߵ�ƽ��
//			{
//				if((TIM2CH_CAPTURE_STA[1]&0X3F)==0X3F)//�ߵ�ƽ̫����
//				{
//					TIM2CH_CAPTURE_STA[1]|=0X80;		//��ǳɹ�������һ��
//					TIM2CH_CAPTURE_VAL[1]=0XFFFFFFFF;
//				}else TIM2CH_CAPTURE_STA[1]++;
//			}	 
//		}
//		if(TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)//����1���������¼�
//		{	
//			if(TIM2CH_CAPTURE_STA[1]&0X40)		//����һ���½��� 		
//			{	  			
//				TIM2CH_CAPTURE_STA[1]|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
//			  TIM2CH_CAPTURE_VAL[1]=TIM_GetCapture2(TIM2);//��ȡ��ǰ�Ĳ���ֵ.
//	 			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
//			}else  								//��δ��ʼ,��һ�β���������
//			{
//				TIM2CH_CAPTURE_STA[1]=0;			//���
//				TIM2CH_CAPTURE_VAL[1]=0;
//				TIM2CH_CAPTURE_STA[1]|=0X40;		//��ǲ�����������
//				TIM_Cmd(TIM2,DISABLE ); 	//�رն�ʱ��5
//	 			TIM_SetCounter(TIM2,0);
//	 			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
//				TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��5
//			}		    
//		}			     	    					   
// 	}
//	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
//	TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_Update); //����жϱ�־λ
//}
#define TRIGSEND_CH3    0
#define TRIGSEND_CH4    1
#define GET_DIST_CH3    TRIGSEND_CH3    
#define GET_DIST_CH4    TRIGSEND_CH4    

void Trig_Send(u8 ch)
{
	switch (ch)
	{
		case TRIGSEND_CH3:
			TIM2CH3_CAPTURE_STA=0;
			HAL_GPIO_WritePin(HC_SR04_TRIG1_GPIO_Port,HC_SR04_TRIG1_Pin,GPIO_PIN_SET);
			printf("GPIO_SetBits\r\n");
			delay_us(30);
			HAL_GPIO_WritePin(HC_SR04_TRIG1_GPIO_Port,HC_SR04_TRIG1_Pin,GPIO_PIN_RESET);
		break;
		case TRIGSEND_CH4:
			TIM2CH4_CAPTURE_STA = 0;
			HAL_GPIO_WritePin(HC_SR04_TRIG2_GPIO_Port,HC_SR04_TRIG2_Pin,GPIO_PIN_SET);  
			delay_us(30);
			HAL_GPIO_WritePin(HC_SR04_TRIG2_GPIO_Port,HC_SR04_TRIG2_Pin,GPIO_PIN_RESET); 
		break;
	}
}


uint16_t Get_Dist(u8 ch)
{
	long long temp = 0;		 
	uint32_t dist = 0;
	printf("Get_Dist\r\n");
 	switch(ch)
	{
		case GET_DIST_CH3:
			if(TIM2CH3_CAPTURE_STA&0X80)        //�ɹ�������һ�θߵ�ƽ
			{
				printf("TIM2CH_CAPTURE_STA:%d us\r\n",TIM2CH3_CAPTURE_STA);
				temp=TIM2CH3_CAPTURE_STA&0X3F; 
				printf("%lld\r\n",temp);
				temp*=0XFFFFFFFF;		 		         //���ʱ���ܺ�
				temp+=TIM2CH3_CAPTURE_VAL;		   //�õ��ܵĸߵ�ƽʱ��
				printf("HIGH:%lld us\r\n",temp); //��ӡ�ܵĸߵ�ƽʱ��
				dist = temp*10/58;
				printf("dist = %d\r\n", dist);
				return dist;
			}
			else
			{
				return 0;
			}
		case GET_DIST_CH4:
			if(TIM2CH4_CAPTURE_STA&0X80)
			{
				//printf("TIM2CH_CAPTURE_STA:%d us\r\n",TIM2CH_CAPTURE_STA[i]);
				temp=TIM2CH4_CAPTURE_STA&0X3F; 
				printf("%lld\r\n",temp);
				temp*=0XFFFFFFFF;		 		         //���ʱ���ܺ�
				temp+=TIM2CH4_CAPTURE_VAL;		   //�õ��ܵĸߵ�ƽʱ��
				printf("HIGH:%lld us\r\n",temp); //��ӡ�ܵĸߵ�ƽʱ��
				dist = temp*10/58;
				printf("dist2 = %d\r\n", dist);
				return dist;
			}
			else
			{
				return 0;
			}
	}
	return 0;	
}
