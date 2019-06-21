#!/bin/sh

FILE=$1

time ./main single $FILE
echo ""
time ./main thread $FILE

