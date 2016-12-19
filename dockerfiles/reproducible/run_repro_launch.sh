#!/bin/bash

# Make the storage directory
mkdir /root/artifacts

# Run the workflow and copy the output to the storage directory
pushd /opt/moos-ivp-umassd/missions/reproducible
./repro_launch.sh
cp `find data_from_runs -type f -name *.png` \
  /root/artifacts/repro.png
popd
