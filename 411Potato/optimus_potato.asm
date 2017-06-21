	addi $3, $0, #90
:loop
	dscn $2
	bgtz $2 :aim
	bgtz $1 :sub
	addi $1, $1, #90
	j :move
:sub
	sub $1, $1, $3
:move
	spd #5($0)
	rot #0($1)
	fir #6($0)
	spd #1($0)
	fir #6($0)
	spd #5($0)
	fir #6($0)
	spd #0($0)
	rot #45($0)
	spd #10($1)
	fir #6($0)
	fir #7($0)
	fir #8($0)
	j :loop
:aim
	ascn $1
	add $1, $1, $1  ;  Its approx twice the angle scan to rotate to them in one command
	rot #0($1)  ;  Rotate
	rot #0($0)  ;  By now we should be rotated
	fir #0($2)  ;  Fire at previously found distance,  hopefully its right still
	j :move
