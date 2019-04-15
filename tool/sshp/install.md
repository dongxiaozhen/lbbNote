1 generate_** 脚本生成文件到 ~/.ssh/config
2 route 配置路由
3 /etc/bash_completion.d/sshp  补全命令
4  /usr/local/bin/sshp  连接脚本
5 sudo apt install expect   脚本使用获取服务发送的数据，然后匹配
6 sudo apt install rar unrar  解压使用


内网vpn设置
   sudo add-apt-repository ppa:nm-l2tp/network-manager-l2tp
   sudo apt-get update
   sudo apt-get install network-manager-l2tp-gnome
   停止服务器 sudo sysctl stop l2tp
              sudo sysctl disable l2tp

   setting--> network --->  vpn --> identity 设置 getway,username,password, 取消ipv4里的(use this connection for resource its network)
   设置完后，使用ip a 命令会出现新的网络,ppp0, 
   需要设置路由表, ip r 或者 route -n 查看路由，如果default走的ppp0，则删除它， sudo ip r del default dev ppp0

