#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include "systab.h"

#define __NR_emp 401

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Steven Cheng");
MODULE_DESCRIPTION("msT module");
MODULE_VERSION("0.1");

static void **syscall_table = 0;

/* restore original syscall for recover */
void *syscall_emp_ori;

asmlinkage void sys_empty(void) {
    return;
}

extern unsigned long __force_order __weak;
#define store_cr0(x) asm volatile("mov %0,%%cr0" : "+r"(x), "+m"(__force_order))
static void allow_writes(void) {
    unsigned long cr0 = read_cr0();
    clear_bit(16, &cr0);
    store_cr0(cr0);
}
static void disallow_writes(void) {
    unsigned long cr0 = read_cr0();
    set_bit(16, &cr0);
    store_cr0(cr0);
}

static int __init msT_init(void) {

    /* avoid effect of KALSR, get address of syscall table by adding offset */
    syscall_table = (void **)(scTab + ((char *)&system_wq - sysWQ));

    /* allow write */
    allow_writes();

    /* backup */
    syscall_emp_ori = (void *)syscall_table[__NR_emp];

    /* hooking */
    syscall_table[__NR_emp] = (void *)sys_empty;

    /* dis-allow write */
    disallow_writes();

    return 0;
}
static void __exit msT_exit(void) {
    /* recover */
    allow_writes();
    syscall_table[__NR_emp] = (void *)syscall_emp_ori;
    disallow_writes();
}
module_init(msT_init);
module_exit(msT_exit);