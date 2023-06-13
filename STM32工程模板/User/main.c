#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#include "Serial.h"

int main(void)
{
	Serial_Init();
	printf("hello,world\r\n");
	
	while (1)
	{
		
	}
}
