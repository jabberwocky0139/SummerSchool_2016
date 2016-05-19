reset

unset xtics
unset ytics
set xr[-5:5]
set yr[0:13]
set yzeroaxis
set label 1 font "Arial,15"
set label 1 at graph 0.47,0.025 "A"
set label 2 font "Arial,15"
set label 2 at graph 0.47,0.63 "B"
set label 3 font "Arial,15"
set label 3 at graph 0.065,0.33 "C"
set label 4 font "Arial,15"
set label 4 at graph 0.065,0.89 "D"
set label 5 font "Arial,12"
set label 5 at screen  0.54,0.045 "x_0"
set label 6 font "Arial,12"
set label 6 at screen  0.001,0.32 "{/Symbol t}_i"
set label 7 font "Arial,12"
set label 7 at screen  0.13,0.295 "x'_i"
set label 8 font "Arial,12"
set label 8 at screen  0.3,0.295 "x_i"
set label 9 font "Arial,12"
set label 9 at screen  0.001,0.03 "{/Symbol t}_0"
set label 10 font "Arial,12"
set label 10 at screen  0.0005,0.9 "{/Symbol t}_N"
plot "path.txt" every :::0::0 using 2:1 w l lw 2 notitle, "path.txt" every :::1::1 using 2:1 with lines dt (10,5) notitle, "path.txt" every :::2::2 using 2:1 with lines dt (10,5) notitle, (x < -2 ? 4 : 1/0) w l dt (10, 5) notitle, (x < -4 ? 12 : 1/0) w l dt (10, 5) notitle, 1000*(x+4) w l dt (10, 5) notitle

