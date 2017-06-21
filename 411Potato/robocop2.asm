	rot #90($0)
:loop
	dscn $1
	sub $1 $0 #6
	blez $1 :preventSD
	add $1 $0 #6
	fir #0($1)
	fir #0($1)
	fir #0($1)
	j :loop
:preventSD
	fir #6($0)
	fir #6($0)
	fir #6($0)
	j :loop