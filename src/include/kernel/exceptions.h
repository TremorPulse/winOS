#pragma once
#include <stdint.h>

#define EXCEPTION_DIVIDE_ERROR              0x00
#define EXCEPTION_DEBUG                     0x01
#define EXCEPTION_BREAKPOINT                0x03
#define EXCEPTION_GENERAL_PROTECTION        0x0D
#define EXCEPTION_PAGE_FAULT                0x0E

struct exception_frame {
    /* Pushed by pusha instruction */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    
    /* Pushed by our assembly code */
    uint32_t ds;
    uint32_t exception_num, error_code;
    
    /* Pushed by CPU automatically */
    uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed));

extern void isr0(void);   // Division Error
extern void isr13(void);  // General Protection Fault  
extern void isr14(void);  // Page Fault

void exception_handler(struct exception_frame *frame);
void exceptions_init(void);

void __attribute__((weak)) divide_error_handler(struct exception_frame *frame);
void __attribute__((weak)) general_protection_handler(struct exception_frame *frame);
void __attribute__((weak)) page_fault_handler(struct exception_frame *frame);