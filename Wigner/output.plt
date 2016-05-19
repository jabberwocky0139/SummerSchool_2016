reset
set palette model RGB rgb 30,31,32
unset colorbox
set pm3d map
set xl "q"
set yl "p"
#set xr [-25:25]
#set yr [-25:25]
set xr [-10:10]
set yr [-10:10]
#set zr[0:60];
set cbr[-0.05:0.4]

#(a)の場合
set size square

# gif動画を出力したい場合
#set term gif animate
#set output "wigner_false.gif"

do for[i=0:99]{
	set title sprintf("phase space on wigner : time = %d", i)
	plot "output_false.txt" every :::i::i using 1:2:3 with image notitle
	pause 0.01
}

reset
set terminal qt
