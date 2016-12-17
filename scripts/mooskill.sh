#! /bin/bash

pgrep pAntler | xargs -r -n1 -i ps -o pid --ppid {} | grep -v PID | sort -nr | xargs -r kill
echo "      * Terminated all MOOS communities"

