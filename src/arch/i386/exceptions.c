#include <kernel/exceptions.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/kernel_levels.h>

void exception_handler(struct exception_frame *frame) {
    switch(frame->exception_num) {
        case EXCEPTION_DIVIDE_ERROR:
            divide_error_handler(frame);
            break;
        case EXCEPTION_GENERAL_PROTECTION:
            general_protection_handler(frame);
            break;
        case EXCEPTION_PAGE_FAULT:
            page_fault_handler(frame);
            break;
        default:
            printk(KERN_CRIT, "Unknown exception %d at EIP: 0x%x\n", 
                   frame->exception_num, frame->eip);
            panic("Unknown exception");
    }
}

void __attribute__((weak)) divide_error_handler(struct exception_frame *frame) {
    printk(KERN_CRIT, "Division by zero at EIP: 0x%x\n", frame->eip);
    printk(KERN_CRIT, "EAX: 0x%x, EBX: 0x%x, ECX: 0x%x, EDX: 0x%x\n", 
           frame->eax, frame->ebx, frame->ecx, frame->edx);
    panic("Division by zero");
}

void __attribute__((weak)) general_protection_handler(struct exception_frame *frame) {
    printk(KERN_CRIT, "General Protection Fault at EIP: 0x%x\n", frame->eip);
    printk(KERN_CRIT, "Error code: 0x%x\n", frame->error_code);
    panic("General protection fault");
}

void __attribute__((weak)) page_fault_handler(struct exception_frame *frame) {
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    
    printk(KERN_CRIT, "Page Fault at EIP: 0x%x\n", frame->eip);
    printk(KERN_CRIT, "Faulting address: 0x%x\n", faulting_address);
    printk(KERN_CRIT, "Error code: 0x%x\n", frame->error_code);
    panic("Page fault");
}

void exceptions_init(void) {
    printk(KERN_INFO, "\nExceptions OK.\n");
}