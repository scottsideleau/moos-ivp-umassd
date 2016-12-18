#! /bin/bash

# Start by launching the simulation environment
echo "Starting the MOOS-IvP environment."
source simulation_launch.sh

# Wait for the IvP Helm to start
while [ $(pgrep pHelmIvP) ]; do
  sleep 1
done

# Start the mission (quietly)
sleep 1
echo "Starting the MOOS-IvP mission."
source simulation_launch.sh
source scripts/engage_ivphelm.sh >& /dev/null &
source scripts/release_control.sh >& /dev/null &

# Check if mission is still running
while [ $(pgrep pAntler) ]; do
  echo "MOOS-IvP mission running..."
  sleep 1
done
echo "Completed the MOOS-IvP mission."

# Post-process the MOOS-IvP mission

