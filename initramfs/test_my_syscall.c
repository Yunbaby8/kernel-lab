#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define __NR_my_syscall 548

int main(void)
{
    long ret;

    ret = syscall(__NR_my_syscall);

    printf("my_syscall return = %ld\n", ret);

    return 0;
}
