#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kallsyms.h>
#include <linux/vmalloc.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,7,0)
#include "systab.h"
#endif

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

#if defined(__x86_64__)
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
#elif defined(__aarch64__)
static struct mm_struct *init_mm_ptr;
// From arch/arm64/mm/pageattr.c.
struct page_change_data {
    pgprot_t set_mask;
    pgprot_t clear_mask;
};

// From arch/arm64/mm/pageattr.c.
static int change_page_range(pte_t *ptep, unsigned long addr, void *data)
{
    struct page_change_data *cdata = data;
    pte_t pte = READ_ONCE(*ptep);

    pte = clear_pte_bit(pte, cdata->clear_mask);
    pte = set_pte_bit(pte, cdata->set_mask);

    set_pte(ptep, pte);
    return 0;
}

// From arch/arm64/mm/pageattr.c.
static int __change_memory_common(unsigned long start, unsigned long size,
                  pgprot_t set_mask, pgprot_t clear_mask)
{
    struct page_change_data data;
    int ret;

    data.set_mask = set_mask;
    data.clear_mask = clear_mask;

    ret = apply_to_page_range(init_mm_ptr, start, size, change_page_range, &data);

    flush_tlb_kernel_range(start, start + size);
    return ret;
}

// Simplified set_memory_rw() from arch/arm64/mm/pageattr.c.
static int set_page_rw(unsigned long addr)
{
    vm_unmap_aliases();
    return __change_memory_common(addr, PAGE_SIZE, __pgprot(PTE_WRITE), __pgprot(PTE_RDONLY));
}

// Simplified set_memory_ro() from arch/arm64/mm/pageattr.c.
static int set_page_ro(unsigned long addr)
{
    vm_unmap_aliases();
    return __change_memory_common(addr, PAGE_SIZE, __pgprot(PTE_RDONLY), __pgprot(PTE_WRITE));
}

static void allow_writes(void) {
    set_page_rw((unsigned long)(syscall_table + __NR_emp) & PAGE_MASK);
}
static void disallow_writes(void) {
    set_page_ro((unsigned long)(syscall_table + __NR_emp) & PAGE_MASK);
}
static void enable_cycle_counter_el0(void* data)
{
	u64 val;
	/* Disable cycle counter overflow interrupt */
	asm volatile("msr pmintenset_el1, %0" : : "r" ((u64)(0 << 31)));
	/* Enable cycle counter */
	asm volatile("msr pmcntenset_el0, %0" :: "r" BIT(31));
	/* Enable user-mode access to cycle counters. */
	asm volatile("msr pmuserenr_el0, %0" : : "r"(BIT(0) | BIT(2)));
	/* Clear cycle counter and start */
	asm volatile("mrs %0, pmcr_el0" : "=r" (val));
	val |= (BIT(0) | BIT(2));
	isb();
	asm volatile("msr pmcr_el0, %0" : : "r" (val));
	val = BIT(27);
	asm volatile("msr pmccfiltr_el0, %0" : : "r" (val));
}

static void disable_cycle_counter_el0(void* data)
{
	/* Disable cycle counter */
	asm volatile("msr pmcntenset_el0, %0" :: "r" (0 << 31));
	/* Disable user-mode access to counters. */
	asm volatile("msr pmuserenr_el0, %0" : : "r"((u64)0));

}
#endif

static int __init msT_init(void) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,7,0)
	if (!(syscall_table = (void **)kallsyms_lookup_name("sys_call_table"))) {
		printk(KERN_ERR "Cannot find sys_call_table\nNeed CONFIG_KALLSYMS & CONFIG_KALLSYMS_ALL\n");
		return -ENOSYS;
	}
#else
	/* avoid effect of KALSR, get address of syscall table by adding offset */
    syscall_table = (void **)(scTab + ((char *)&system_wq - sysWQ));
#endif

#if defined(__aarch64__)
    init_mm_ptr = (struct mm_struct *)kallsyms_lookup_name("init_mm");
    on_each_cpu(enable_cycle_counter_el0, NULL, 1);
#endif

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

#if defined(__aarch64__)
    init_mm_ptr = (struct mm_struct *)kallsyms_lookup_name("init_mm");
    on_each_cpu(disable_cycle_counter_el0, NULL, 1);
#endif
}
module_init(msT_init);
module_exit(msT_exit);