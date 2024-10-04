#! /bin/bash

SCRIPT_DIR=$(cd $(dirname "$0") ; pwd -P)
RUNTYPE=simulation

pushd "${SCRIPT_DIR}">& /dev/null
if [ $? -ne 0 ] ; then
  echo "ERROR! Failed to change into directory."
  exit 1
fi

echo "//---------------------------------------------------------------------"
echo "// Building & Launching the Simple Robot Simulation"

source scripts/launch.sh

echo -e
echo "// End of line."
echo "//---------------------------------------------------------------------"

popd >& /dev/null

