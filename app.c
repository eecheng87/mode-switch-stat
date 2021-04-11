#include <stdio.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/types.h>
#if defined(__x86_64__)
#include <x86intrin.h>
#endif

int main() {
    int i;
    /* warm-up */
    for(i = 0; i < 80000000; i++);
    long long a, b;
#if defined(__x86_64__)
    a = __rdtsc();
#elif defined(__aarch64__)
    asm volatile("mrs %0, pmccntr_el0" : "=r"(a));
#endif
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
    syscall(401);
#if defined(__x86_64__)
    b = __rdtsc();
#elif defined(__aarch64__)
    asm volatile("mrs %0, pmccntr_el0" : "=r"(b));
#endif
    printf("%lld \n", (b - a) / 120);
    return 0;
}
