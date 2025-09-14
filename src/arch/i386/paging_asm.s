/* 
Tell the processor where to find our page directory by putting it's address into the CR3 register. 
Because C code cannot directly access the computer's registers, we use assembly code to access CR3. 

Takes one parameter: the address of the page directory. 
It then loads the address onto the CR3 register, where the MMU will find it.
*/ 

.text
.global loadPageDirectory
loadPageDirectory:
push %ebp
mov %esp, %ebp
mov 8(%esp), %eax
mov %eax, %cr3
mov %ebp, %esp
pop %ebp
ret

.text
.global enablePaging
enablePaging:
push %ebp
mov %esp, %ebp
mov %cr0, %eax
or $0x80000000, %eax
mov %eax, %cr0
mov %ebp, %esp
pop %ebp
ret