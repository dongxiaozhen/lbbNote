#namespace

* https://man7.org/linux/man-pages/man7/namespaces.7.html

## Namespace:访问隔离 /proc/$$/ns 每个进程都有

    * Mount Namespace: 隔离文件系统挂载点
    * User Namespace: 隔离用户ID和组ID
    * UTS Namespace: 隔离主机名和域名
    * IPC Namespace: 隔离SYSTEMV IPC和POSIX消息队列
    * PID Namespace: 隔离进程ID
    * Net Namespace: 隔离网络资源

## namespace 的api主要使用下面的三个系统调用：

1. clone 创建新进程
2. unshare 将进程移除某个namespace,
3. setns 将进程加入某个namespace

### /proc/[pid]/ns/ directory

 Each process has a /proc/[pid]/ns/ subdirectory containing one
       entry for each namespace that supports being manipulated by

### /proc/sys/user directory

    The files in the /proc/sys/user directory (which is present since
       Linux 4.9) expose limits on the number of namespaces of various
       types that can be created.  The files are as follows
       * max_cgroup_namespaces
       * max_ipc_namespaces
       * max_mnt_namespaces
       * max_net_namespaces
       * max_pid_namespaces
       * max_time_namespaces
       * max_user_namespaces
       * max_uts_namespaces

## UTS namespace

 主要用来隔离nodename,domainname 两个系统标识

# control groups

* https://man7.org/linux/man-pages/man7/cgroups.7.html
  Cgroups 是对进程分组管理的一种机制，提供了对一组进程及它们的子进程的资源限制、控制和统计的能力，并为每种可以控制的资源定义了一个 subsystem （子系统）的方式进行统一接口管理，因此 subsystem 也被称为 resource controllers （资源控制器）。

查看1号进程 ps 1 
          1 ?        Ss     0:10 /sbin/init
查看 ls -ld   /sbin/init
    lrwxrwxrwx 1 root root 20 Mar  2 20:58 /sbin/init -> /lib/systemd/systemd

系统中运行的所有进程，都是 systemd init 进程的子进程。在资源管控方面，systemd 提供了三种 unit 类型：

    service： 一个或一组进程，由 systemd 依据 unit 配置文件启动。service 对指定进程进行封装，这样进程可以作为一个整体被启动或终止。
    scope：一组外部创建的进程。由进程通过 fork() 函数启动和终止、之后被 systemd 在运行时注册的进程，scope 会将其封装。例如：用户会话、 容器和虚拟机被认为是 scope。
    slice： 一组按层级排列的 unit。slice 并不包含进程，但会组建一个层级，并将 scope 和 service 都放置其中。真正的进程包含在 scope 或 service 中。在这一被划分层级的树中，每一个 slice 单位的名字对应通向层级中一个位置的路径。

我们可以通过 systemd-cgls 命令来查看 cgroups 的层级结构：
     systemd-cgls
     systemd-cgls /
     systemd-cgtop

  systemd-run  --unit=toptest  --slice=test.slice top -b

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-06-21-31-56-image.png)



![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-06-21-39-52-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-06-21-47-22-image.png)

子系统    作用
cpu    限制进程的 cpu 使用率
cpuacct    统计进程的 cpu 使用情况
cpuset    在多核机器上为进程分配单独的 cpu 节点或者内存节点（仅限 NUMA 架构）
memory    限制进程的 memory 使用量
blkio    控制进程对块设备（例如硬盘） io 的访问
devices    控制进程对设备的访问
net_cls    标记进程的网络数据包，以便可以使用 tc 模块（traffic control）对数据包进行限流、监控等控制
net_prio    控制进程产生的网络流量的优先级
freezer    挂起或者恢复进程
pids    限制 cgroup 的进程数量
更多子系统参考 Linux man cgroups[3]文档    https://man7.org/linux/man-pages/man7/cgroups.7.html

## cpu

cfs_period_us用来配置时间周期长度，cfs_quota_us用来配置当前cgroup在设置的周期长度内所能使用的CPU时间数，两个文件配合起来设置CPU的使用上限
    限制使用2个CPU（内核）（每500ms能使用1000ms的CPU时间，即使用两个内核）
           # echo 1000000 > cpu.cfs_quota_us /* quota = 1000ms */
           # echo 500000 > cpu.cfs_period_us /* period = 500ms */

cgroup.procs：查看这个文件显示的是当前在这个cgroup中的pid list。echo一个pid到这个文件可以将对应进程放入这个组中进行资源限制。

根旧版本cgroup功能类似，针对cpu的限制仍然可以支持绑定核心、配额和权重三种方式。只是配置方法完全不一样了

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-06-22-05-49-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-06-22-06-13-image.png)

## memory

