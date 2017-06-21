
:loop
    dscn $2
    bgtz $2 :aim
    beq $0 $3 :left
    spd #-12($0)
    rot #60($0)
    spd #0($0)
    rot #0($0)
    addi $3, $0, #0
    j :loop
:left
    spd #-12($0)
    rot #-60($0)
    spd #0($0)
    rot #0($0)
    addi $3, $0, #1
    j :loop
:aim
    ascn $1
    add $1, $1, $1
	rot #0($1)
	rot #0($0)
    fir #0($2)
	j :loop
