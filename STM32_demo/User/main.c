#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "countsensor.h"
#include "encoder.h"
#include "Serial.h"
#include "timer.h"
#include "pwm.h"

int main(void)
{
	Serial_Init();
	//countsensor_init();
	//encoder_init();
	//timer_init();
	//pwminit();
	
	
	printf("hello\r\n");
	//pwm_led();
	pwm_servo_test();
	

}
