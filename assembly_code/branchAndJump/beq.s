  .section .text
	.align 2
	.globl	_start	

_start:
	addi x18, x18, 1
	addi x19, x19, 1
	beq x19, x18, taken
	addi x18, x18, -1
	addi x19, x19, -1
.word 0

taken:
	addi x18, x18, 1
	addi x19, x19, 1
.word 0
