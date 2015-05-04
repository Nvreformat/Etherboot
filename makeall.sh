#!/bin/sh

DIR=$( cd "$( dirname "$0" )" && pwd )
cd $DIR

cd kernel
make || exit
cd ..

cd linux
gcc main.c ethernet.c -o ../etherboot || exit
cd ..
