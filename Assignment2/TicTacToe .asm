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
	intro2:	.asciiz "Each player is required to enter a number from 0-8 corresponding to the position as below. Player 1 goes first:\n"
	intro3:	.asciiz "0 1 2\n"
	intro4:	.asciiz "3 4 5\n"
	intro5: .asciiz	"6 7 8\n"

	cross:		.asciiz "X"
	dash:		.asciiz "-"
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

display1:
	li	$t6,0		# Initializing nested loop parameters. Outer loop i = 0
	la	$t7,array	# Address of the array.
	jal	outer1

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

	blt	$t0,9,display2	# If total number of turns are less than 9, it is player2's turn.
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


display2:
	li	$t6,0		# Initializing nested loop parameters. Outer loop i = 0
	la	$t7,array	# Address of the array.
	jal	outer2

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
	blt	$t0,9,display1	# If total number of turns are less than 9, it is player 1's turn.
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

outer1:
	
	li	$t1,0		# Inner loop variable
	jal	innerLoop	# Jumps to the inner loop.

	la	$a0,newline	# New line for printing next row	
	li	$v0,4
	syscall		

	add	$t6,$t6,1	# Incrementing j
	blt	$t6,3,outer1	
	j	player1		# End the outer loop if the outer index becomes 3

outer2:
	
	li	$t1,0		# Inner loop variable
	jal	innerLoop	# Jumps to the inner loop.

	la	$a0,newline	# New line for printing next row	
	li	$v0,4
	syscall		

	add	$t6,$t6,1	# Incrementing j
	blt	$t6,3,outer2	
	j	player2		# End the outer loop if the outer index becomes 3

	
innerLoop:
	
	lw	$t3,($t7)
	beq	$t3,0,printzero
	beq	$t3,1,printone
	beq	$t3,2,printtwo

printzero:
	la	$a0,dash
	li	$v0,4
	syscall
	b	label1

printone:
	la	$a0,cross
	li	$v0,4
	syscall
	b	label1

printtwo:
	li	$a0,0
	li	$v0,1
	syscall
	b	label1

label1:
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
	li 	$t3,7		
	beq 	$t8,$t3,endGame	
	li	$t3,56		
	beq 	$t8,$t3,endGame
	li	$t3,448		
	beq 	$t8,$t3,endGame
	li	$t3,73		
	beq 	$t8,$t3,endGame
	li	$t3,146		
	beq 	$t8,$t3,endGame
	li	$t3,292		
	beq 	$t8,$t3,endGame
	li	$t3,273		
	beq 	$t8,$t3,endGame
	li	$t3,84		
	beq 	$t8,$t3,endGame

	li	$t3,57							
	beq 	$t8,$t3,endGame
	li	$t3,449	
	beq 	$t8,$t3,endGame
	li	$t3,147		
	beq 	$t8,$t3,endGame
	li	$t3,293		
	beq 	$t8,$t3,endGame
	li	$t3,85	
	beq 	$t8,$t3,endGame
	
	li	$t3,58				
	beq 	$t8,$t3,endGame
	li	$t3,450	
	beq 	$t8,$t3,endGame
	li	$t3,75	
	beq 	$t8,$t3,endGame
	li	$t3,294	
	beq 	$t8,$t3,endGame
	li	$t3,275	
	beq 	$t8,$t3,endGame
	li	$t3,86
	beq 	$t8,$t3,endGame
	
	li	$t3,60
	beq 	$t8,$t3,endGame
	li	$t3,452
	beq 	$t8,$t3,endGame
	li	$t3,77	
	beq 	$t8,$t3,endGame
	li	$t3,150
	beq 	$t8,$t3,endGame
	li	$t3,277
	beq 	$t8,$t3,endGame

	li 	$t3,15
	beq 	$t8,$t3,endGame	
	li	$t3,456		
	beq 	$t8,$t3,endGame
	li	$t3,154
	beq 	$t8,$t3,endGame
	li	$t3,300
	beq 	$t8,$t3,endGame
	li	$t3,281	
	beq 	$t8,$t3,endGame
	li	$t3,92
	beq 	$t8,$t3,endGame

	li 	$t3,23		
	beq 	$t8,$t3,endGame	
	li	$t3,464		
	beq 	$t8,$t3,endGame
	li	$t3,89		
	beq 	$t8,$t3,endGame
	li	$t3,308		
	beq 	$t8,$t3,endGame

	li 	$t3,39		
	beq 	$t8,$t3,endGame	
	li	$t3,480		
	beq 	$t8,$t3,endGame
	li	$t3,105		
	beq 	$t8,$t3,endGame
	li	$t3,178		
	beq 	$t8,$t3,endGame
	li	$t3,305		
	beq 	$t8,$t3,endGame
	li	$t3,168		
	beq 	$t8,$t3,endGame

	li 	$t3,71		
	beq 	$t8,$t3,endGame	
	li	$t3,120		
	beq 	$t8,$t3,endGame
	li	$t3,210		
	beq 	$t8,$t3,endGame
	li	$t3,356		
	beq 	$t8,$t3,endGame
	li	$t3,337		
	beq 	$t8,$t3,endGame

	li 	$t3,135		
	beq 	$t8,$t3,endGame	
	li	$t3,184		
	beq 	$t8,$t3,endGame
	li	$t3,201		
	beq 	$t8,$t3,endGame
	li	$t3,420		
	beq 	$t8,$t3,endGame
	li	$t3,401		
	beq 	$t8,$t3,endGame
	li	$t3,212		
	beq 	$t8,$t3,endGame

	li 	$t3,263		
	beq 	$t8,$t3,endGame	
	li	$t3,312		
	beq 	$t8,$t3,endGame
	li	$t3,329		
	beq 	$t8,$t3,endGame
	li	$t3,402		
	beq 	$t8,$t3,endGame
	li	$t3,340		
	beq 	$t8,$t3,endGame
	jr	$ra

