**Tic Tac Toe game in Assmebly Language**
========================================

The assignment involves Tic-Tac-Toe game simulated in the assembly language. The specification for the game is as follows:

	1. Each player needs to enter an integer in the range of 0-8 in the following format.
		0 1 2
		3 4 5
		6 7 8

	2. Each player would be provided with a prompt unless any 1 player wins or until all the available positions are
	occupied i.e A draw has resulted. 

	3. At the completion of the game, the result (win or draw) is printed and the board is printed. The positions of 1
	indicate the positions selected by player1 and those of 2 indicate the positions selected by the player2. The positions
	0 indicate those which haven't been selected by either of the players.

**Implementation Details**

	- An array is maintained which is initialized by 9 zero's and is updated after the entries of each players.
	- Numbers(registers) are assigned to each of the players, namely $t8, and $t9. For each entry of the position
	these registers are updated by adding 2^i where i is the position entered by the corresponding player.
	- After the move made by each player, a check is made whether that player has made a winning move or not
	(branching off to the label decision1 and decision2. 
	- If any player has won, then a branch is made to the endGame where the board is printed using nested loops
	(outerLoop and innerLoop) and along with it other relevant information is printed.
	- If the player hasn't made a winning move, the execution is then branched to the next player and a prompt is 
	made to enter the position.
	- Since the positions entered by the players are stored in a common array, the invalid entries are checked by 
	comparing the position entered by the user and the value already present at that position in the array.


