/*
 * @Author: your name
 * @Date: 2023-08-03 15:28:21
 * @LastEditTime: 2023-08-03 15:49:43
 * @LastEditors: LAPTOP-REOS7BFD
 * @Description: In User Settings Edit
 * @FilePath: \undefinedf:\aaprj\C_test\0大小端\test.c
 */


/*
32bit宽的数0x12345678在Little-endian模式CPU内存中的存放方式（假设从地址0x4000开始存放）为：
内存地址	0x4000	0x4001	0x4002	0x4003
存放内容	0x78	0x56	0x34	0x12
而在Big-endian模式CPU内存中的存放方式则为：
内存地址	0x4000	0x4001	0x4002	0x4003
存放内容	0x12	0x34	0x56	0x78
*/
#include <stdio.h>
//联合体union的存放顺序是所有成员都从低地址开始存放
int checkCPU(void)
{ 
    union w 
    {   
        int  a; 
        char b; 
    } c; 
    c.a = 1; 
    if(c.b == 1)
    {
        printf("小端\r\n");
    }
    else
    {
        printf("大端\r\n");
    }

} 

int main(void)
{
    checkCPU();
    return 0;
}