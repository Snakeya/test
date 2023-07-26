/*
 * @Author: your name
 * @Date: 2023-07-26 16:24:16
 * @LastEditTime: 2023-07-26 19:39:31
 * @LastEditors: LAPTOP-REOS7BFD
 * @Description: In User Settings Edit
 * @FilePath: \STM32工程模板\hardware\key.c
 */
#include  "key.h"


Key_t key;

void key_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//下拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 |GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
}


void key_get_value(void)
{
   key.key1 = 0;
   key.key2 = 0;
   key.key3 = 0;
   key.key4 = 0;
    
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
	{
		Delay_ms(20);
		beep_on(10);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0);
		Delay_ms(20);
		key.key1 = 1;
	}
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
	{
		Delay_ms(20);
		beep_on(10);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
		Delay_ms(20);
		key.key2 = 1;
	}
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 1)
	{
		Delay_ms(20);
		beep_on(10);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 1);
		Delay_ms(20);
		key.key3 = 1;
	}
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 1)
	{
		Delay_ms(20);
		beep_on(10);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 1);
		Delay_ms(20);
		key.key4 = 1;
	}

}

