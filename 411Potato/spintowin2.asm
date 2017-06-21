	spd #0($0)
	addi $2, $2, #0
	addi $3, $0, #12
:loop
	spd #0($0)
	addi $1, $1, #90
	addi $2, $2, #2
	beq $3, $2, :shiftdown
:back
	rot #0($2)
	fir #6($0)
	spd #1($2)
	fir #6($0)
	spd #5($2)
	fir #6($0)
	spd #0($2)
	rot #45($0)
	spd #10($2)
	fir #6($0)
	fir #7($0)
	fir #8($0)
	j :loop

:shiftdown
	addi $2, $0, #-24
	j:back