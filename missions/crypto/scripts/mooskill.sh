#! /bin/bash

pgrep pAntler | xargs -n1 -i ps -o pid --ppid {} | grep -v PID | sort -nr | xargs kill

echo "      * Terminated all MOOS communities"

