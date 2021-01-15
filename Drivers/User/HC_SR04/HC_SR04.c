#include "HC_SR04.h"
//PA15 TIM 2_CH3 ECHO_Reci1
//PB3  TIM 2_CH2 ECHO_Reci2





//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8 TIM2CH3_CAPTURE_STA=0; //输入捕获状态
u32 TIM2CH3_CAPTURE_VAL;//输入捕获值(TIM2/TIM5 是 32 位)

u8 TIM2CH4_CAPTURE_STA=0; //输入捕获状态
u32 TIM2CH4_CAPTURE_VAL;//输入捕获值(TIM2/TIM5 是 32 位)

//void TIM2_IRQHandler(void)
//{ 		   
//	//printf("TIM2_IRQHandler\r\n");
//	HAL_TIM_IRQHandler(&htim2);
//}




//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
////更新中断（溢出）发生时执行
//{
//	if(htim->Instance == TIM2)
//	{
//		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
//		{
//			if((TIM2CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
//			{
//				if(TIM2CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
//				{
//					if((TIM2CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
//					{
//						TIM2CH3_CAPTURE_STA|=0X80; //标记成功捕获了一次
//						TIM2CH3_CAPTURE_VAL=0XFFFFFFFF;
//					}else TIM2CH3_CAPTURE_STA++;
//				}
//			}
//		}
//		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
//		{
//			if((TIM2CH4_CAPTURE_STA&0X80)==0)//还未成功捕获
//			{
//				if(TIM2CH4_CAPTURE_STA&0X40)//已经捕获到高电平了
//				{
//					if((TIM2CH4_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
//					{
//						TIM2CH4_CAPTURE_STA|=0X80; //标记成功捕获了一次
//						TIM2CH4_CAPTURE_VAL=0XFFFFFFFF;
//					}else TIM2CH4_CAPTURE_STA++;
//				}
//			}
//		}
//	}
//}
//定时器输入捕获中断处理回调函数，该函数在 HAL_TIM_IRQHandler 中会被调用
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获中断发生时执行
{
	if(htim->Instance == TIM2)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			if((TIM2CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM2CH3_CAPTURE_STA&0X40) //捕获到一个下降沿
				{
					printf("捕获到下降沿\r\n");
					TIM2CH3_CAPTURE_STA|=0X80;//标记成功捕获到一次高电平脉宽
					 TIM2CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(
					&htim2,TIM_CHANNEL_3);//获取当前的捕获值.
					 TIM_RESET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_3); //一定要先清除原来的设置！！
					 TIM_SET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);
					//配置 TIM2 通道 1 上升沿捕获
				}else //还未开始,第一次捕获上升沿
				{
					printf("捕获到上升沿\r\n");
					TIM2CH3_CAPTURE_STA=0; //清空
					
					TIM2CH3_CAPTURE_STA|=0X40; //标记捕获到了上升沿
					TIM2CH3_CAPTURE_VAL=0;
					__HAL_TIM_DISABLE(&htim2);//关闭定时器 5
					__HAL_TIM_SET_COUNTER(&htim2,0);
					TIM_RESET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_3); //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);
					//定时器 5 通道 1 设置为下降沿捕获
					__HAL_TIM_ENABLE(&htim2);//使能定时器 1
				}
			}
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
			if((TIM2CH4_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM2CH4_CAPTURE_STA&0X40) //捕获到一个下降沿
				{
					TIM2CH4_CAPTURE_STA|=0X80;//标记成功捕获到一次高电平脉宽
					TIM2CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(
					&htim2,TIM_CHANNEL_4);//获取当前的捕获值.
					TIM_RESET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_4); //一定要先清除原来的设置！！
					 TIM_SET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);
					//配置 TIM2 通道 4 上升沿捕获
				}else //还未开始,第一次捕获上升沿
				{
					TIM2CH4_CAPTURE_STA=0; //清空
					TIM2CH4_CAPTURE_VAL=0;
					TIM2CH4_CAPTURE_STA|=0X40; //标记捕获到了上升沿
					__HAL_TIM_DISABLE(&htim2);//关闭定时器 5
					__HAL_TIM_SET_COUNTER(&htim2,0);
					TIM_RESET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_4); //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim2,
					TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);
					//定时器 1 通道 4 设置为下降沿捕获
					__HAL_TIM_ENABLE(&htim2);//使能定时器 1
				}
			}
		}
	}
}


