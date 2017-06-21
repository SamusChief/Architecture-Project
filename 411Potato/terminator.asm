    spd     #-12($0)
    addi    $3, $0, #90
:loop
    addi    $1, $1, #31
    sub     $2, $1, $3
    blez    $2 :cont
    sub 	$1, $0, $3
:cont
    rot     #0($1)
    fir     #6($0)
    j       :loop
