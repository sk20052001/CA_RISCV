  .section .text
	.align 2
	.globl	_start	

_start:
	addi x19, x19, 1
	jal x1, J
	addi x20, x20, 2
	addi x21, x21, 3

J:
	addi x19, x19, 7
	addi x20, x20, 8
	.word 0
