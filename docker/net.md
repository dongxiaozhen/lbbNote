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
