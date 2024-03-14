版本：

1. 版本号第二位是奇数，为非稳定版本

2. 版本号第二位是偶数，为稳定版本

3. redis7
   
   1. redis function (想替换lua)
   
   2. 多个aof 文件
   
   3. ACL v2
   
   4. listpack 替换ziplist
   
   5. snapshot 变动：
      
      1. old save 900 1 300 10 60 10000
      
      2. new save 3600 1 300 100 60 10000

4. 数据类型
   
   1. string
      
      1. 最大512M
      
      2. set
         
         1. keepttl 保持原来的ttl
         
         2. xx 存在设置
         
         3. nx 不存在设置
         
         4. get 返回老值
   
   2. list
      
      1. 最多2^32-1（40亿个）
   
   3. hash
      
      1. 每个hash最多2^32-1（40亿个）个field
   
   4. set
      
      1. 最多2^32-1（40亿个）成员
   
   5. sorted set
      
      1. 最多2^32-1（40亿个）成员
   
   6. biitmap 位图
      
      1. 最大位数2^32位，使用512M的内存可以统计42亿用户
   
   7. bitfield 位域
   
   8. stream 流
      
      1. ![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-03-26-13-52-26-image.png)
      
      2. ![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-03-26-13-49-16-image.png)
   
   9. hyperloglog 基数统计
      
      1. 用于统计一个集合中不重复的元素个数。
      
      2. 有误差，0.81%
      
      3. 使用12kB的空间
   
   10. geo 地理空间
       
       1. 底层是zset

单个redis的qps 可以达到7万-10万
codis的qps 可以基本没有限制，和分片有关系，

redis持久化

1. snapshot 快照 rdb save/bgsave (rdbSaveBackground)
   
   1. 占内存，cpu
   
   2. 数据丢失
   
   3. 适合定时一天或者30天保存一次，恢复到指定时间

2. aof always, everysec, no  (flushAppendOnlyFile)
   
   1. aof 会有内存缓存
   
   2. 存在三种写回策略
      
      1. always 总是
      
      2. everysec 每一秒
      
      3. no有操作系统决定缓存区写回磁盘
   
   3. aof重写 
      
              触发条件：1 命令 2 aof文件的大小超过预定的百分比(参数auto-aof-rewrite-percentage，默认是100%)
              创建新的aof文件替换老的aof 文件 rewriteAppendOnlyFileBackground
              子进程完成之后通知父进程，父进程 backgroundRewriteDoneHandler
   
   4. aof 文件类型            
      
      1. BASE：表示基础 AOF，它一般由子进程通过重写产生，该文件最多只有一个。        
      
      2. INCR：表示增量 AOF，它一般会在 AOFRW 开始执行时被创建，该文件可能存在多个。
      
      3. HISTORY：表示历史 AOF，它由 BASE 和 INCR AOF 变化而来，每次 AOFRW 成功完成时，本次 AOFRW 之前对应的 BASE 和 INCR AOF 都将变为 HISTORY ，HISTORY类型的AOF 会被Redis自动删除。
      
      4. manifest 清单文件管理上述aof文件
   
   5. ![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-03-26-20-49-42-image.png)

3. snapshot+aof
   
   1. aof-use-rdb-preamble = yes ,开启混合模式

4. lastsave 可以查看最近一次snapshot时间
   
        为了管理这些 AOF 文件，我们引入了一个 manifest（清单）文件来跟踪、管理这些 AOF
   
    2.3 流程
   
        在开始时我们依然会 fork 一个子进程进行重写操作，在主进程中，我们会同时打开一个新的 INCR 类型的 AOF 文件，在子进程重写操作期间，所有的数据变化都会被写入到这个新打开的 INCR AOF 中。子进程的重写操作完全是独立的，重写期间不会与主进程进行任何的数据和控制交互，最终重写操作会产生一个 BASE AOF。新生成的 BASE AOF 和新打开的 INCR AOF 就代表了当前时刻 Redis 的全部数据。AOFRW 结束时，主进程会负责更新 manifest 文件，将新生成的 BASE AOF 和 INCR AOF 信息加入进去，并将之前的 BASE AOF 和 INCR  AOF 标记为 HISTORY（这些 HISTORY AOF 会被Redis 异步删除）。一旦 manifest 文件更新完毕，就标志整个 AOFRW  流程结束。
   
    2.4 文件命名规则
   
         basename.suffix 的方式命名多个 AOF 文件。其中，appendfilename 配置内容将作为 basename 部分，suffix 则由三个部分组成，格式为 seq.type.format ，其中：
            seq为文件的序号，由1开始单调递增，BASE 和INCR 拥有独立的文件序号
            type为AOF的类型，表示这个 AOF 文件是 BASE 还是 INCR
            format， 用来表示这个 AOF 内部的编码方式，由于 Redis 支持 RDB preamble 机制，因此BASE AOF可能是RDB格式编码也可能是AOF格式编码：

