# Manav Modi - 2018CS10353; Param Khakhar - 2018CS10362
# 17/01/2020
# TicTacToe.asm -- working with arrays
# Given two matrices, find their mean squared error

# --------------------------------------------------------------
# Comment:

# Registers used:
# $t0 - used to index the rows (looping index of the outer loop)     

# $t1 - used to index the columns (looping index of the inner loop) 

# $t2 - used to hold the array address calculated using $t0 and $t1. matrix1

# $t3 - utility variable

# $t4 - used to store the response of the players.

# $t5 - used to store the address of the array.

# $t6 - used to store the count of the number of turns.

# $t7 - used as a utility variable

# $v0 - syscall parameter with return value

# $f1

# a0 - syscall parameter

# data segment
.data
	
	array:		.word 0, 0, 0, 0, 0, 0, 0, 0, 0
	length:		.word 9
	intro1:	.asciiz "Welcome to tic-tac-toe.\n"
	intro2:	.asciiz "Each player is required to enter a number from 1-9 corresponding to the position as below. Player 1 goes first:\n"
	intro3:	.asciiz "0 1 2\n"
	intro4:	.asciiz "3 4 5\n"
	intro5: .asciiz	"6 7 8\n"

	player1input:	.asciiz "Player 1's turn:"
	player2input:	.asciiz "Player 2's turn:"
	space:		.asciiz " "
	newline:	.asciiz "\n"
	draw:		.asciiz "its a draw :(\n"
	p1win:		.asciiz "Player 1 wins! :)\n"
	p2win:		.asciiz "Player 2 wins! :)\n"
	inv1:		.asciiz "Invalid entry. Position already selected by other player!\n"	
# ------------------------------------------------------------
# text/code segment

        .globl main
        .text
main:
	la	$a0,intro1	# Printing the details of the game	
	li	$v0,4
	syscall

	la	$a0,intro2	# Printing the details of the game	
	li	$v0,4
	syscall

	la	$a0,intro3	# Printing the details of the game	
	li	$v0,4
	syscall

	la	$a0,intro4	# Printing the details of the game	
	li	$v0,4
	syscall

	la	$a0,intro5	# Printing the details of the game	
	li	$v0,4
	syscall


	li	$t6,0		
	li 	$t8,0		# Value for player1
	li  	$t9,0		# Value for player2
	li	$t0,0
	li	$t2,0		
	la	$t5,array

player1:

	la	$a0,player1input	# Prompt for player1 to input.	
	li	$v0,4
	syscall	 			
	
	li	$v0,5		# read_int syscall code = 5
	syscall	
	move	$t4,$v0		# syscall results returned in $v0

	mul	$t7,$t4,4	# Calculating offset
	add	$t7,$t7,$t5	# Calculating the address in the array

	lw	$t2,($t7)	
	bnez	$t2,invalid1

	li	$t3,1		# storing 1 corresponding to player 1's turn.
	sw	$t3,($t7)
	
	add	$t0,$t0,1	# Update the total number of turns.
	jal	decision1	# Check whether player 1 has won or not.
	blt	$t0,9,player2	# If total number of turns are less than 9, it is player2's turn.
	j	endGame

invalid1:
	la	$a0,inv1	# Prompt for player1 to input.	
	li	$v0,4
	syscall
	b player1
	
invalid2:
	la	$a0,inv1	# Prompt for player2 to input.	
	li	$v0,4
	syscall
	b player2

player2:

	la	$a0,player2input	# Prompt for player1 to input.	
	li	$v0,4
	syscall	 			
	
	li	$v0,5		# read_int syscall code = 5
	syscall	
	move	$t4,$v0		# syscall results returned in $v0

	mul	$t7,$t4,4	# Calculating offset
	add	$t7,$t7,$t5	# Calculating the address in the array

	li	$t3,2		# Storing 2 corresponding to player2

	lw	$t2,($t7)	
	bnez	$t2,invalid2

	sw	$t3,($t7)	
	
	add	$t0,$t0,1	# Update the total number of turns.
	jal	decision2	# Check whether player 2 has won or not.
	blt	$t0,9,player1	# If total number of turns are less than 9, it is player 1's turn.
	j	endGame

endGame:
	beq	$t0,9,ciao	# Three possible results, p1 win, p2 win or a draw
	rem	$t1,$t0,2	
	beq	$t1,0,p2wins	
	beq	$t1,1,p1wins
	
