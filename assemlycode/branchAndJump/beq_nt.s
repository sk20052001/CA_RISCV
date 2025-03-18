  .section .text
	.align 2
	.globl	_start	

_start:
	addi x18, x18, 11
	addi x19, x19, 12
	beq x18, x19, 8
	addi x19, x19, 20
	addi x20, x20, 1
	addi x21, x21, 2
.word 0