redis 主从同步
    通过replicaof(redis 5.0之前使用slaveof命令）

repl_backlog_buffer 为了解决从库断连后找不到主从差异数据而设立的环形缓冲区，从而避免全量同步带来的性能开销
replication_buffer：对于客户端或从库与redis通信，redis都会分配一个内存buffer进行数据交互

sendBulkToSlave()函数将RDB文件写入到fd中，触发从节点的读事件，从节点调用readSyncBulkPayload()函数，来将RDB文件的数据载入数据库中
sendReplyToClient()当做发送缓冲区的对象

在链表中记录replica节点对数据块的引用(replBufBlock)，这样就能区分各个replica节点对master节点的更新情况。具体是使用引用法来记录着哪块数据块被使用着
redis使用的是rax树（前缀压缩树）结构,解决一个问题：当从库尝试和主库进行数据复制时，会传自己的repl_offset过来，定位到对应的replBufBlock数据块
ReplicationBuffer 由多个replBufBlock 组成链表，当复制积压区或从库对某个block 使用时，便对正在使用的replBufBlock 增加引用计数，上图中可以看到，复制积压区正在使用的replBufBlock refcount 是1，从库A 和B 正在使用的replBufBlock refcount 是2。当从库使用完当前的replBufBlock（已经将数据发送给从库）时，就会对其refcount 减1 而且移动到下一个replBufBlock，并对其refcount 加1。
ReplicationBacklog 和从库一样共享一份复制缓冲区的数据，也避免了 ReplicationBacklog 的内存开销。

事务，先放在队列里，然后依次执行

1. multi 标记事务开始

2. exec 执行所有事务内的所有命令

3. discard 取消事务，放弃执行事物那的所有命令

4. 不保证原子性，没有回滚操作

5. exec之前有命令有问题或者内存满了，全部执行失败

6. exec执行之后，出现的问题会被忽略，其他语句正常执行

7. watch 监控key的变化，后面加multi事务，如果key发生了变化，则exec会失败，相当于乐观锁的实现

8. unwatch 放弃监控key, 在multi开始事务前

管道

1. 复利用tcp连接

2. 缩短往返时间

3. 非原子性

复制 replica (高性能，容灾备份、)

1. 配从库不配主库
   
   1. info replication 查看从信息
   
   2. 从配置 replicaof 主库ip
   
   3. slaveof 重新拜山头
   
   4. slaveof no one 切换master
      
      #### master配置masterauth
      
      ![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-03-27-09-38-47-image.png)
      
      repl-ping-replica-period 10 心跳包

哨兵 sentinel (高可用)

1. 监控master,slave

2. 主从切换 master宕机，主动将slav切换到master

3. 多个哨兵，容错
   
   1. 配置 quorum 标识最少有几个哨兵认可客观下线，同意故障迁移的法定票数
   
   2. 主管下线，down-after-milliseconds 指定多少秒后，主节点没有反应，此时哨兵主观认为主节点下线
   
   3. ![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-03-27-10-06-33-image.png)

          4.  sentinel 启动后会在配置文件里保存redis和哨兵信息

        4. 修改sentinel配置![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-03-27-10-20-20-image.png)

        5. 修改redis 配置

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-03-27-10-37-40-image.png)

6 leader选举过程  raft算法(先到先得)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-03-27-10-47-34-image.png)

7 slave选举策略

    priority 是在config里配置的

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-03-27-10-59-29-image.png)

集群

1. slot 16384(2^14)

2. 节点最多1000

3. slotid = crc16(key) % 16384

4. 方便扩容

5. 一致性hash 不利于从新平衡slot分布

6. slot 曹位便于数据移动，以曹为单位移动

7. ![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-03-27-12-04-30-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-03-27-12-06-18-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-03-27-12-07-02-image.png)

配置

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-03-27-12-20-20-image.png)

启动参数

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-03-27-12-23-51-image.png)
