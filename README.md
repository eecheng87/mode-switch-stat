# msTimer
msTimer is a infrastructure to measure the cost of mode switch (transition). In here, one mode switch trip contains user mode to kernel mode and kernel to user.

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
Intel(R) Core(TM) i5-7200U CPU @ 2.50GHz
Avg of mode switch takes 1608.11 cycles and standard deviation is 145.51
```

## How to contribute
Add your output as comment in issue#1