	.section .text
	.align 2
	.globl	_start		# toolchain likes _start
	.equ x, 0			# create a symbolic name (like a define) for X's position on stack
	.equ y, 4			# ditto for Y -- both 4 bytes
	.equ yb1, 5			# and displacement for 2nd byte of Y (little endian)
	
_start:
		addi sp, sp, -8		# allocate space for two 4-byte values (X,Y) on stack  
		li t1, 0xff010233	# create an "interesting" 4 byte value
		sw t1, x(sp)		# store to X on stack
		sw t1, y(sp)		# store to Y on stack
		sb t1, yb1(sp)		# store single byte into second byte of Y

	    la a2, var			# get the PC-relative address of var
	    lw t2, 0(a2)		# put contents of var into register
	    sw t2, x(sp)		# write it to X
		.word 0				# force simulator to halt with 0 -- undefined instruction
var:	.word 0x77553322  	# some statically defined data we can use
