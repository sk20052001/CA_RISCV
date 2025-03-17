  .section .text
	.align 2
	.globl	_start	

J:
	addi x19, x19, 7
	addi x20, x20, 8
	.word 0

_start:
	addi x19, x19, 1
	addi x20, x20, 2
	jal x1, J
	addi x21, x21, 3
	.word 0
	