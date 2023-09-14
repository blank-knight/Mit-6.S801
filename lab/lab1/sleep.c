#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    { // 判断输入参数是否为2
        // int fprintf(FILE *stream, const char *format, ...) 发送格式化输出到流 stream 中。
        fprintf(2, "usage: sleep [ticks num]\n");
        exit(1);
    }
    // atoi sys call guarantees return an integer
    int ticks = atoi(argv[1]);
    // sleep(ticks)返回一个正值，表示还剩余多少时钟滴答数未休眠完成。这样的返回值可以用于确定休眠被提前唤醒了多少滴答数。
    // 如果发生了错误，sleep(ticks) 可能返回 - 1，表示休眠操作失败。
    int ret = sleep(ticks);
    // exit和return区别
    //   1. exit用于结束正在运行的整个程序，它将参数返回给OS，把控制权交给操作系统；而return 是退出当前函数，返回函数值，把控制权交给调用函数。
    //   2. exit是系统调用级别，它表示一个进程的结束；而return 是语言级别的，它表示调用堆栈的返回。
    //   3. 在main函数结束时，会隐式地调用exit函数，所以一般程序执行到main() 结尾时，则结束主进程。exit将删除进程使用的内存空间，同时把错误信息返回给父进程。

    // exit函数是退出应用程序, 并将应用程序的一个状态返回给OS，这个状态标识了应用程序的一些运行信息。
    // main函数结束时也会隐式地调用exit函数，exit函数运行时首先会执行由atexit() 函数登记的函数，然后会做一些自身的清理工作，同时刷新所有输出流、关闭所有打开的流并且关闭通过标准I/O函数tmpfile() 创建的临时文件。
    exit(ret);
}