文件名    功能
tasks    cgroup 中运行的进程（ PID）列表。将 PID 写入一个 cgroup 的 tasks 文件，可将此进程移至该 cgroup
cgroup.procs    cgroup 中运行的线程群组列表（ TGID ）。将 TGID 写入 cgroup 的 cgroup.procs 文件，可将此线程组群移至该 cgroup
cgroup.event_control    event_fd() 的接口。允许 cgroup 的变更状态通知被发送
notify_on_release    用于自动移除空 cgroup 。默认为禁用状态（0）。设定为启用状态（1）时，当 cgroup 不再包含任何任务时（即，cgroup 的 tasks 文件包含 PID，而 PID 被移除，致使文件变空），kernel 会执行 release_agent 文件（仅在 root cgroup 出现）的内容，并且提供通向被清空 cgroup 的相关路径（与 root cgroup 相关）作为参数
memory.usage_in_bytes    显示 cgroup 中进程当前所用的内存总量（以字节为单位）
memory.memsw.usage_in_bytes    显示 cgroup 中进程当前所用的内存量和 swap 空间总和（以字节为单位）
memory.max_usage_in_bytes    显示 cgroup 中进程所用的最大内存量（以字节为单位）
memory.memsw.max_usage_in_bytes    显示 cgroup 中进程的最大内存用量和最大 swap 空间用量（以字节为单位）
memory.limit_in_bytes    设定用户内存（包括文件缓存）的最大用量
memory.memsw.limit_in_bytes    设定内存与 swap 用量之和的最大值
memory.failcnt    显示内存达到 memory.limit_in_bytes 设定的限制值的次数
memory.memsw.failcnt    显示内存和 swap 空间总和达到 memory.memsw.limit_in_bytes 设定的限制值的次数
memory.oom_control    可以为 cgroup 启用或者禁用“内存不足”（Out of Memory，OOM） 终止程序。默认为启用状态（0），尝试消耗超过其允许内存的任务会被 OOM 终止程序立即终止。设定为禁用状态（1）时，尝试使用超过其允许内存的任务会被暂停，直到有额外内存可用。
更多文件的功能说明可以查看 kernel 文档中的 cgroup-v1/memory[4]    https://www.kernel.org/doc/Documentation/cgroup-v1/memory.txt

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-06-22-06-44-image.png)

## cgroups tool

lscgroup
lssubsys -m //查看子系统挂载
lssubsys -a //列出所有的子系统

cgexec -g cpu:my-cpu ./main // cgexec 指定cgroup运行程序c



# UnionFS

UnionFS 全称 Union File System （联合文件系统），是为 Linux、FreeBSD 和 NetBSD 操作系统设计的一种分层、轻量级并且高性能的文件系统，可以 把多个目录内容联合挂载到同一个目录下 ，而目录的物理位置是分开的，并且对文件系统的修改是类似于 git 的 commit 一样 作为一次提交来一层层的叠加的 。

目前 Docker 支持的 UnionFS 有以下几种类型：
联合文件系统    存储驱动    说明
OverlayFS    overlay2    当前所有受支持的 Linux 发行版的 首选 存储驱动程序，并且不需要任何额外的配置
OverlayFS    fuse-overlayfs    仅在不提供对 rootless 支持的主机上运行 Rootless Docker 时才首选
Btrfs 和 ZFS    btrfs 和 zfs    允许使用高级选项，例如创建快照，但需要更多的维护和设置
VFS    vfs    旨在用于测试目的，以及无法使用写时复制文件系统的情况下使用。此存储驱动程序性能较差，一般不建议用于生产用途
AUFS    aufs    Docker 18.06 和更早版本的首选存储驱动程序。但是在没有 overlay2 驱动的机器上仍然会使用 aufs 作为 Docker 的默认驱动
Device Mapper    devicemapper    RHEL （旧内核版本不支持 overlay2，最新版本已支持）的 Docker Engine 的默认存储驱动，有两种配置模式：loop-lvm（零配置但性能差） 和 direct-lvm（生产环境推荐）
OverlayFS    overlay    推荐使用 overlay2 存储驱动

## overlay2

overlay2 由四个结构组成，其中：
lowerdir ：表示较为底层的目录，对应 Docker 中的只读镜像层
upperdir ：表示较为上层的目录，对应 Docker 中的可写容器层
workdir ：表示工作层（中间层）的目录，在使用过程中对用户不可见
merged ：所有目录合并后的联合挂载点，给用户暴露的统一目录视图，对应 Docker 中用户实际看到的容器内的目录视图

** workdir 是用来进行一些中间操作的，其中就包括了原子性保证。

我们可以从读写的视角来理解，对于读的情况：
文件在 upperdir ，直接读取
文件不在 upperdir ，从 lowerdir 读取，会产生非常小的性能开销
文件同时存在 upperdir 和 lowerdir 中，从 upperdir 读取（upperdir 中的文件隐藏了 lowerdir 中的同名文件）
对于写的情况：
创建一个新文件，文件在 upperdir 和 lowerdir 中都不存在，则直接在 upperdir 创建
修改文件，如果该文件在 upperdir 中存在，则直接修改
修改文件，如果该文件在 upperdir 中不存在，将执行 copy_up 操作，把文件从 lowerdir 复制到 upperdir ，后续对该文件的写入操作将对已经复制到 upperdir 的副本文件进行操作。这就是 写时复制（copy-on-write）
删除文件，如果文件只在 upperdir 存在，则直接删除
删除文件，如果文件只在 lowerdir 存在，会在 upperdir 中创建一个同名的空白文件（whiteout file），lowerdir 中的文件不会被删除，因为他们是只读的，但 whiteout file 会阻止它们继续显示
删除文件，如果文件在 upperdir 和 lowerdir 中都存在，则先将 upperdir 中的文件删除，再创建一个同名的空白文件（whiteout file）
删除目录和删除文件是一致的，会在 upperdir 中创建一个同名的不透明的目录（opaque directory），和 whiteout file 原理一样，opaque directory 会阻止用户继续访问，即便 lowerdir 内的目录仍然存在

.
|-- lower1
|   `-- file1.txt
|-- lower2
|   `-- file2.txt
|-- merged
|-- upper
|   `-- file3.txt
`-- work

mount -t overlay overlay -olowerdir=lower1:lower2,upperdir=upper,workdir=work merged

挂载之后，
|-- merged
|   -- file1.txt
|   -- file2.txt
|   -- file3.txt

在merged目录下修改file1.txt之后，
|-- uper
|   -- file1.txt
|   -- file3.txt
