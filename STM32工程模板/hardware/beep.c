#include "stm32f10x.h"  
#include "Delay.h"


#define BEEP_PORT GPIOB
#define BEEP_PIN GPIO_Pin_13
#define BEEP_PORT_CLK RCC_APB2Periph_GPIOB


void beep_init(void)
{
    
    RCC_APB2PeriphClockCmd(BEEP_PORT_CLK, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
 	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
    
    GPIO_WriteBit(BEEP_PORT,BEEP_PIN,(BitAction)1);
    
}

void beep_on(uint32_t time)
{
    GPIO_WriteBit(BEEP_PORT,BEEP_PIN,(BitAction)0);
    Delay_ms(time);
    GPIO_WriteBit(BEEP_PORT,BEEP_PIN,(BitAction)1);
    Delay_ms(time);
}

