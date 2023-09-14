// Lab Xv6 and Unix utilities
// find.c

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

void find(char *dir, char *file)
{
    char buf[512], *p;
    int fd;
    struct dirent de; // 用于表示目录项（directory entry）的结构体。目录项包含有关目录中文件或子目录的信息，如文件名、文件类型和文件的inode号等
    struct stat st;   // 用于获取文件状态信息的结构体。它包含有关文件的详细信息，如文件大小、所有者、权限、修改时间等。

    // open() 函数打开路径，返回一个文件描述符，如果错误返回 -1
    // 表示目录字符串
    // 0: 这是一个标志参数，通常用于指定打开文件的方式和权限。在这里，0 表示不指定任何特殊的标志，即只是尝试打开文件或目录。
    if ((fd = open(dir, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", dir);
        return;
    }

    // fstat(fd, &st) 是一个用于获取文件状态信息的系统调用，它可以用于获取一个已经打开的文件的状态。返回0表示成功，-1表示失败
    // &st : 这是一个指向 struct stat 结构体的指针，用于存储获取到的文件状态信息。fstat 函数会将文件的状态信息填充到这个结构体中。
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", dir);
        close(fd);
        return;
    }

    // 如果不是目录类型
    if (st.type != T_DIR)
    {
        fprintf(2, "find: %s is not a directory\n", dir);
        close(fd);
        return;
    }

    // 如果路径过长放不入缓冲区，则报错提示
    // DIRSIZ 是一个用于表示目录项（directory entry）中文件名的最大长度的常量
    // 第一个 + 1 是用来考虑路径分隔符（ /）。
    // 第二个 + 1 是为了考虑字符串终止符（\0）。
    if (strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        fprintf(2, "find: directory too long\n");
        close(fd);
        return;
    }
    strcpy(buf, dir);
    // 将指针 p 移动到了字符串的末尾。这样，p 指向了目录路径的结尾位置。
    p = buf + strlen(buf);
    *p++ = '/';
    // 读取 fd ，如果 read 返回字节数与 de 长度相等则循环
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
            continue;
        // strcmp(s, t);
        // 根据 s 指向的字符串小于（s<t）、等于（s==t）或大于（s>t） t 指向的字符串的不同情况
        // 分别返回负整数、0或正整数
        // 不要递归 "." 和 "..."
        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;
        // memmove，把 de.name 信息复制 p，其中 de.name 代表文件名
        memmove(p, de.name, DIRSIZ);
        // 设置文件名结束符
        p[DIRSIZ] = 0;
        // int stat(char *, struct stat *);
        // stat 系统调用，可以获得一个已存在文件的模式，并将此模式赋值给它的副本
        // stat 以文件名作为参数，返回文件的 i 结点中的所有信息
        // 如果出错，则返回 -1
        if (stat(buf, &st) < 0)
        {
            // 出错则报错
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }
        // 如果是目录类型，递归查找
        if (st.type == T_DIR)
        {
            find(buf, file);
        }
        // 如果是文件类型 并且 名称与要查找的文件名相同
        else if (st.type == T_FILE && !strcmp(de.name, file))
        {
            // 打印缓冲区存放的路径
            printf("%s\n", buf);
        }
    }
}

int main(int argc, char *argv[])
{
    // 如果参数个数不为 3 则报错
    if (argc != 3)
    {
        // 输出提示
        fprintf(2, "usage: find dirName fileName\n");
        // 异常退出
        exit(1);
    }
    // 调用 find 函数查找指定目录下的文件
    find(argv[1], argv[2]);
    // 正常退出
    exit(0);
}
