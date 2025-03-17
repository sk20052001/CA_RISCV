  .section .text
	.align 2
	.globl	_start	

_start:
    addi x8, x8, 4
	jalr x1, x8, 8
	addi x19, x19, 20
	addi x20, x20, 1
	addi x21, x21, 2
    addi x22, x22, 3
.word 0