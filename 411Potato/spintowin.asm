	spd #0($0)
:loop
	spd #0($0)
	addi $1, $1, #90
	addi $2, $2, #6
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