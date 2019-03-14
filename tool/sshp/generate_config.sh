#/bin/bash

unrar e libinbin.rar

ls | grep xsh | xargs  grep UserName |  sed 's/\(^.*_ssh\).*UserName/\1/g' | awk -F'_ssh=' '{ printf "Host %s\n  HostName 10.33.101.152\n  User %s\n  PubkeyAuthentication no\n",$1,$2}'  > config.tmp

dos2unix config.tmp
ls | grep xsh | xargs rm -rf

datename=$(date +%Y%m%d-%H%M%S)
filename=config.${datename}
mv ~/.ssh/config ~/.ssh/$filename
mv config.tmp ~/.ssh/config