decision2:
	li	$t3,1 	  	#initializes t3 to 1
	sll 	$t3,$t3,$t4	#t3 = t3*(2^(value at t4))
	add 	$t9,$t9,$t3	#t9 = t9 + t3
	li 	$t3,7		
	beq 	$t9,$t3,endGame	
	li	$t3,56		
	beq 	$t9,$t3,endGame
	li	$t3,448		
	beq 	$t9,$t3,endGame
	li	$t3,73		
	beq 	$t9,$t3,endGame
	li	$t3,146		
	beq 	$t9,$t3,endGame
	li	$t3,292		
	beq 	$t9,$t3,endGame
	li	$t3,273		
	beq 	$t9,$t3,endGame
	li	$t3,84		
	beq 	$t9,$t3,endGame

	li	$t3,57							
	beq 	$t9,$t3,endGame
	li	$t3,449	
	beq 	$t9,$t3,endGame
	li	$t3,147		
	beq 	$t9,$t3,endGame
	li	$t3,293		
	beq 	$t9,$t3,endGame
	li	$t3,85	
	beq 	$t9,$t3,endGame
	
	li	$t3,58				
	beq 	$t9,$t3,endGame
	li	$t3,450	
	beq 	$t9,$t3,endGame
	li	$t3,75	
	beq 	$t9,$t3,endGame
	li	$t3,294	
	beq 	$t9,$t3,endGame
	li	$t3,275	
	beq 	$t9,$t3,endGame
	li	$t3,86
	beq 	$t9,$t3,endGame
	
	li	$t3,60
	beq 	$t9,$t3,endGame
	li	$t3,452
	beq 	$t9,$t3,endGame
	li	$t3,77	
	beq 	$t9,$t3,endGame
	li	$t3,150
	beq 	$t9,$t3,endGame
	li	$t3,277
	beq 	$t9,$t3,endGame

	li 	$t3,15
	beq 	$t9,$t3,endGame	
	li	$t3,456		
	beq 	$t9,$t3,endGame
	li	$t3,154
	beq 	$t9,$t3,endGame
	li	$t3,300
	beq 	$t9,$t3,endGame
	li	$t3,281	
	beq 	$t9,$t3,endGame
	li	$t3,92
	beq 	$t9,$t3,endGame

	li 	$t3,23		
	beq 	$t9,$t3,endGame	
	li	$t3,464		
	beq 	$t9,$t3,endGame
	li	$t3,89		
	beq 	$t9,$t3,endGame
	li	$t3,308		
	beq 	$t9,$t3,endGame

	li 	$t3,39		
	beq 	$t9,$t3,endGame	
	li	$t3,480		
	beq 	$t9,$t3,endGame
	li	$t3,105		
	beq 	$t9,$t3,endGame
	li	$t3,178		
	beq 	$t9,$t3,endGame
	li	$t3,305		
	beq 	$t9,$t3,endGame
	li	$t3,168		
	beq 	$t9,$t3,endGame

	li 	$t3,71		
	beq 	$t9,$t3,endGame	
	li	$t3,120		
	beq 	$t9,$t3,endGame
	li	$t3,210		
	beq 	$t9,$t3,endGame
	li	$t3,356		
	beq 	$t9,$t3,endGame
	li	$t3,337		
	beq 	$t9,$t3,endGame

	li 	$t3,135		
	beq 	$t9,$t3,endGame	
	li	$t3,184		
	beq 	$t9,$t3,endGame
	li	$t3,201		
	beq 	$t9,$t3,endGame
	li	$t3,420		
	beq 	$t9,$t3,endGame
	li	$t3,401		
	beq 	$t9,$t3,endGame
	li	$t3,212		
	beq 	$t9,$t3,endGame

	li 	$t3,263		
	beq 	$t9,$t3,endGame	
	li	$t3,312		
	beq 	$t9,$t3,endGame
	li	$t3,329		
	beq 	$t9,$t3,endGame
	li	$t3,402		
	beq 	$t9,$t3,endGame
	li	$t3,340		
	beq 	$t9,$t3,endGame
	jr	$ra

endLoop:
	li	$v0, 10	
	syscall

.end main



































