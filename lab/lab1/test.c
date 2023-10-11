#include <stdio.h>
#include <unistd.h>

int main()
{
    char *argv[3];
    argv[0] = "echo";
    argv[1] = "hello";
    argv[2] = 0;
    exec("/bin/echo", argv);
    printf("exec error\n");

    return 0;
}
