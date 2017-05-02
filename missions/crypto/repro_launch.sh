#! /bin/bash

# Start by launching the simulation environment
source simulation_launch.sh
echo "Starting the MOOS-IvP environment."

# Wait for the IvP Helm to start
until [ "$(pgrep pHelmIvP)" ]; do
  echo "Waiting on the IvP Helm..."
  sleep 1
done

# Engage the IvP Helm (quietly)
echo "Starting the MOOS-IvP mission."
source scripts/engage_ivphelm.sh >& /dev/null &
while [ "$(pgrep uPokeDB)" ]; do
  echo "Engaging the IvP Helm..."
  sleep 1
done

# Release control to the IvP Helm (quietly)
source scripts/release_control.sh >& /dev/null &
while [ "$(pgrep uPokeDB)" ]; do
  echo "Releasing control to the IvP Helm..."
  sleep 1
done

# Check if mission is still running
while [ "$(pgrep pAntler)" ]; do
  echo "MOOS-IvP mission running..."
  sleep 1
done
echo "Secret Message delivered."
echo "Completed the MOOS-IvP mission."
while [ "$(pgrep pLogger)" ]; do
  echo "Waiting for log files to be closed..."
  sleep 1
done

# Post-process the MOOS-IvP mission
echo "Generating artifacts."
cd data_from_runs/*/.

# Grab encrypted SECRET_MESSAGE from the alog file
aloggrep SECRET_MESSAGE iver*.alog > secret_message.alog

