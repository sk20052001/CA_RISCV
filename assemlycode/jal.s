  .section .text
	.align 2
	.globl	_start	

_start:
    addi x8, x8, 48
	jal x1, 4
	addi x19, x19, 20
.word 0