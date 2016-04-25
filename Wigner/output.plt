reset
set terminal x11
set palette model RGB rgb 30,31,32
set pm3d map
set title "phase space with wigner"
set xl "q"
set yl "p"
set xr [-5:5]
set yr [-5:5]
#set zr[0:60];
set cbr[0:]

#(a)の場合
set size square
do for[i=0:29]{
	plot "output.txt" every :::i::i using 1:2:3 with image notitle
	pause 0.02
}

reset
set terminal qt
