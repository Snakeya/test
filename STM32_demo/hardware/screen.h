/*
 * @Author: your name
 * @Date: 2023-07-07 16:06:49
 * @LastEditTime: 2023-07-08 15:23:08
 * @LastEditors: LAPTOP-REOS7BFD
 * @Description: In User Settings Edit
 * @FilePath: \STM32_demo\hardware\screen.h
 */
#ifndef __SCREEN_H
#define __SCREEN_H

typedef struct screen
{
    unsigned int width;
    unsigned int height;
    unsigned int bpp;
    void(*init)(void);
    void(*clear)(void);
    void(*showchar)(uint8_t Line, uint8_t Column, char Char);
    void(*showstring)(uint8_t Line, uint8_t Column, char *String);
    void(*shownum)(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
    void(*showsignednum)(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
    void(*showhexnum)(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
    void(*showbinnum)(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
    

}screen,*p_screen;

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void oo_test();

#endif