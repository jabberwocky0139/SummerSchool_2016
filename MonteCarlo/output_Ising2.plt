reset
set palette model RGB rgb 30,31,32
set pm3d map
set xl "q"
set yl "p"
#set xr [-25:25]
#set yr [-25:25]
set xr [0:]
set yr [0:]
#set zr[0:60];
set cbr[-1:1]

#(a)の場合
set size square
set multiplot layout 3,1
do for[i=0:1]{
	set title sprintf("Ising model : %d", i)
	plot "output_ising2_3e-0.txt" every :::i::i using 1:2:3 with image notitle
	pause 1
}
unset multiplot
reset
set terminal qt
