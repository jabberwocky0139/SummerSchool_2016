reset
set xr [-5:5]
set yr [0:5]
#set zr[0:60];

do for[i=0:59]{
	plot "output_test.txt" every :::i::i w l lw 2
	pause 0.1
}

reset
