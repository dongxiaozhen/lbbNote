

# 层次

集群配置，节点配置 以及 Pod 配置。与这三种情况对应的，其实是对集群网络 IP 段的三次划分：
首先 是集群 CIDR，接着为每个节点分配 podCIDR（即集群 CIDR 的子网段），最后在
podCIDR 里为每个 Pod 分配自己的 IP。