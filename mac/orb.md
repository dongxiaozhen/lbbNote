# mac 虚拟

1. 支持docker, docker-compose 命令
2. 支持虚拟机
  orb 直接进入默认的ubuntu 机器
  ord run -m ubuntu1 cmd  执行机器ubutun1执行命令cmd
  ord sudo passwd libinbin  修改用户libinbin的密码
  虚拟机里命令执行前加上mac 就会执行mac里的命令
  在虚拟里执行open 可以打开文件
  在虚拟机里，/Users/libinbin 直接映射的就是mac 用户家目录, /mnt/mac 映射的是mac 根目录
  在mac里执行命令前加上orb 就会执行虚拟机里的命令
  orb pull // Copy files from Linux
  orb push  // Copy files to Linux
