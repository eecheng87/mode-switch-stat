# msTimer
To mitigate meltdown and spectre vulnerability, the mode switch cost was increased. msTimer is a easy infrastructure to measure the cost of mode switch. In here, one mode switch trip contains user mode to kernel mode and kernel to user.

## How to build & testing
In current status, code only available for x86_64 and also need package: numpy

Build project:
```
$ git clone https://github.com/eecheng87/msTimer.git
$ cd msTimer
$ sudo make
```
Testing:
```
$ sudo make testing
```
sample output:
```
Model name: Intel(R) Core(TM) i5-7200U CPU @ 2.50GHz
Vulnerability Meltdown: Mitigation; PTI

Avg of mode switch takes 1921.45 cycles and standard deviation is 106.92
```

## Common error
1. ERROR: could not insert module msT.ko: Operation not permitted

    Solution: Disable SecureBoot (check by `mokutil --sb-state`)

2. If kernel version lower than `5.7.0`, please disable KALSR or modify macro `KERNEL_VERSION` to version lower than yours. It's also fine to comment it and force include `systab.h` and calculating `syscall_table`  by adding offset to original system call table.


## How to contribute
Add your output as comment in issue#1