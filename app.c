#include <stdio.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>
#include <unistd.h> /* alarm, pause */
#include <sys/types.h>

int main() {
    int i;
    for(i=0;i<80000000;i++);
    long long a, b;
    a = __rdtsc();
    for(i=0;i<50;i++)
    syscall(401);
    b = __rdtsc();
    printf("%lld \n", (b - a)/50);
    return 0;
}
