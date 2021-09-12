#! /bin/bash

##############################################################################
# This script is called by 'runtime_launch.sh' and 'simulation_launch' in the
# ../missions directory, so please be mindful of your edits.
##############################################################################

# Pass command line flags to nsplug.
FLAGS="${RUNTYPE} $@"

# Remove the current BHV file.
rm -f current.bhv
echo "      * removed the existing BHV file"

# Build the new BHV file.
nsplug bhv.meta current.bhv -f ${FLAGS}
echo "      * built new BHV file from plugs"

# Remove the current MOOS file.
rm -f last.moos
echo "      * removed the existing MOOS file"

# Build the new MOOS file.
nsplug moos.meta last.moos -f ${FLAGS}
echo "      * built new MOOS file from plugs"

# Launch the MOOS file.
pAntler last.moos >& /dev/null &
echo "      * launched the new MOOS file"

