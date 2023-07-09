/*
 * @Author: your name
 * @Date: 2023-07-08 21:40:20
 * @LastEditTime: 2023-07-09 21:57:22
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

static TaskHandle_t serial_task1_handle = NULL;
static TaskHandle_t serial_task2_handle = NULL;

//静态创建任务3传入的参数
StaticTask_t xtask3_tcb;
StackType_t xtask3_stack[100];//任务3的栈


StaticTask_t xidle_tcb;//空闲任务tcb
StackType_t xidle_stack[100];//空闲任务栈

/*
    静态创建任务，需实现此函数，查看vTaskStartScheduler定义
*/
void vApplicationGetIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
								StackType_t ** ppxIdleTaskStackBuffer,
								uint32_t * pulIdleTaskStaticSize)
{
	*ppxIdleTaskTCBBuffer = &xidle_tcb;
	*ppxIdleTaskStackBuffer = xidle_stack;
	*pulIdleTaskStaticSize = 100;
}

void serial_task1(void *arg)
{
    while(1)
    {
        printf("1\r\n");
    }
}

void serial_task2(void *arg)
{
    while(1)
    {
        printf("2\r\n");
    }
}

void serial_task3(void *arg)
{
	while(1)
	{
		printf("3\r\n");
	}
}


int main(void) 
{
    Serial_Init();
    printf("hello world\r\n");
	/*
		默认调度机制下：高优先级的任务优先执行，如果优先级没有主动放弃运行的话，低优先级的任务不会运行
	*/
	xTaskCreate(serial_task1,"task1",100,NULL,2,&serial_task1_handle);
	xTaskCreate(serial_task2,"task2",100,NULL,1,&serial_task2_handle);
	/*
		静态创建任务，需打开	configSUPPORT_STATIC_ALLOCATION
	*/
	xTaskCreateStatic(serial_task3,"task3", 100,NULL, 1,xtask3_stack,&xtask3_tcb);
                                    
                                    
                                   
                                    
                                    

	vTaskStartScheduler();
	while(1);
}
