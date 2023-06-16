#include "stm32f10x.h"
#include "Serial.h"
/*
    正转波形
    a:————____————
      b:————____————
    反转波形：
    a:————____————
  b:————____————
  将一相的下降沿用作触发中断，在中断时刻读取另一相的电平，即可判断旋转方向
*/

/*
    接线： 
*/

int16_t encoder_count = 0;
void encoder_init(void)
{
    /*1.配置RCC,打开外设时钟
    2.配置GPIO,选择端口为输入模式
    3.配置AFIO
    4.配置EXTI,选择中断触发方式、触发响应方式
    5.配置NVIC
    */
    //1
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    //2
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB,&GPIO_InitStructure);
    //3
   GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);//配置AFIO的数据选择器，选择中断引脚
   GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);//配置AFIO的数据选择器，选择中断引脚
    //4
   EXTI_InitTypeDef EXTI_InitStructure;
   EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断or事件
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//选择触发方式为下降沿
   EXTI_Init(&EXTI_InitStructure);
   
    //5
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置抢占和响应优先级
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//NVIC_PriorityGroup_2,抢占和响应优先级均为0~3
    NVIC_Init(&NVIC_InitStructure);//!!!!!!

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//NVIC_PriorityGroup_2,抢占和响应优先级均为0~3
    
    NVIC_Init(&NVIC_InitStructure);

}

/*在stm32中,中断函数的名字都是固定的，每个中断通道对应一个函数,无参无返回值
外部中断名字为：EXTI0_IRQHandler           ; EXTI Line 0
        EXTI1_IRQHandler           ; EXTI Line 1
        EXTI2_IRQHandler           ; EXTI Line 2
        EXTI3_IRQHandler           ; EXTI Line 3
        EXTI4_IRQHandler           ; EXTI Line 4
        EXTI15_10_IRQHandler       ; EXTI Line 15..10
        参考启动文件startup_stm32f10x_md.s文件
    中断编程建议：1.不要在中断函数调用delay等耗时的函数
                2.不要在中断函数里操作硬件，建议操作变量或标志位
*/
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) == SET)//对于中断通道10~15，应该在中断函数中调用EXTI_GetITStatus函数确定中断通道
    {
        
        
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0)
        {
            printf("enter exti0\r\n");
            encoder_count --;
        }
        EXTI_ClearITPendingBit(EXTI_Line0);//清除中断标志位，否则程序会一直进中断
    }
}

void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) == SET)//对于中断通道10~15，应该在中断函数中调用EXTI_GetITStatus函数确定中断通道
    {
        
        
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0)
        {
            printf("enter exti1\r\n");
            encoder_count ++;
        }
        EXTI_ClearITPendingBit(EXTI_Line1);//清除中断标志位，否则程序会一直进中断
    }
}

int16_t get_encoder_count(void)
{
    return encoder_count;
}