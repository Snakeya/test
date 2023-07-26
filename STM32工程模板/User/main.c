/*
 * @Author: your name
 * @Date: 2023-07-25 13:51:37
 * @LastEditTime: 2023-07-26 20:16:59
 * @LastEditors: LAPTOP-REOS7BFD
 * @Description: In User Settings Edit
 * @FilePath: \STM32工程模板\User\main.c
 */
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "stopwatch.h"
#include "IIC_OLED.h"
#include "Serial.h"
#include "mpu6050.h"
#include "beep.h"
#include "u8g2.h"
#include "watchface.h"
#include "key.h"


uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	switch(msg)
		{
			case U8X8_MSG_DELAY_MILLI://Function which implements a delay, arg_int contains the amount of ms
				Delay_ms(arg_int);
			break;
		
			case U8X8_MSG_DELAY_10MICRO://Function which delays 10us
				Delay_us(10);
			break;
		
			case U8X8_MSG_DELAY_100NANO://Function which delays 100ns
				__NOP();
			break;
			case U8X8_MSG_GPIO_I2C_CLOCK:
				if (arg_int) IIC_OLED_SCL_HIGH();
				else IIC_OLED_SCL_LOW();
			break;
			case U8X8_MSG_GPIO_I2C_DATA:
				if (arg_int) IIC_OLED_SDA_HIGH();
				else IIC_OLED_SDA_LOW();
			break;
			default:
				return 0; //A message was received which is not implemented, return 0 to indicate an error
	}
	return 1; // command processed successfully.
}

u8g2_t u8g2;
mpu6050_data mpu_data;
extern Key_t key;
extern stopwatch_t stopwatch1;
int main(void)
{
	Serial_Init();
    stopwatch_init();
    //OLED_Init();
    //OLED_I2C_Init();
	printf("hello,world\r\n");
	//OLED_ShowString(1,1,"hello");
    mpu6050_init();
    beep_init();
    key_init();
    RTC_Init();	
    RTC_Set(2023,7,30,1,1,1);  //设置时间
    OLED12864_IoInit();
    
    
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8g2_gpio_and_delay_stm32);
    u8g2_InitDisplay(&u8g2); 
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_ClearBuffer(&u8g2);
    u8g2_SendBuffer(&u8g2);

   
    
    //watchface_ShowTime(&u8g2);
    
    //stopwatch_show(&u8g2);
    
//    u8g2_ClearBuffer(&u8g2);
//    u8g2_SendBuffer(&u8g2);
	while (1)
	{
        key_get_value();
        watchface_ShowTime(&u8g2);
        if(key.key1 == 1 || key.key2 == 1 || key.key3 == 1 || key.key4 == 1)
        {
           
            while(1)
            {
                key_get_value();
                //printf("%d\r\n", key.key2);
                printf("key2 = %d,state = %d\r\n",key.key2,stopwatch1.state );
                if(key.key2 == 1 && (stopwatch1.state == SUSPEND))
                {
                    stopwatch_start();
                    key.key2 = 0;
                }
                if(key.key2 == 1 && (stopwatch1.state == RUNING))
                {
                    stopwatch_suspend();
                    key.key2 = 0;
                }
                if(key.key1 == 1)
                {
                    stopwatch_close();
                    key.key1  = 0;
                    break;
                }
                stopwatch_show(&u8g2);
            }
            
        }
        MPU6050_GetData(&mpu_data);
        if(mpu_data.AccX > 100 || mpu_data.AccX < 0)
        {
            //OLED_Close();
           // beep_on(100);
           u8g2_SetPowerSave(&u8g2, 1);
        }
        else
        {
            u8g2_SetPowerSave(&u8g2,0);
        }
		
	}
}
