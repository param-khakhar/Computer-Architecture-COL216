BLEZ $t1,8
BGTZ $t1,3
SUB $t1,$t1,$t0
BNE $t3,$t4,5
BEQ $t1,$t0,2
ADD $t1,$t1,$t0
J 0
