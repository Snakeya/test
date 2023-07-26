/*
 * @Author: your name
 * @Date: 2023-07-26 10:50:38
 * @LastEditTime: 2023-07-26 15:54:25
 * @LastEditors: LAPTOP-REOS7BFD
 * @Description: In User Settings Edit
 * @FilePath: \STM32工程模板\APP\stopwatch.h
 */
#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__

#include "stm32f10x.h"
#include "Serial.h"
#include "u8g2.h"
#include "Delay.h"

#define SUSPEND 0
#define RUNING 1

typedef struct 
{
	
	uint8_t min;
	uint8_t sec;
    uint8_t	sec_100;
    uint8_t	state;
			 
}stopwatch_t;	

void stopwatch_init(void);
void stopwatch_start(void);
void stopwatch_suspend(void);


void stopwatch_show(u8g2_t * u8g2);
void stopwatch_draw_Button(u8g2_t *u8g2);



#endif