  .section .text
	.align 2
	.globl	_start	

_start:
    addi x18, x18, 10
    addi x19, x19, 10
	beq x19, x18, 4
	addi x20, x20, 20
.word 0