p1wins:
	la	$a0,p1win	# Congratulations! to player 1.
	li	$v0,4
	syscall
	li	$t0,0		# Initializing nested loop parameters. Outer loop i = 0
	la	$t7,array	# Address of the array.
	j	outerLoop

p2wins:
	la	$a0,p2win	# Congratulations! to player2.
	li	$v0,4
	syscall
	li	$t0,0		# Initializing nested loop parameters. Outer loop i = 0
	la	$t7,array	# Address of the array.
	j	outerLoop

ciao:
	la	$a0,draw
	li	$v0,4
	syscall
	li	$t0,0		# Outer loop i = 0.
	la	$t7,array	# Address of the array.
	j	outerLoop

outerLoop:
	
	li	$t1,0		# Inner loop variable
	jal	innerLoop	# Jumps to the inner loop.

	la	$a0,newline	# New line for printing next row	
	li	$v0,4
	syscall		

	add	$t0,$t0,1	# Incrementing j
	blt	$t0,3,outerLoop	
	j	endLoop		# End the outer loop if the outer index becomes 3
	
innerLoop:
	
	lw	$t3,($t7)
	move	$a0,$t3		
	li	$v0,1
	syscall

	la	$a0,space		# New line for printing next row	
	li	$v0,4
	syscall
	
	add	$t7,$t7,4
	add	$t1,$t1,1		
	blt	$t1,3,innerLoop	
	jr	$ra		# Again resumes from outerloop.

decision1:
	li	$t3,1 	  	#initializes t3 to 1
	sll 	$t3,$t3,$t4	#t3 = t3*(2^(value at t4))
	add 	$t8,$t8,$t3	#t8 = t8 + t3
	li 	$t3,5		#from here we check if different winner positions have been achieved or not.So 5 stands for 0,0,0,0,0,0,1,1,1
	beq 	$t8,$t3,endGame	
	li	$t3,56		#56 stands for 0,0,0,1,1,1,0,0,0
	beq 	$t8,$t3,endGame
	li	$t3,448		#448 stands for 1,1,1,0,0,0,0,0,0
	beq 	$t8,$t3,endGame
	li	$t3,73		#73 stands for 0,0,1,0,0,1,0,0,1
	beq 	$t8,$t3,endGame
	li	$t3,146		#146 stands for 0,1,0,0,1,0,0,1,0
	beq 	$t8,$t3,endGame
	li	$t3,292		#292 stands for 1,0,0,1,0,0,1,0,0
	beq 	$t8,$t3,endGame
	li	$t3,273		#273 stands for 1,0,0,0,1,0,0,0,1
	beq 	$t8,$t3,endGame
	li	$t3,84		#84 stands for 0,0,1,0,1,0,1,0,0
	beq 	$t8,$t3,endGame
	jr	$ra

decision2:
	li	$t3,1 	  	#initializes t3 to 1
	sll 	$t3,$t3,$t4	#t3 = t3*(2^(value at t4))
	add 	$t9,$t9,$t3	#t9 = t9 + t3
	li 	$t3,5		#from here we check if different winner positions have been achieved or not.So 5 stands for 0,0,0,0,0,0,1,1,1
	beq 	$t9,$t3,endGame	
	li	$t3,56		#56 stands for 0,0,0,1,1,1,0,0,0
	beq 	$t9,$t3,endGame
	li	$t3,448		#448 stands for 1,1,1,0,0,0,0,0,0
	beq 	$t9,$t3,endGame
	li	$t3,73		#73 stands for 0,0,1,0,0,1,0,0,1
	beq 	$t9,$t3,endGame
	li	$t3,146		#146 stands for 0,1,0,0,1,0,0,1,0
	beq 	$t9,$t3,endGame
	li	$t3,292		#292 stands for 1,0,0,1,0,0,1,0,0
	beq 	$t9,$t3,endGame
	li	$t3,273		#273 stands for 1,0,0,0,1,0,0,0,1
	beq 	$t9,$t3,endGame
	li	$t3,84		#84 stands for 0,0,1,0,1,0,1,0,0
	beq 	$t9,$t3,endGame
	jr	$ra


endLoop:
	li	$v0, 10	
	syscall

.end main



































