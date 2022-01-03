ADD $t4,$t0,$t1
BGTZ $t4,3
BLEZ $t4,4
BNE $t4,$t1,6
SUB $t5,$t1,$t2
SLL $t6,$t1,2
SRL $t7,$t1,2