	.file	"TestCase.c"
	.option pic
	.attribute arch, "rv32i2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	li	a5,0x12345678
	sb	a5,-20(s0)
	li	a5,0x12345678
	sh	a5,-24(s0)
	li	a5,0x12345678
	sw	a5,-28(s0)
	lb	s1,-20(s0) 
	lh	s2,-24(s0)  
 	lw	s3,-28(s0)
	lw	s0,44(sp)
	addi	sp,sp,48
  .word 0
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
