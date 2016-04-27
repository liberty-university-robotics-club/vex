#!/bin/bash
#TODO:makefile
cp ../include/robot.h ./
echo Building autonomous simulation-----------------------------------------------------
gcc -g -o testauto ../src/opcontrol.c ../src/auto.c sim.c tank.c  field.c -I ./ -lm && gdb --se=./testauto

