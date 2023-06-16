#include "stm32f10x.h"
#include "Serial.h"
#define USE_EXTERNAL_CLOCK 1
/*
        1.配置RCC,打开外设时钟
        2.选择时基单元的时钟源 //stm32f10x_tim.h,1073~1081
        3.配置时基单元，包括PSC(预分频器),CNT（计数器）,ARR（自动重装器）//TIM_TimeBaseInit
        4.配置输出中断控制，允许更新中断输出到NVIC//TIM_ITConfig
        5.配置NVIC//NVIC_Init
        6.使能timer//TIM_Cmd
*/
void timer_init(void)
{
     //1
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//注意不同定时器挂载的总线区别
    #ifdef USE_EXTERNAL_CLOCK//使用外部时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0F);
    #else
    TIM_InternalClockConfig(TIM2);
    #endif
    //2
    //TIM_InternalClockConfig(TIM2);
    //3
    TIM_TimeBaseInitTypeDef timebase_str;
    timebase_str.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
    //ARR+PSC决定定时频率（时间），定时频率 = CK_CNT/(ARR+1) = CK_PSC/(PSC+1)/(ARR+1)
    if(USE_EXTERNAL_CLOCK)
    {
        timebase_str.TIM_Period = 10 - 1;//ARR，
        timebase_str.TIM_Prescaler = 1 - 1;//PSC
    } 
    else
    {
        timebase_str.TIM_Period = 10000 - 1;//ARR，
        timebase_str.TIM_Prescaler = 7200 - 1;//PSC，对72M进行7200分频，得到10k计数频率
    }
    
    timebase_str.TIM_ClockDivision = TIM_CKD_DIV1;//不管
    timebase_str.TIM_RepetitionCounter = 0;//高级定时器的参数，不需要给0
    TIM_TimeBaseInit(TIM2,&timebase_str);
    //避免初始化完成就进中断
    TIM_ClearFlag(TIM2,TIM_IT_Update);
    //4
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    //5
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    //6
    TIM_Cmd(TIM2,ENABLE);
}

uint16_t aa = 0;
/*
    TIM1_BRK_IRQHandler        ; TIM1 Break
    TIM1_UP_IRQHandler         ; TIM1 Update
    TIM1_TRG_COM_IRQHandler    ; TIM1 Trigger and Commutation
    TIM1_CC_IRQHandler         ; TIM1 Capture Compare
    TIM2_IRQHandler            ; TIM2
    TIM3_IRQHandler            ; TIM3
    TIM4_IRQHandler            ; TIM4
*/
void TIM2_IRQHandler(void)
{

    if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)//检查中断标志位
    {
        aa ++;
        printf("timer %d s\r\n",aa);
    
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    }
}