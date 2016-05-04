reset
set palette model RGB rgb 30,31,32
unset colorbox
set pm3d map
#set xr [-25:25]
#set yr [-25:25]
set xr [0:]
set yr [0:]
#set zr[0:60];
set cbr[-1:1]

#(a)の場合
set size square
	set title sprintf("Initial D", i)
	plot "output_ising2D.txt" every :::0::0 using 1:2:3 with image notitle
	pause 1
	set title sprintf("Final D", i)
	plot "output_ising2D.txt" every :::1::1 using 1:2:3 with image notitle
	
reset
set terminal qt
