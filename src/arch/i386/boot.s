.set ALIGN,    1<<0             
.set MEMINFO,  1<<1             
.set FLAGS,    ALIGN | MEMINFO  
.set MAGIC,    0x1BADB002       
.set CHECKSUM, -(MAGIC + FLAGS) 

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
	movl $stack_top, %esp
	
	# Push multiboot info for kernel_main() function
	# EAX = magic number, EBX = multiboot info pointer
	pushl %eax    # Push magic number as second parameter
	pushl %ebx    # Push multiboot info pointer as first parameter
	
	call _init
	call kernel_main 
	
	cli
1:	hlt
	jmp 1b