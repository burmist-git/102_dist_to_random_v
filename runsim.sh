#!/bin/bash

function printHelp {
    echo " --> ERROR in input arguments "
    echo "  -d             : default"
    echo "  -t             : test reading of conf.conf"
    echo "  -ts            : test sierpinski polygon (conf.conf -> test_sierpinski.root)"
    echo "  -p             : plotdigitizer"
    echo "  --timeReversal : timeReversal"
    echo "  -g             : gaus dist"
    echo " --p2            : second parameter"
}

if [ $# -eq 0 ]; then
    printHelp
else
    if [ "$1" = "-d" ]; then
	./runsim 0 conf.conf 30000 trajectories.root
    elif [ "$1" = "-t" ]; then
	./runsim 100 conf.conf
    elif [ "$1" = "-ts" ]; then
	./runsim 101 conf.conf test_sierpinski.root
    elif [ "$1" = "-p" ]; then
	#./runsim 1 conf.conf vadymovich.csv 3000 trajectories_vadym.root
	./runsim 1 conf.conf dashameer.csv 3000 trajectories_dashameer.root
    elif [ "$1" = "--timeReversal" ]; then
	./runsim 11 conf.conf vadymovich.csv 500 trajectories_vadym_inv.root trajectories_vadym.root 300 
    elif [ "$1" = "-g" ]; then
	./runsim 2 conf.conf 30000 trajectories_gausdist.root
    elif [ "$1" = "--p2" ]; then
	echo "test"
    else
        printHelp
    fi
fi
