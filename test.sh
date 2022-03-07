#!/bin/bash


run() {
   func2=$(readelf -s hoge  | grep func2 | awk '{print $2}')
   exename=$1
   readelf -r hoge | grep GLIBC | awk '{print $1,$5}'  > hoge_data.txt
   readelf -r lib | grep GLIBC | awk '{print $1,$5}' > lib_data.txt
   python3 create.py > data.txt
   ./hoge &
   sleep 1
   X=$(pidof hoge)
   Y=$(objdump -t ${exename} | grep func3 | awk '{print $1}')
   Z=0x${Y}
   ./rewrite ${X} ${exename} 0x${func2}  ${Z} data.txt
   sleep 1
   pkill hoge
}

run lib

