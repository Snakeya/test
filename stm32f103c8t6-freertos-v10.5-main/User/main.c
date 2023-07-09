/*
 * @Author: your name
 * @Date: 2023-07-08 21:40:20
 * @LastEditTime: 2023-07-09 14:05:03
 * @LastEditors: LAPTOP-REOS7BFD
 * @Description: In User Settings Edit
 * @FilePath: \stm32f103c8t6-freertos-v10.5-main\User\main.c
 */
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include "Serial.h"

static TaskHandle_t led_task_handle0 = NULL;
static TaskHandle_t serial_task_handle1 = NULL;


void led_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void led_task0(void *arg)
{
	while(1)                            
	{
/* 		GPIO_SetBits(GPIOA, GPIO_Pin_0);//高
		vTaskDelay(500/portTICK_PERIOD_MS);
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);//低
		vTaskDelay(500/portTICK_PERIOD_MS);
				GPIO_SetBits(GPIOA, GPIO_Pin_2);
		vTaskDelay(1000/portTICK_PERIOD_MS);
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		vTaskDelay(1000/portTICK_PERIOD_MS);
				GPIO_SetBits(GPIOA, GPIO_Pin_4);
		vTaskDelay(2000/portTICK_PERIOD_MS);
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		vTaskDelay(2000/portTICK_PERIOD_MS); */
         printf("led_task0\r\n");
	}
}


void serial_task1(void *arg)
{
    while(1)
    {
        printf("hello world\r\n");
    }
}
int main(void) 
{
	led_init();
    Serial_Init();
	xTaskCreate(led_task0, "led_task0", 1024, NULL, 20, &led_task_handle0);
    xTaskCreate(serial_task1, "serial_task1", 1024, NULL, 20, &serial_task_handle1);
	vTaskStartScheduler();
	while(1);
}
