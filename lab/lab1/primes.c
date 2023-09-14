// Lab Xv6 and Unix utilities
// primes.c

#include "kernel/types.h"
#include "user/user.h"
#include "stddef.h"

// 文件描述符重定向,防止递归导致文件描述符过多
void mapping(int pd[], int n)
{
    // 将描述符n关闭，然后指向pd指向的描述符，之后关闭pd描述符，n就完成了重定向
    close(n);
    // dup(pd[n]);创建一个新的文件描述符，这个描述符是当前进程中可用的最低的未使用的文件描述符。
    // 将这个新的描述符与 pd[n] 所指向的文件表项关联起来。这意味着新的描述符和 pd[n] 现在都指向同一个文件或管道。
    // 返回新创建的文件描述符，这个描述符可以被用来读取或写入与 pd[n] 相关联的文件或管道。
    dup(pd[n]);
    close(pd[0]);
    close(pd[1]);
}

void primes()
{
    // 因为管道都进行了重定向，所以这里仍然能找到之前管道的数据
    int previous, next;
    int fd[2];
    if (read(0, &previous, sizeof(int)))
    {
        // 第一个一定是素数，直接打印
        printf("prime %d\n", previous);
        pipe(fd);
        if (fork() == 0)
        {
            // 管道的写端口重定向到描述符 1 上
            mapping(fd, 1);
            // 循环读取管道中的数据
            while (read(0, &next, sizeof(int)))
            {
                // 如果该数不是管道中第一个数的倍数
                if (next % previous != 0)
                {
                    write(1, &next, sizeof(int));
                }
            }
        }
        else
        {
            wait(NULL); // 暂停执行，等待任何一个子进程终止。父进程可以通过返回值来获取子进程的退出状态信息。
            // 如果子进程正常终止，wait 返回子进程的进程号（PID），并将子进程的退出状态保存在一个参数中
            // 父进程将管道的读端口映射到描述符 0 上
            // 如果父进程没有任何子进程或者没有子进程终止，wait 函数可能会一直阻塞等待，直到有子进程终止为止。如果父进程不希望阻塞等待子进程的终止，可以使用 waitpid 函数，并传递适当的选项来控制等待的行为。
            mapping(fd, 0);
            // 递归执行此过程
            primes();
        }
    }
}

int main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    if (fork() == 0)
    {
        mapping(fd, 1);
        for (int i = 2; i < 36; i++)
        {
            write(1, &i, sizeof(int));
        }
    }
    else
    {
        wait(NULL);
        mapping(fd, 0);
        primes();
    }
    exit(0);
}
