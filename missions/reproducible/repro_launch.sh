#! /bin/bash

# Start by launching the simulation environment
source simulation_launch.sh
echo "Starting the MOOS-IvP environment."

# Wait for the IvP Helm to start
until [ $(pgrep pHelmIvP) ]; do
  echo "Waiting on the IvP Helm..."
  sleep 1
done

# Engage the IvP Helm (quietly)
echo "Starting the MOOS-IvP mission."
source scripts/engage_ivphelm.sh >& /dev/null &
while [ $(pgrep uPokeDB) ]; do
  echo "Engaging the IvP Helm..."
  sleep 1
done

# Release control to the IvP Helm (quietly)
source scripts/release_control.sh >& /dev/null &
while [ $(pgrep uPokeDB) ]; do
  echo "Releasing control to the IvP Helm..."
  sleep 1
done

# Check if mission is still running
while [ $(pgrep pAntler) ]; do
  echo "MOOS-IvP mission running..."
  sleep 1
done
echo "Completed the MOOS-IvP mission."
while [ $(pgrep pLogger) ]; do
  echo "Waiting for log files to be closed..."
  sleep 1
done

# Post-process the MOOS-IvP mission
echo "Processing the log file."
cd data_from_runs/*/.

# Grab NAV_X from the alog file
aloggrep NAV_X iver*.alog > nav_x.alog

# How to Remove Last N-lines from File (without `head -n -x` support)
# Accessed online: 
#   http://superuser.com/questions/543950/
#     opposite-of-tail-all-lines-except-the-last-n-lines
head -n $(( $(wc -l nav_x.alog | awk '{print $1}') - 5 )) nav_x.alog \
  > tmp && mv tmp nav_x.alog

# Skip first six lines, grab the data (stored in 4th column)
awk 'NR > 6 {print $4}' nav_x.alog > tmp && mv tmp nav_x.alog

# Repeat for NAV_Y
aloggrep NAV_Y iver*.alog > nav_y.alog
head -n $(( $(wc -l nav_y.alog | awk '{print $1}') - 5 )) nav_y.alog \
  > tmp && mv tmp nav_y.alog
awk 'NR > 6 {print $4}' nav_y.alog > tmp && mv tmp nav_y.alog

# Merge the x,y into a single two-column file
paste nav_x.alog nav_y.alog > nav_xy.alog

# Generate the image to use with compare.py
echo "Generating the XY plot."
gnuplot ../../../../scripts/compare-two-images/repro_png.gnuplot
echo "Done: temp_plot.png generated."

