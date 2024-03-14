系统维护操作

1 compaction 压缩,删除之前的revision
    1.1 etcd --auto-compaction-retention=1h | 3m |1000  时间策略是指每隔一段时间压缩一次，默认单位是h，
             --auto-compaction-mode 'periodic|revision' 压缩策略，periodic时间过期
    1.2 etcdctl compact 11 命令行压缩

2 Defragmentation 碎片整理
    etcdctl defrag 压缩后会有磁盘碎片，用此命令压缩,
    注意：
      1 该命令只会执行在制定的endpoint，不会整个结群都操作。
      2 该命令执行期间，系统阻塞。

3 Space quota 空间预留
       etcd --quota-backend-bytes 预留多大的空间，超过此值报警。

4 snapshot
     etcdctl snapshot save
     etcdctl snapshot status 
     etcdctl snapshot restore

5 runtime config 
   member add 添加新server
   member del 删除
   member update 更新initial-advertise-peer-urls
   member list 读取
