#!/bin/bash

##############################################################################
# This script enables the IvP Helm much the same way that the 'Deploy' button 
# in pMarineViewer does.  The IvP Helm's health should be verified after 
# running this script.
##############################################################################

uPokeDB last.moos "DEPLOY=true"
uPokeDB last.moos "MOOS_MANUAL_OVERRIDE=false"

