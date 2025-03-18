  .section .text
	.align 2
	.globl	_start	

_start:
	addi x18, x18, 11
	addi x19, x19, 11
	beq x18, x19, 24
	addi x8, x8, 11
	addi x9, x9, 11
	beq x18, x19, 24
	addi x8, x8, 12
	addi x9, x9, 12
	beq x18, x19, -20
	addi x8, x8, 13
	addi x9, x9, 13
.word 0