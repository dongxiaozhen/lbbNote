#/bin/bash

#外网sshp 连接前，使用此脚本添加设备信息到config

#unrar e libinbin.rar

cd ~/Downloads/libinbin-3/ && mkdir libin && find . -name *.xsh -exec mv  {} libin/ \; && cd libin && ls | xargs  grep UserName |  sed 's/\(^.*_ssh\).*UserName/\1/g' | awk -F'_ssh=' '{ printf "Host %s\n  HostName 10.33.101.152\n  User %s\n  PubkeyAuthentication no\n",$1,$2}'  > config.tmp

dos2unix config.tmp
ls | grep xsh | xargs rm -rf

datename=$(date +%Y%m%d-%H%M%S)
filename=config.${datename}
mv ~/.ssh/config ~/.ssh/$filename
mv config.tmp ~/.ssh/config
