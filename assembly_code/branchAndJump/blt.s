  .section .text
	.align 2
	.globl	_start	

_start:
	addi x18, x18, 11
	addi x19, x19, 12
	blt x18, x19, taken
	addi x20, x20, 1
	addi x21, x21, 2
.word 0

taken2:
	addi x18, x18, 10
.word 0

taken:
	addi x18, x18, -20
	addi x19, x19, -20
	blt x18, x19, taken2
.word 0
