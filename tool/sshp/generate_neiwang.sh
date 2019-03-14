#/bin/bash

unzip  libinbin.zip

cd libinbin/设备 && ls | grep xsh | xargs  grep UserName |  sed 's/\(^.*_ssh\).*UserName/\1/g' | awk -F'_ssh=' '{ printf "Host %s\n  HostName 10.31.63.2\n  User %s\n  PubkeyAuthentication no\n",$1,$2}'  > config.tmp

dos2unix config.tmp
ls | grep xsh | xargs rm -rf

cd ../..
filename=config.${datename}
mv libinbin/设备/config.tmp .
cat config.tmp >> ~/.ssh/config

rm -rf libinbin config.tmp
