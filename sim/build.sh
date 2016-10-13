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
cp ../include/lisp.h ./

echo Interactive simulation----------------------------------------------------
gcc -g -o isim ../src/*.c sim.c isim.c  -I ./ -lm
#echo Static testing simulation-------------------------------------------------
#cc -g -o testrobot ../src/opcontrol.c sim.c static.c -I ./
#echo Static path framework testing simulation----------------------------------
#cc -g -o testrobot ../src/opcontrol.c ../src/auto.c ../src/path.c sim.c pathtest.c -I ./