//定时器2中断服务程序	 
//void TIM2_IRQHandler(void)
//{ 		   
//	//printf("TIM2_IRQHandler\r\n");
// 	if((TIM2CH_CAPTURE_STA[0]&0X80)==0)//还未成功捕获	
//	{
//		//printf("还未成功捕获\r\n");
//		if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//溢出
//		{	     
//			if(TIM2CH_CAPTURE_STA[0]&0X40)//已经捕获到高电平了
//			{
//				if((TIM2CH_CAPTURE_STA[0]&0X3F)==0X3F)//高电平太长了
//				{
//					//printf("高电平太长了 \r\n");
//					TIM2CH_CAPTURE_STA[0]|=0X80;		//标记成功捕获了一次
//					TIM2CH_CAPTURE_VAL[0]=0XFFFFFFFF;
//				}else TIM2CH_CAPTURE_STA[0]++;
//			}	 
//		}
//		if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
//		{	
//			//printf("捕获1发生捕获事件\r\n");
//			if(TIM2CH_CAPTURE_STA[0]&0X40)		//捕获到一个下降沿 		
//			{	  	
//				//printf("捕获到一个下降沿 \r\n");
//				TIM2CH_CAPTURE_STA[0]|=0X80;		//标记成功捕获到一次高电平脉宽
//			  TIM2CH_CAPTURE_VAL[0]=TIM_GetCapture1(TIM2);//获取当前的捕获值.
//				//printf("TIM2CH_CAPTURE_VAL:%d us\r\n",TIM2CH_CAPTURE_VAL[0]); 
//	 			TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
//			}else  								//还未开始,第一次捕获上升沿
//			{
//				//printf("捕获到一个上升沿 \r\n");
//				TIM2CH_CAPTURE_STA[0]=0;			//清空
//				TIM2CH_CAPTURE_VAL[0]=0;
//				TIM2CH_CAPTURE_STA[0]|=0X40;		//标记捕获到了上升沿
//				TIM_Cmd(TIM2,DISABLE ); 	//关闭定时器5
//	 			TIM_SetCounter(TIM2,0);
//	 			TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
//				TIM_Cmd(TIM2,ENABLE ); 	//使能定时器5
//			}		    
//		}			     	    					   
// 	}
//	
//	if((TIM2CH_CAPTURE_STA[1]&0X80)==0)//还未成功捕获	
//	{
//		if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//溢出
//		{	     
//			if(TIM2CH_CAPTURE_STA[1]&0X40)//已经捕获到高电平了
//			{
//				if((TIM2CH_CAPTURE_STA[1]&0X3F)==0X3F)//高电平太长了
//				{
//					TIM2CH_CAPTURE_STA[1]|=0X80;		//标记成功捕获了一次
//					TIM2CH_CAPTURE_VAL[1]=0XFFFFFFFF;
//				}else TIM2CH_CAPTURE_STA[1]++;
//			}	 
//		}
//		if(TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)//捕获1发生捕获事件
//		{	
//			if(TIM2CH_CAPTURE_STA[1]&0X40)		//捕获到一个下降沿 		
//			{	  			
//				TIM2CH_CAPTURE_STA[1]|=0X80;		//标记成功捕获到一次高电平脉宽
//			  TIM2CH_CAPTURE_VAL[1]=TIM_GetCapture2(TIM2);//获取当前的捕获值.
//	 			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
//			}else  								//还未开始,第一次捕获上升沿
//			{
//				TIM2CH_CAPTURE_STA[1]=0;			//清空
//				TIM2CH_CAPTURE_VAL[1]=0;
//				TIM2CH_CAPTURE_STA[1]|=0X40;		//标记捕获到了上升沿
//				TIM_Cmd(TIM2,DISABLE ); 	//关闭定时器5
//	 			TIM_SetCounter(TIM2,0);
//	 			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
//				TIM_Cmd(TIM2,ENABLE ); 	//使能定时器5
//			}		    
//		}			     	    					   
// 	}
//	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
//	TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_Update); //清除中断标志位
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
			if(TIM2CH3_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
			{
				printf("TIM2CH_CAPTURE_STA:%d us\r\n",TIM2CH3_CAPTURE_STA);
				temp=TIM2CH3_CAPTURE_STA&0X3F; 
				printf("%lld\r\n",temp);
				temp*=0XFFFFFFFF;		 		         //溢出时间总和
				temp+=TIM2CH3_CAPTURE_VAL;		   //得到总的高电平时间
				printf("HIGH:%lld us\r\n",temp); //打印总的高点平时间
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
				temp*=0XFFFFFFFF;		 		         //溢出时间总和
				temp+=TIM2CH4_CAPTURE_VAL;		   //得到总的高电平时间
				printf("HIGH:%lld us\r\n",temp); //打印总的高点平时间
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
