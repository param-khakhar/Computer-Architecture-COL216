ADD $a0,$t2,$zero
ADD $t0,$t0,$t1
SUB $t1,$t2,$t1
JAL 5
J 21
ADD $sp,$sp,$t0
SW $ra, 1($sp)
SW $a0, 0($sp)
SUB $t3,$a0,$t1
BGTZ $t3,14
BEQ $t3,$zero,14
ADD $v0,$zero,$zero
SUB $sp,$sp,$t0
JR $ra
SUB $a0,$a0,$t1
JAL 5
LW $a0, 0($sp)
LW $ra, 1($sp)
SUB $sp,$sp,$t0
ADD $v0,$a0,$v0
JR $ra
ADD $t7,$zero,$v0
