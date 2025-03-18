  .section .text
	.align 2
	.globl	_start	

_start:
	addi x18, x18, 12
	addi x19, x19, -11
	bge x18, x19, taken
	addi x10, x10, 20
	addi x20, x20, 1
	addi x21, x21, 2
.word 0

taken2:
	addi x19, x19, 12
	bge x18, x19, taken
	addi x19, x19, 11
.word 0

taken:
	addi x18, x18, -10
	bge x18, x19, taken2
.word 0