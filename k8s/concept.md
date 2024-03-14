k8s文档 https://github.com/kubernetes/website

CPU限流（throttle）
    https://github.com/kubernetes/website

Limits and requests for CPU resources are measured in cpu units. In Kubernetes, 1 CPU unit is equivalent to 1 physical CPU core, or 1 virtual core, depending on whether the node is a physical host or a virtual machine running inside a physical machine.

在调度的时候，kube-scheduler 只会按照 requests 的值进行调度。而在真正设置 Cgroups 限制的时候，kubelet 则会按照 limits 的值来进行设置。这是因为在实际场景中，大多数作业使用到的资源其实远小于它所请求的资源限额，这种策略能有效的提高整体资源的利用率。
The CPU limit defines a hard ceiling on how much CPU time that the container can use. During each scheduling interval (time slice), the Linux kernel checks to see if this limit is exceeded; if so, the kernel waits before allowing that cgroup to resume execution.
The CPU request typically defines a weighting. If several different containers (cgroups) want to run on a contended system, workloads with larger CPU requests are allocated more CPU time than workloads with small requests.

When you create a Pod, the Kubernetes scheduler selects a node for the Pod to run on. Each node has a maximum capacity for each of the resource types: the amount of CPU and memory it can provide for Pods. The scheduler ensures that, for each resource type, the sum of the resource requests of the scheduled containers is less than the capacity of the node. Note that although actual memory or CPU resource usage on nodes is very low, the scheduler still refuses to place a Pod on a node if the capacity check fails. This protects against a resource shortage on a node when resource usage later increases, for example, during a daily peak in request rate

kube-scheduler 给一个 Pod 做调度选择包含三个步骤：
    过滤：调用一组叫作 Predicate 的调度算法，将所有满足 Pod 调度需求的 Node 选出来；
    打分：调用一组叫作 Priority 的调度算法，给每一个可调度 Node 进行打分；
    绑定：调度器将 Pod 对象的 nodeName 字段的值，修改为得分最高的 Node。

cpu饱和度（saturation)
