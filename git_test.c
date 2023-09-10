/*
 * @Author: your name
 * @Date: 2023-09-10 22:39:12
 * @LastEditTime: 2023-09-10 22:43:27
 * @LastEditors: LAPTOP-REOS7BFD
 * @Description: In User Settings Edit
 * @FilePath: \undefinedf:\aaprj\stm32\git_test.c
 */

#include "stm32f10x.h"                  // Device header

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	while (1)
	{
		//sourcetree上的第一次修改
		//GITHUB上的第一次修改
		//ST第三次
		//ST第四次
		
	}
	//soucetree第二次修改
	//github第二次修改
}
