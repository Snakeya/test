/*
 * @Author: your name
 * @Date: 2023-07-25 13:51:37
 * @LastEditTime: 2023-07-25 19:37:12
 * @LastEditors: LAPTOP-REOS7BFD
 * @Description: In User Settings Edit
 * @FilePath: \STM32工程模板\User\main.c
 */
#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#include "IIC_OLED.h"
#include "Serial.h"
#include "mpu6050.h"
#include "beep.h"
#include "u8g2.h"

mpu6050_data mpu_data;
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
void draw(u8g2_t *u8g2)
{
	u8g2_ClearBuffer(u8g2); 
    u8g2_SetFontMode(u8g2, 1); 
    u8g2_SetFontDirection(u8g2, 0); 
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 0, 20, "U");
    
    u8g2_SetFontDirection(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
    u8g2_DrawStr(u8g2, 21,8,"8");
        
    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 51,30,"g");
    u8g2_DrawStr(u8g2, 67,30,"\xb2");
    
    u8g2_DrawHLine(u8g2, 2, 35, 47);
    u8g2_DrawHLine(u8g2, 3, 36, 47);
    u8g2_DrawVLine(u8g2, 45, 32, 12);
    u8g2_DrawVLine(u8g2, 46, 33, 12);
  
    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
    u8g2_DrawStr(u8g2, 1,54,"github.com/olikraus/u8g2");
//	u8g2_ClearDisplay(u8g2);
	u8g2_SendBuffer(u8g2);
	//Delayxms(1000);
}

int main(void)
{
	Serial_Init();
    //OLED_Init();
    //OLED_I2C_Init();
	printf("hello,world\r\n");
	//OLED_ShowString(1,1,"hello");
    mpu6050_init();
    beep_init();
    OLED12864_IoInit();
    
    u8g2_t u8g2;
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8g2_gpio_and_delay_stm32);
    u8g2_InitDisplay(&u8g2); 
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_ClearBuffer(&u8g2);
    Delay_ms(1000);
    u8g2_DrawLine(&u8g2, 0,0, 127, 63);
	u8g2_DrawLine(&u8g2, 127,0, 0, 63);
    u8g2_SendBuffer(&u8g2);
    draw(&u8g2);

    
	while (1)
	{
        
        MPU6050_GetData(&mpu_data);
/*         OLED_ShowSignedNum(2, 1, mpu_data.AccX, 5);
        OLED_ShowSignedNum(3, 1, mpu_data.AccY, 5);
        OLED_ShowSignedNum(4, 1, mpu_data.AccZ, 5);
        OLED_ShowSignedNum(2, 8, mpu_data.GyroX, 5);
        OLED_ShowSignedNum(3, 8, mpu_data.GyroY, 5);
        OLED_ShowSignedNum(4, 8, mpu_data.GyroZ, 5); */
        if(mpu_data.AccX > 100 || mpu_data.AccX < 0)
        {
            //OLED_Close();
           // beep_on(100);
        }
        else
        {
            //OLED_Open();
        }
		
	}
}
