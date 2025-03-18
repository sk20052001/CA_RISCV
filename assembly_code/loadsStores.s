    .section .text
	.align 2
	.globl	_start	

_start:
    addi sp, sp, -16
	sw s0, 12(sp)
	addi s0, sp, 16
    li a1, 0x00000078
	sb a1, 0(sp)
	li a2, 0x000000a8
	sb a2, 1(sp)
    li a3, 0x00005678
	sh a3, 2(sp)
	li a4, 0x0000a678
	sh a4, 4(sp)
	li a5, 0x12345678
	sw a5, 6(sp)
	lb s1, 0(sp) 
    lb s2, 1(sp) 
    lbu s3, 0(sp)
    lbu s4, 1(sp)
	lh s5, 2(sp)  
    lh s6, 4(sp)  
    lhu s7, 2(sp)  
    lhu s8, 4(sp) 
 	lw s9,6(sp)
	lw s0, 12(sp)
	addi sp, sp, 16
.word 0