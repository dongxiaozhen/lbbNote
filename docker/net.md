 sudo ip netns exec nsl ifconfig vethO 172.18.0.2/24 up
 sudo apt install net-tools
 sudo ip netns exec nsl ifconfig vethO 172.18.0.2/24 up
 sudo ip netns exec ns2 ifconfig vethl 172.18.0.3/24 up
 sudo ip netns exec ns2 route add default dev vethl


 sudo ip netns add ns3
 sudo ip link add veth3 type veth peer name veth4
 sudo ip link set vethO netns nsl
 sudo ip link set veth4 setns ns3
 sudo brctl addbr brO
 sudo brctl addif brO ethO
 sudo brctl addif brO veth3

 github com/vishvananda/netlink
 github com/vishvananda/netns



Network: bridge host ipvlan macvlan null overlay
1. bridge 桥接，相当于网桥，工作在链路层
    // 安装
     sudo apt install bridge-utils

    // 创建虚拟网卡
    ip link add A type veth peer name B

    // A 端插入docker0
    brctl addif docker0 A
    ip link set A up

    // B 端插入容器
    PID=$(docker inspect -f '{{.State.Pid}}' contaminxxx)
    mkdir -p /var/run/netns
    ln -s /proc/$PID/ns/net /var/run/netns/$PID

    // B端放到命名空间
    ip link set B netns $PID
    ip netns exec $PID ip link set dev B name eth0
    ip netns exec $PID ip link set eth0 up

    // 设置ip
    ip netns exec $PID ip addr add 172.17.0.100/16 dev eth0
    ip netns exec $PID ip route add default via 172.17.0.1

2. host  和宿主机在一个命名空间
