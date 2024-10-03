#!/bin/bash
# Remove all dirty files from the mission directory (e.g. log files)

SCRIPT_DIR=$(cd $(dirname "$0") ; pwd -P)

pushd "${SCRIPT_DIR}">& /dev/null
if [ $? -ne 0 ] ; then
  echo "ERROR! Failed to change into directory."
  exit 1
fi

rm -rf .LastOpenedMOOSLogDirectory \
  current.bhv \
  last.moos

# Remove all files except `data_from_runs/.keep`
find data_from_runs -type f ! -name ".keep" -exec rm -rf {} +
find data_from_runs/ -type d -empty -exec rmdir -p {} + >& /dev/null

popd >& /dev/null

