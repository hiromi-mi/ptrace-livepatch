#!/bin/bash


run() {
   exename=$1
   ./hoge &
   sleep 1
   X=$(pidof hoge)
   Y=$(objdump -t ${exename} | grep func3 | awk '{print $1}')
   Z=0x${Y}
   ./rewrite ${X} ${exename} 0x555555555189  ${Z}
   sleep 1
   pkill hoge
}

run lib

