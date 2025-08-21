.section .text

.extern exception_handler

# ISR stubs for exceptions that don't push error codes
# These need to push a dummy error code to keep stack aligned
.global isr0
isr0:
    cli                    # Disable interrupts
    push $0               # Push dummy error code
    push $0               # Push exception number (0 = divide error)
    jmp isr_common_stub

# ISR stubs for exceptions that DO push error codes
# These don't need to push a dummy error code
.global isr13
isr13:
    cli                    # Disable interrupts  
    push $13              # Push exception number (13 = general protection)
    jmp isr_common_stub

.global isr14  
isr14:
    cli                    # Disable interrupts
    push $14              # Push exception number (14 = page fault)
    jmp isr_common_stub

# Common ISR stub - saves state and calls C handler
isr_common_stub:
    pusha                  # Push all general purpose registers (EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI)
    
    mov %ds, %ax          # Save data segment
    push %eax
    
    mov $0x10, %ax        # Load kernel data segment (adjust if your GDT is different)
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs  
    mov %ax, %gs
    
    # At this point, the stack looks like:
    # [SS, ESP, EFLAGS, CS, EIP] <- pushed by CPU on interrupt
    # [error_code]               <- pushed by CPU (real) or us (dummy)  
    # [exception_number]         <- pushed by our ISR stub
    # [DS]                       <- pushed by us
    # [EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX] <- pushed by pusha
    
    mov %esp, %eax        # Pass pointer to exception frame as argument
    push %eax             # Push argument on stack
    call exception_handler # Call our C exception handler
    add $4, %esp          # Clean up argument from stack
    
    # Restore state
    pop %eax              # Restore original data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    
    popa                   # Restore all general purpose registers
    add $8, %esp          # Clean up exception number and error code
    sti                    # Re-enable interrupts
    iret                   # Return from interrupt