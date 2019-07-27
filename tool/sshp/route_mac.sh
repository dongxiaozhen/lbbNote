#!/bin/bash

#内外网同时连接的脚本，线启动外网的vpn，然后启动内网的vpn,再执行此脚本

echo "clean old route ..."
sudo  route -n delete  10.31.0.0 

ip=`ifconfig|grep  10.31.63.|awk '{print $2}'`
echo "add new route, ip is $ip ..."
sleep 1
sudo route -n add -net 10.31.0.0 -netmask 255.255.0.0 $ip

sleep 1
ping -c 3 10.31.63.20
ping -c 3 10.33.108.232
