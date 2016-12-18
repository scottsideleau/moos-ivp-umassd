#! /bin/bash

# Start by launching the simulation environment
echo "Starting the MOOS-IvP environment."
source simulation_launch.sh
sleep 1

# Wait for the IvP Helm to start
echo "Starting the MOOS-IvP mission."
while [ $(pgrep pHelmIvP) ]; do
  sleep 1
done

# Engage the IvP Helm (quietly)
source scripts/engage_ivphelm.sh >& /dev/null &
while [ $(pgrep uPokeDB) ]; do
  echo "Engaging the IvP Helm."
  sleep 1
done

# Release control to the IvP Helm (quietly)
source scripts/release_control.sh >& /dev/null &
while [ $(pgrep uPokeDB) ]; do
  echo "Releasing control to the IvP Helm."
  sleep 1
done

# Check if mission is still running
while [ $(pgrep pAntler) ]; do
  echo "MOOS-IvP mission running..."
  sleep 1
done
echo "Completed the MOOS-IvP mission."

# Post-process the MOOS-IvP mission
while [ $(pgrep pLogger) ]; do
  echo "Waiting for log files to be closed..."
  sleep 1
done

