#!/bin/bash


run() {
   runningexename=$1
   exename=$2
   func2=$(readelf -s ${runningexename}  | grep func2 | awk '{print $2}')
   readelf -r ${runningexename} | grep R_X86_64_JUMP_SLO | awk '{print $1,$5}'  > hoge_data.txt
   readelf -r ${exename} | grep R_X86_64_JUMP_SLO | awk '{print $1,$5}' > lib_data.txt
   python3 create.py > data.txt
   #./${runningexename} &
   #sleep 1
   X=$(pidof ${runningexename})
   Y=$(objdump -t ${exename} | grep -w func3 | awk '{print $1}')
   Z=0x${Y}
   ./rewrite ${X} ${exename} 0x${func2}  ${Z} data.txt
   #sleep 1
   #pkill ${runningexename}
}

#run hoge lib
run A B
