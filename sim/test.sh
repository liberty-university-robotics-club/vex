#!/bin/bash

./build.sh
echo running tests
cp  ../include/robot.h ./ #this is the wrong thing to do but Im doing it anyway because its easy
gdb --exec=./testrobot 
tail ./testlog.txt

