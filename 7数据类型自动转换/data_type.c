/*
 * @Author: your name
 * @Date: 2023-08-05 16:50:17
 * @LastEditTime: 2023-08-05 16:53:21
 * @LastEditors: LAPTOP-REOS7BFD
 * @Description: In User Settings Edit
 * @FilePath: \C_test\7数据类型自动转换\data_type.c
 */
#include <string.h>
#include <stdio.h>

/*
    原因是当表达式中存在有符号类型和无符号类型时所有的操作数都自动转换为无符号类型。
    因此-20变成了一个非常大的正整数，所以该表达式计算出的结果大于6。
*/
void foo(void)
{
    unsigned int a = 6;
    int b = -20;
    (a+b > 6) ? puts("> 6") : puts("<= 6");
    //添加一个冲突，保留
}

int main(void)
{
    foo();
    
    return 0;
}