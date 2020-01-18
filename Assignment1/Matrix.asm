# Manav Modi - 2018CS10353; Param Khakhar - 2018CS10362
# 07/01/2020
# Matrix.asm -- working with arrays
# Given two matrices, find their mean squared error

# --------------------------------------------------------------
# Comment:

# Registers used:
# $t0 - used to index the rows (looping index of the outer loop)     

# $t1 - used to index the columns (looping index of the inner loop) 

# $t2 - used to hold the array address calculated using $t0 and $t1. matrix1

# $t3 - used to hold the array address calculated using $t0 and $t1. matrix2

# $t4 - used as a utility variable

# $t5 - used as a utility variable

# $t6 - used to store the difference of the matrix entries

# $t7 - used as a utility variable

# $v0 - syscall parameter with return value

# $f1

# a0 - syscall parameter

# data segment
.data
	matrix1:	.word 3, 2, 3 
			.word 4, 5, 6
			.word 7, 8, 9
	
	matrix2:	.word 3, 4, 3 
			.word 4, 5, 6
			.word 7, 8, 1
	
	length:		.word 3
	space:	.asciiz " "
	msg:	.asciiz "First Assignment Completed!"
# ------------------------------------------------------------
# text/code segment

        .globl main
        .text
main:

	li	$t0, -1		# outer loop index, i = 0
	la	$t2, matrix1	# base address for matrix1
	la	$t3, matrix2	# base address for matrix2
	li	$t9, 0		# Sum of the error

outerLoop:
	
	add	$t0,$t0,1	# Incrementing i
	li	$t1,-1
	beq	$t0,3,endLoop	# End the outer loop if the outer index becomes 3
	blt	$t1,3,innerLoop	
	
innerLoop:
	
	add	$t1,$t1,1

	beq	$t1,3,outerLoop

	mul	$t4,$t0,3	# rowIdx * colSize
	add	$t4,$t4,$t1	# rowIdx*colSize + colIdx
	mul	$t4,$t4,4	# (rowIdx*colSIze + colIdx)*DATASIZE

	add	$t4,$t4,$t2	# adding baseeAddr $t2 for matrix1
	lw	$t6,($t4)	# value at matrix1[i][j]
	
	sub	$t4,$t4,$t2	# Subtracting $t2 from $t4
	add	$t4,$t4,$t3	# adding baseAddr $t3 for matrix2
	lw	$t7,($t4)	# value at matrix2[i][j]

	sub	$t8,$t7,$t6	# Computing difference
	mul	$t8,$t8,$t8	# Computing square
	add	$t9,$t9,$t8	# Updating sum
	blt	$t1,3,innerLoop	

endLoop:
	
	move $a0, $t9
	li $v0, 1
	syscall	

	li $v0,2
	mtc1 $t9, $f2
	cvt.s.w $f2, $f2
	syscall

	li.s $f3, 9.0
	div.s $f1,$f2,$f3		# dividing by 9
	li $v0,2
	syscall

	li $v0, 10
	syscall
.end main





































