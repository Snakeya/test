#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "countsensor.h"
#include "Serial.h"

int main(void)
{
	Serial_Init();
	countsensor_init();
	printf("hello\r\n");

	while (1)
	{
		printf("%d\r\n",get_count());
        Delay_s(1);
        
	}
}
