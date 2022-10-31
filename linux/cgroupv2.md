检查cgroup 版本，cgroupfs是v2, tmpfs是v1
stat -fc %T /sys/fs/cgroup/ 

在资源管控方面，systemd 提供了三种单位类型：
service —— 一个或一组进程，由 systemd 依据单位配置文件启动。service 对指定进程进行封装，这样进程可以作为一个整体被启动或终止。命名：name.service

　　scope —— 一组外部创建的进程。由强制进程通过 fork() 函数启动和终止、之后被 systemd 在运行时注册的进程，scope 会将其封装。例如：用户会话、 容器和虚拟机被认为是 scope。命名：name.scope

　　slice —— 一组按层级排列的单位。slice 并不包含进程，但会组建一个层级，并将 scope 和 service 都放置其中。真正的进程包含在 scope 或 service 中。在这一被划分层级的树中，每一个 slice 单位的名字对应通向层级中一个位置的路径。小横线（"-"）起分离路径组件的作用。
例如：如果一个 slice 的名字是：parent-name.slice
　　这说明 parent-name.slice 是 parent.slice 的一个子 slice。这一子 slice 可以再拥有自己的子 slice，被命名为：parent-name-name2.slice，以此类推。

system.slice —— 所有系统 service 的默认位置；
user.slice —— 所有用户会话的默认位置；
machine.slice —— 所有虚拟机和 Linux 容器的默认位置



systemd-cgls命令
lssubsys -am


与 cgroup 相关的 systemd 文档，可以使用man命令查阅
    systemd.resource-control(5) —— 描述系统单位共享的资源控制配置选项。
    systemd.unit(5) —— 描述所有单位配置文件的共同选项。
    systemd.slice(5) —— 提供 .slice 单位的基本信息。
    systemd.scope(5) —— 提供 .scope 单位的基本信息。
    systemd.service(5) —— 提供 .service 单位的基本信息。


systemd-run 指令用于创建、启动临时 service 或 scope 单位，并在此单位中运行自定义指令。
    systemd-run --unit=name --scope --slice=slice_name command

systemctl set-property 　　可以在应用程序运行时，持续修改资源管控设置
    如需使用命令列来限定 httpd.service 的 CPU 和内存占用量，请输入：
    ~]# systemctl set-property httpd.service CPUShares=600 MemoryLimit=500M
    如希望此更改为临时更改，请添加 --runtime 选项：
    ~]# systemctl set-property --runtime httpd.service CPUShares=600


systemctl list-units 使用下列指令将系统中所有被激活单位列表：
systemd-cgtop 提供的统计数据和控制选项与 top 实用工具所提供的相
systemd-cgls 命令提供的只是 cgroup 层级的静态信息快照，要想查看 cgroup 层级的动态信息，可以通过 systemd-cgtop 命令查看：
