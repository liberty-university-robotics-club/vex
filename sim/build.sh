#!/bin/bash
#TODO:makefile
cp ../include/robot.h ./
cp ../include/path.h ./
cp ../include/auto.h ./
echo Interactive simulation----------------------------------------------------
#cc -g -o isim ../src/opcontrol.c sim.c isim.c  -I ./
echo Static testing simulation-------------------------------------------------
#cc -g -o testrobot ../src/opcontrol.c sim.c static.c -I ./
echo Static path framework testing simulation----------------------------------
cc -g -o testrobot ../src/opcontrol.c ../src/auto.c ../src/path.c sim.c pathtest.c -I ./
