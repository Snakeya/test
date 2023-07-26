/*
 * @Author: your name
 * @Date: 2023-07-26 10:50:13
 * @LastEditTime: 2023-07-26 19:54:38
 * @LastEditors: LAPTOP-REOS7BFD
 * @Description: In User Settings Edit
 * @FilePath: \STM32工程模板\APP\stopwatch.c
 */
#include "stopwatch.h"

stopwatch_t stopwatch1;

void stopwatch_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    TIM_InternalClockConfig(TIM2);

    TIM_TimeBaseInitTypeDef timebase_str;
    timebase_str.TIM_CounterMode = TIM_CounterMode_Up;
    timebase_str.TIM_Period = 10000 - 1;//ARR，
    timebase_str.TIM_Prescaler = 7200 - 1;//PSC，
    timebase_str.TIM_ClockDivision = TIM_CKD_DIV1;//不管
    timebase_str.TIM_RepetitionCounter = 0;//高级定时器的参数，不需要给0
    TIM_TimeBaseInit(TIM2,&timebase_str);
    //避免初始化完成就进中断
    TIM_ClearFlag(TIM2,TIM_IT_Update);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    //6
    TIM_Cmd(TIM2,DISABLE);
    stopwatch1.state = SUSPEND;
}



void stopwatch_start(void)
{
    TIM_Cmd(TIM2,ENABLE);
    stopwatch1.state = RUNING;
}

void stopwatch_suspend(void)
{
    TIM_Cmd(TIM2,DISABLE);
    stopwatch1.state = SUSPEND;
}

void stopwatch_close(void)
{
    TIM_Cmd(TIM2,DISABLE);
    stopwatch1.state = SUSPEND;
    stopwatch1.min = 0;
    stopwatch1.sec = 0;
    stopwatch1.sec_100 = 0;
    TIM_SetCounter(TIM2, 0);

}

void TIM2_IRQHandler(void)
{

    if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)//检查中断标志位
    {
        printf("TIM2_IRQHandler\r\n");
        stopwatch1.sec ++;
        if(stopwatch1.sec == 60)
        {
            stopwatch1.min ++;
            stopwatch1.sec = 0;
        }
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    }
}


void stopwatch_show(u8g2_t * u8g2)
{
    stopwatch1.sec_100 = TIM_GetCounter(TIM2)/100;
    char buff[64];
    u8g2_ClearBuffer(u8g2); 
    u8g2_SetFontMode(u8g2, 1); 
    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_t0_15b_mr);
    u8g2_DrawStr(u8g2,0,15,"<-");
    u8g2_DrawButtonUTF8(u8g2,27,15,U8G2_BTN_BW2, 0, 2, 2, "Stopwatch" );
    sprintf(buff,"%02d:%02d:%02d",stopwatch1.min,stopwatch1.sec,stopwatch1.sec_100);
    u8g2_DrawStr(u8g2,27,40,buff);

    stopwatch_draw_Button(u8g2);
    
    u8g2_SendBuffer(u8g2);
}

void stopwatch_draw_Button(u8g2_t *u8g2)
{
    switch(stopwatch1.state)
    {
        case(SUSPEND):
            u8g2_DrawTriangle(u8g2,110,5,110,15,115,10);
            break;

        case(RUNING):
            u8g2_DrawBox(u8g2, 110, 5, 3, 10);
            u8g2_DrawBox(u8g2, 115, 5, 3, 10);
            break;
        default:
            break;
    }
}



