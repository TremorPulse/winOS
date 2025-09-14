.section .text
.extern exception_handler

.global isr0
isr0:
    cli                     
    push $0                 # Push dummy error code (divide error doesn't push one)
    push $0                 # Push exception number (0 = divide error)
    jmp isr_common_stub

.global isr13
isr13:
    cli                     # General protection fault already pushes error code
    push $13                # Push exception number (13 = general protection)
    jmp isr_common_stub

.global isr14  
isr14:
    cli                     # Page fault already pushes error code
    push $14                # Push exception number (14 = page fault)
    jmp isr_common_stub

isr_common_stub:
    pusha                   # Push all general purpose registers
    
    mov %ds, %ax            # Save data segment
    push %eax
    
    mov $0x10, %ax          # Load kernel data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs  
    mov %ax, %gs
    
    mov %esp, %eax          # Pass stack pointer as argument
    push %eax               
    call exception_handler  
    add $4, %esp            # Clean up argument
    
    pop %eax                # Restore original data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    
    popa                    # Restore all general purpose registers
    add $8, %esp            # Clean up error code and interrupt number
    sti                     # Re-enable interrupts
    iret                    # Return from interrupt