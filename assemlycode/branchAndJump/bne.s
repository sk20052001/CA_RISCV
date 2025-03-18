  .section .text
	.align 2
	.globl	_start	

_start:
	addi x18, x18, 11
	addi x19, x19, 12
	bne x18, x19, 8
	addi x8, x8, 11
	addi x9, x9, 12
	addi x10, x10, 13
.word 0