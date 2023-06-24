#include "stm32f10x.h"
#include "timer.h"
#include "Delay.h"
/*
    初始化输出比较单元，四个输出比较通道，不同的通道对应的GPIO口不同，查看引脚定义表
    void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
    void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
    void TIM_OC3Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
    void TIM_OC4Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
    更改占空比需要的函数
    void TIM_SetCompare1(TIM_TypeDef* TIMx, uint16_t Compare1);
    void TIM_SetCompare2(TIM_TypeDef* TIMx, uint16_t Compare2);
    void TIM_SetCompare3(TIM_TypeDef* TIMx, uint16_t Compare3);
    void TIM_SetCompare4(TIM_TypeDef* TIMx, uint16_t Compare4);
*/

/*
    1.开启GPIO和TIM时钟
    2.配置时基单元
    3.配置输出比较单元
    4.配置GPIO,初始化为复用推挽输出
    5.运行控制
*/

/*
    PWM频率 = 72M/(PSC + 1)/(ARR + 1)
    占空比 = CCR/(ARR + 1)
    分辨率 = 1/(ARR + 1)
*/

/*
    @brief:将pa0重映射到pa15
    参考：STM32F10xxx参考手册（中文）.pdf,8.3.7 定时器复用功能重映射

*/
static void gpio_remap(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//开启APB2外设时钟
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);//将pa0重映射到pa15
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//pa15默认为jtag引脚，需要remap一下，remap后p13,pb3,pb4可以当做GPIO使用
}

void pwminit(void)
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
    #ifdef USE_EXTERNAL_CLOCK//使用外部时钟
        timebase_str.TIM_Period = 10 - 1;//ARR，
        timebase_str.TIM_Prescaler = 1 - 1;//PSC
    #else
        timebase_str.TIM_Period = 100 - 1;//ARR，控制PWM分辨率，100
        timebase_str.TIM_Prescaler = 720 - 1;//PSC，对72M进行7200分频，得到10k计数频率
    #endif
    
    timebase_str.TIM_ClockDivision = TIM_CKD_DIV1;//不管
    timebase_str.TIM_RepetitionCounter = 0;//高级定时器的参数，不需要给0
    TIM_TimeBaseInit(TIM2,&timebase_str);

    //初始化输出比较单元
     TIM_OCInitTypeDef TIM_OCInitStruct;
     TIM_OCStructInit(&TIM_OCInitStruct);//给结构体一个初始值，在根据需要修改结构体的值，可以避免结构体值未初始化带来的问题
     //tips:高级定时器和普通定时器的TIM_OCInitStruct成员变量不同
     TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
     TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
     TIM_OCInitStruct.TIM_Pulse = 0;//CCR;
     TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2,&TIM_OCInitStruct); 

    //
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
   #ifdef USE_REMAP
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   #else
   gpio_remap();
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
   #endif
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA,&GPIO_InitStructure);


    TIM_Cmd(TIM2,ENABLE);
    
	

}

void pwm_led(void)
{
    uint16_t i = 0;
	while (1)
	{
		//printf("ARR is %d\r\n",TIM_GetCounter(TIM2));//打印自动重装值，0~9999
		for(i = 0; i < 100;i++)
		{
			TIM_SetCompare1(TIM2,i);//设置CCR的值，用于控制占空比
            Delay_ms(10);
			printf("CCR is %d \r\n",i);
		}
		for(i = 0;i < 100; i++)
		{
			TIM_SetCompare1(TIM2,100 - i);
            Delay_ms(10);
		}
	}
}

/*
    舵机：根据PWM信号占空比控制输出角度
    PWM信号要求：周期20ms，对应PWM周期50hz
    ,高电平宽度：0.5ms~2.5ms
*/
void pwm_servo_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);//PA1对应TIM2的通道2
    /*
        tips：对于同一个定时器的，所以频率是一样的，占空比由各自的CCR决定，可以自己设定
    */
	
	TIM_Cmd(TIM2, ENABLE);
}




void Servo_SetAngle(float Angle)
{
    TIM_SetCompare2(TIM2, Angle / 180 * 2000 + 500);//PWM_SetCompare2通道2 
}


void pwm_servo_test(void)
{
    pwm_servo_init();

    float angle = 0;
    while(1)
    {
        for(angle = 0;angle <= 180 ;angle += 10)
        {
            Servo_SetAngle(angle);
            Delay_s(2);
        }
        angle = 0;

    }
}



void input_capture_init(void)
{
    //开启gpio和tim时钟
    //配置为输入模式
    //配置timebase
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//input up上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//tim3的1通道对应引脚为pa6
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR，最大计数值65536
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    //配置输入捕获单元
    TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//配置定时器输入捕获通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//配置滤波器，数字越大，滤波效果越好
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//边沿检测极性选择
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//配置定时器输入捕获分频器，@TIM_ICPSC_DIV1不分频
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    //选择从模式触发源
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
    //选择触发后执行的操作
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
    //tim_cmd开启定时器
    TIM_Cmd(TIM3, ENABLE);
}

uint32_t input_capture_get_freq(void)
{
    return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}


/*
    输入捕获实验：使用一个io口输出pwm，另一个io口测量输出频率
*/
void input_capture_test1(void)
{
    pwminit();//pa15输出pwm
    input_capture_init();

    uint32_t freq = 0;
    freq = input_capture_get_freq();

    while(1)
    {
        printf("freq is %d\r\n",freq);
    }
    
} 
    