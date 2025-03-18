  .section .text
	.align 2
	.globl	_start	

_start:
	addi x18, x18, 9
	addi x19, x19, 11
	bge x18, x19, taken
	addi x18, x18, -1
	bge x18, x19, taken2
	addi x21, x21, 2
.word 0

taken2:
	addi x19, x19, 12
.word 0

taken:
	addi x18, x18, -1
	bge x18, x19, taken2
.word 0
