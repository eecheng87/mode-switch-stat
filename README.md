# CPU mode switch statistics

The `mode-switch-stat` tool measures the cost of CPU mode switch, the round
trip between user and kernel mode. At present, this tool supports `x86_64` and `aarch64` (ARMv8) architectures, and it would be useful to understand the performance impact
to defense against Meltdown and Spectre vulnerability.

## Background: Meltdown and Spectre

In operating systems such as Linux,  the kernel code/data is protected from
direct access by user code with MMU access control mechanisms. Various attacks
have been discovered exploiting kernel code bugs, leading to privilege
escalation and system control. To carry out these attacks, the attacker would
need to know two things:
* what the vulnerability is; and
* what the address of related kernel code and/or data is.

The kernel page-table isolation (KPTI) implementation was finally committed to
the Linux kernel on [Dec 29, 2017](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=5aa90a84589282b87666f92b6c3c917c8080a9bf).
Changes brought by KPTI can have a significant impact on performance. Early
tests showed a 5% impact in most cases, with worst-case tests indicating a 50%
performance hit.

Meltdown ([CVE-2017-5754](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2017-5754))
allows an unprivileged user to access the complete kernel (and physical) memory
of a computer. This attack is relatively simple to execute; to carry it out,
attackers need to run their own program on the target system. This attack is
particularly damaging to shared systems (such as cloud services), as an
attacker with access to one virtual machine can use Meltdown to access other
VMs on the same physical system. Meltdown is specific to Intel systems.

Spectre ([CVE-2017-5753](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2017-5753),
[CVE-2017-5715](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2017-5715))
is a broader vulnerability. Spectre relies on issues with speculative execution
itself to be carried out. In its current form, the attack is more complicated
as more prerequisites must be fulfilled. One of them is a code gadget, which
must be found in a code shared by both victim and attacker. For some variants
of this attack, a branch prediction CPU subsystem must be trained to redirect
execution of a code to the selected gadget.

The real challenge with Spectre is its mitigation. Unlike Meltdown, Spectre
requires changes to the hardware itself. As a workaround, some vulnerable code
can be mitigated by inserting synchronization primitives (like the `LFENCE`
instruction on Intel platforms) which effectively stops speculative execution.
Another one is using return trampoline approach (Retpoline). This approach
requires modification of compilers and careful selection of critical locations,
which is non-trivial and cannot be easily done without human interaction; doing
otherwise would impose a significant performance penalty.

## Prerequisites

This tool requires Linux kernel headers and [NumPy](https://numpy.org/).
For Ubuntu/Debian, you can install these packages in advance.
```shell
$ sudo apt install linux-headers-$(uname-r)
$ sudo apt install python3-numpy
```

## Usage

Build project:
```shell
$ git clone https://github.com/eecheng87/mode-switch-stat.git
$ cd mode-switch-stat
$ make
```

Testing:
```shell
$ make testing
```

Reference output
```
Model name: Intel(R) Core(TM) i5-7200U CPU @ 2.50GHz
Vulnerability Meltdown: Mitigation; PTI

Avg of mode switch takes 1921.45 cycles and standard deviation is 106.92
```

## Troubleshooting
1. ERROR: could not insert module msT.ko: Operation not permitted

    Solution: Disable SecureBoot (check by `mokutil --sb-state`)

2. If kernel version lower than `5.7.0`, please disable KALSR or modify macro `KERNEL_VERSION` to version lower than yours. It's also fine to comment it and force include `systab.h` and calculating `syscall_table`  by adding offset to original system call table.


## How to contribute
Add your output as comment in issue#1
