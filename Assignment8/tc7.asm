ADD $t4,$t0,$t1
SUB $t5,$t0,$t4
SLL $t6,$t4,2
BNE $t1,$t6,5
SUB $t7,$t2,$t5
SW $t6,1024($t2)
LW $t3,1024($t2) 
