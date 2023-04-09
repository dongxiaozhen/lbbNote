# 层次

集群配置，节点配置 以及 Pod 配置。与这三种情况对应的，其实是对集群网络 IP 段的三次划分：
首先 是集群 CIDR，接着为每个节点分配 podCIDR（即集群 CIDR 的子网段），最后在
podCIDR 里为每个 Pod 分配自己的 IP。

cni container network interface

1. flannel

2. calico





ipvlan_l2

![](ipvlan_l2.png)

ipvlan l3

![](ipvlan_l3.png)

```perl
# 创建网络空间
ip netns add net1
ip netns add net2
# 创建子接口
ip link add ipvlan1 link ens32 type ipvlan mode l3
ip link add ipvlan2 link ens32 type ipvlan mode l3
# 关联ipvlan l3与vm上的网络空间
ip link set ipvlan1 netns net1
ip link set ipvlan2 netns net2
# 设置不同网段
ip netns exec net1 ifconfig ipvlan1 192.168.10.135/24 up
ip netns exec net2 ifconfig ipvlan2 192.168.20.136/24 up
# 测试一下二则是否ping得通
ip netns exec net1 ping 192.168.20.136
```



maxvlan

![](macvlan.png)

```perl

```
