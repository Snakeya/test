/*
 * @Author: your name
 * @Date: 2023-07-26 16:24:23
 * @LastEditTime: 2023-07-26 19:38:37
 * @LastEditors: LAPTOP-REOS7BFD
 * @Description: In User Settings Edit
 * @FilePath: \STM32工程模板\hardware\key.h
 */
#ifndef __KEY_H__
#define __KEY_H__


#include "stm32f10x.h"
#include "Delay.h"
#include "beep.h"


typedef struct 
{
	
    uint8_t key1;
    uint8_t key2;
    uint8_t key3;
    uint8_t key4;

}Key_t;

void key_init(void);
void key_get_value(void);




#endif