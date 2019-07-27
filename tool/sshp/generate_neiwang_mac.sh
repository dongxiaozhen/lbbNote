#/bin/bash

#内网sshc 连接前，使用此脚本添加设备信息到config

#unzip  libinbin.zip

cd /Users/libinbin/Downloads/libinbin-2/  && find . -name *.xsh |  xargs  grep UserName |  sed 's/\(^.*_ssh\).*UserName/\1/g' | awk -F'_ssh=' '{ printf "Host %s\n  HostName 10.31.63.2\n  User %s\n  PubkeyAuthentication no\n",$1,$2}'  > config.tmp

dos2unix config.tmp
ls | grep xsh | xargs rm -rf

cat config.tmp >> ~/.ssh/config
