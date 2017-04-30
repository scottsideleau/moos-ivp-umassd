#! /bin/bash

pgrep pAntler | xargs -n1 -I ps -o pid --ppid {} | grep -v PID | sort -nr | xargs kill
echo "      * Terminated all MOOS communities"

