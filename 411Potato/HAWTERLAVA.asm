	spd #0($0)
:loop
	spd #0($0)
	addi $2, $2, #6
	rot #90($0)
	fir #6($0)
	spd #1($0)
	fir #6($0)
	spd #5($2)
	fir #6($0)
	spd #0($2)
	rot #45($2)
	spd #10($2)
	fir #6($0)
	fir #7($0)
	fir #8($0)
	j :loop