#include "stm32f10x.h"
#include "Serial.h"

/*
    接线：红外对射D0接B14脚
*/
static uint32_t count = 0;

void countsensor_init(void)
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
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB,&GPIO_InitStructure);
    //3
   GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);//配置AFIO的数据选择器，选择中断引脚
    //4
   EXTI_InitTypeDef exti_initstr;
   exti_initstr.EXTI_Line = EXTI_Line14;
   exti_initstr.EXTI_Mode = EXTI_Mode_Interrupt;//中断or事件
   exti_initstr.EXTI_Trigger = EXTI_Trigger_Falling;//选择触发方式为下降沿
   exti_initstr.EXTI_LineCmd = ENABLE;
   EXTI_Init(&exti_initstr);
   
    //5
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置抢占和响应优先级
    NVIC_InitTypeDef nvic_initstr;
    nvic_initstr.NVIC_IRQChannel = EXTI15_10_IRQn;
    nvic_initstr.NVIC_IRQChannelPreemptionPriority = 1;
    nvic_initstr.NVIC_IRQChannelSubPriority = 1;//NVIC_PriorityGroup_2,抢占和响应优先级均为0~3
    nvic_initstr.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_initstr);

}

/*在stm32中,中断函数的名字都是固定的，每个中断通道对应一个函数,无参无返回值
名字为：EXTI0_IRQHandler           ; EXTI Line 0
        EXTI1_IRQHandler           ; EXTI Line 1
        EXTI2_IRQHandler           ; EXTI Line 2
        EXTI3_IRQHandler           ; EXTI Line 3
        EXTI4_IRQHandler           ; EXTI Line 4
        EXTI15_10_IRQHandler       ; EXTI Line 15..10
        参考启动文件startup_stm32f10x_md.s文件
    中断编程建议：1.不要在中断函数调用delay等耗时的函数
                2.不要在中断函数里操作硬件，建议操作变量或标志位
*/
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line14) == SET)//对于中断通道10~15，应该在中断函数中调用EXTI_GetITStatus函数确定中断通道
    {
        count++;
        EXTI_ClearITPendingBit(EXTI_Line14);//清除中断标志位，否则程序会一直进中断
    }
}

uint32_t get_count(void)
{
    return count;
}