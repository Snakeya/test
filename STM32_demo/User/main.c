#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "countsensor.h"
#include "encoder.h"
#include "Serial.h"
#include "timer.h"

int main(void)
{
	Serial_Init();
	//countsensor_init();
	//encoder_init();
	timer_init();
	
	printf("hello\r\n");

	while (1)
	{
		printf("ARR is %d\r\n",TIM_GetCounter(TIM2));//打印自动重装值，0~9999
	}
}
