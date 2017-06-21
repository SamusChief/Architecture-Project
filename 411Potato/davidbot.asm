	addi $1, $1, #90
:loop
	rot #90($0)
	spd #10($0)
	spd #0($0)
	dscan $1
	ascan $2
	bgtz $2 :skedaddle
	sub $1, $0, #6
	bgtz $1 :skedaddle
	rot #78($0)
	spd #-10($0)
	j :loop
:skedaddle
	rot #90($0)
	spd #10($0)
	j :loop