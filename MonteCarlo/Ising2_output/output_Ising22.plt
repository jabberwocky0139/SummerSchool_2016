reset
unset colorbox
set palette model RGB rgb 30,31,32
set pm3d map
#set xr [-25:25]
#set yr [-25:25]
set xr [0:]
set yr [0:]
#set zr[0:60];
set cbr[-1:1]

#(a)の場合
set multiplot layout 1,4

set size square
set title "Ising model : Initial state"
plot "output_ising2_1e-0.txt" every :::0::0 using 1:2:3 with image notitle
set size square
set title "Ising model : T=3"
plot "output_ising2_3e-0.txt" every :::1::1 using 1:2:3 with image notitle
set size square
set title "Ising model : T=2"
plot "output_ising2_2e-0.txt" every :::1::1 using 1:2:3 with image notitle
set size square
set title "Ising model : T=1"
plot "output_ising2_1e-0.txt" every :::1::1 using 1:2:3 with image notitle

unset multiplot
reset
set terminal qt
