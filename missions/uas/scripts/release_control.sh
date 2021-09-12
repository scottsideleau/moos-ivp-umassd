#!/bin/bash

##############################################################################
# This script enables iOceanServerComms to command the Frontseat Helm using
# the servo ($OMS) commands.  Please make sure the IvP Helm is engaged before
# running this script (i.e. see engage_ivphelm.sh).
##############################################################################

uPokeDB last.moos "VEHICLE_UNDERWAY=true"

