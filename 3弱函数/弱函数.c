/*
 * @Author: your name
 * @Date: 2023-08-03 16:32:58
 * @LastEditTime: 2023-08-03 16:54:17
 * @LastEditors: LAPTOP-REOS7BFD
 * @Description: In User Settings Edit
 * @FilePath: \C_test\3弱函数\弱函数.c
 */
/*在C语言中，弱函数（Weak Function）是一种函数定义修饰符，指示编译器在链接时对函数进行弱符号处理。
弱函数的特点是允许在链接阶段出现多个同名函数的定义，但只有一个最强的函数定义被保留在最终的二进制文件中。

在C语言中，使用关键字 __attribute__((weak)) 可以将函数定义标记为弱函数。例如：

    
c
插入代码
复制代码
void weak_function() __attribute__((weak));
void weak_function() {
    // 函数定义
}

    
上面的代码将 weak_function 函数声明为一个弱函数。

在程序的链接阶段，如果存在多个同名的弱函数定义，那么编译器会选择一个被认为最强的函数定义，
并将其保留在最终的可执行文件中。
如果没有其他强符号与之匹配，那么弱函数的定义将被保留在可执行文件中。

弱函数可以在一些场景中有用，例如插件系统中的函数注册、覆盖库函数等。
通过定义弱函数，可以在应用程序中重新定义某个函数，
从而实现对原函数的覆盖或替换。

需要注意的是，在不同的编译器和链接器中，弱函数的处理方式可能存在差异，
因此在使用弱函数时要留意相关工具链的具体规范与限制。
*/

#include <stdio.h>
#include  "test.h"
// 声明弱函数 foo
//test.c中重新定义了foo,没有出现报错，弱在本文件中再定义，则会报错
void foo() __attribute__((weak));
void foo() {
    printf("我是弱函数 foo\n");
}





int main() {
    // 如果在链接时没有强函数定义了 foo，那么将使用弱函数的定义
    foo();
    return 0;
}