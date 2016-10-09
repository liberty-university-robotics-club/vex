#!/bin/bash
#TODO:makefile
#cant copy API.h
cp ../include/robot.h ./
cp ../include/tank.h ./
cp ../include/auto.h ./
cp ../include/lispinit.h ./
cp ../include/liftcontrol.h ./
cp ../include/controlloop.h ./
cp ../include/ir.h ./
echo Interactive simulation----------------------------------------------------
cc -g -o isim ../src/*.c sim.c isim.c  -I ./
echo Static testing simulation-------------------------------------------------
#cc -g -o testrobot ../src/opcontrol.c sim.c static.c -I ./
echo Static path framework testing simulation----------------------------------
#cc -g -o testrobot ../src/opcontrol.c ../src/auto.c ../src/path.c sim.c pathtest.c -I ./
