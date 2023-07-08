#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "countsensor.h"
#include "encoder.h"
//#include "Serial.h"
#include "timer.h"
#include "pwm.h"
#include "ad.h"
#include "usart.h"
#include "OLED.h"
#include "mpu6050.h"
#include "screen.h"//面向对象test

uint8_t id;
int16_t AX, AY, AZ, GX, GY, GZ;
int16_t num;
extern int16_t g_encoder_count;
screen g_src={
        128,
        64,
        0,
        OLED_Init,
        OLED_Clear,
        OLED_ShowChar,
        OLED_ShowString,
        OLED_ShowNum,
        OLED_ShowSignedNum,
        OLED_ShowHexNum,
        OLED_ShowBinNum
    };

int main(void)
{
	
	encoder_init();
	usart1_init();
	g_src.init();
	g_src.clear();
	
	printf("hello world\r\n");
	while(1)
	{
		num += get_encoder_count();
		g_src.showsignednum(2,2,num,3);
		//printf("%d\r\n",get_encoder_count());
	}


	//Serial_Init();
	//countsensor_init();
	//encoder_init();
	//timer_init();
	//pwminit();
	//ad_init();
	// OLED_Init();
	// mpu6050_init();
	// // usart1_init();
	// // usart2_init(57600);
	// id = mpu6050_get_id();
	// OLED_ShowHexNum(1, 4, id, 2);

	
/* 	uint8_t arr[4] = {0};
	arr[1] = 0xaa;
	
	OLED_ShowString(1, 1, "TxPacket");
	printf("hello\r\n");
	usart2_send_bytes(0x55);
	usart1_SendArray(arr,4);
	OLED_ShowHexNum(2,1) */
	// while(1)
	// {
	// 	MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
	// 	OLED_ShowSignedNum(2, 1, AX, 5);
	// 	OLED_ShowSignedNum(3, 1, AY, 5);
	// 	OLED_ShowSignedNum(4, 1, AZ, 5);
	// 	OLED_ShowSignedNum(2, 8, GX, 5);
	// 	OLED_ShowSignedNum(3, 8, GY, 5);
	// 	OLED_ShowSignedNum(4, 8, GZ, 5);
	// }
	

	//pwm_led();
	//pwm_servo_test();
	//input_capture_test1();
	//ad_test_1();
	//oo_test();

	

}
