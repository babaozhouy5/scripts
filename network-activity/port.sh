#!/bin/bash
if [[ $1 =~ (tcp|udp) ]] || [ -z "$1" ];then
    :
else
    echo Wrong option!
    exit 1
fi
trap 'tput clear;exit 0' 2
col=$(tput cols)
while true; do
    tput clear
    lsof -nPi4$1 | \
        awk 'NR>1{split($9,ip,"->")split(ip[1],ip1,":")split(ip[2],ip2,":")
        printf("%-10s%5s %-15s%1s%5s %1s %-15s%1s%5s %s\n",$1,$2,ip1[1],(NR>1?":":""),ip1[2],ip[2]!=""?">":"",ip2[1],length(ip2)>1?":":"",ip2[2],$10)
        }'
    tput cup 0 $col
    sleep 5
done
