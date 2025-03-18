  .section .text
	.align 2
	.globl	_start	

_start:
	addi x18, x18, 12
	addi x19, x19, -11
	bltu x18, x19, taken
	addi x20, x20, 1
	addi x21, x21, 2
.word 0

taken2:
	addi x16, x16, 13
.word 0

taken:
	addi x17, x17, -12
	bltu x17, x19, taken2
.word 0
