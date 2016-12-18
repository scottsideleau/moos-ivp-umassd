#!/usr/bin/gnuplot

# Plotting with GNU Plots
# Accessed online:
#   http://unix.stackexchange.com/questions/32420/plotting-in-gnuplot
set term png size 800,800
set output "temp_plot.png"
plot 'nav_xy.alog' with linespoints

