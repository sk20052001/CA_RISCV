  .section .text
	.align 2
	.globl	_start	

_start:
    addi x8, x8, 4
	jalr x1, x8, 20
	addi x19, x19, 1
	addi x20, x20, 2
	addi x21, x21, 3
    jalr x1, x8, 20
	addi x19, x19, 4
	addi x20, x20, 5
	addi x21, x21, 6
    jalr x1, x8, -28
	addi x19, x19, 7
	addi x20, x20, 8
	addi x21, x21, 9
.word 0