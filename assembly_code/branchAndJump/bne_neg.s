  .section .text
	.align 2
	.globl	_start	

_start:
	addi x18, x18, -12
	addi x19, x19, -11
	bne x18, x19, taken
	addi x20, x20, 1
	addi x21, x21, 2
.word 0

taken2:
	addi x19, x19, 1
.word 0

taken:
	addi x18, x18, -20
	addi x19, x19, -20
	bne x18, x19, taken2
.word 0
