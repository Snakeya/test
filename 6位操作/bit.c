/*
 * @Author: your name
 * @Date: 2023-08-05 16:00:15
 * @LastEditTime: 2023-08-05 16:30:42
 * @LastEditors: LAPTOP-REOS7BFD
 * @Description: In User Settings Edit
 * @FilePath: \C_test\6位操作\bit.c
 */


/*
    某位置1：(逻辑或，有1即1)
        a |= (1 << bit);

    某位置0：(逻辑与，有0即0)
        a &= ~(1 << bit);

    某位取反：(异或)
        a ^= (1 << bit);
*/
#include <stdio.h>
int a;
int b = 15;
int c = 15;
int main(void)
{
    a |= (1 << 3);
    printf("%d\r\n",a);//8
    b &= ~(1 << 3);
    printf("%d\r\n",b);//7
    c ^= (1 << 3);
    printf("%d\r\n",c);//7
}