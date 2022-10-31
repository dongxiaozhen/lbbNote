
单个redis的qps 可以达到7万-10万
codis的qps 可以基本没有限制，和分片有关系，


redis数据保存
1 rdb save/bgsave (rdbSaveBackground)
2 aof always, everysec, no  (flushAppendOnlyFile)
	2.1 aof重写 
		触发条件：1 命令 2 aof文件的大小超过预定的百分比(参数auto-aof-rewrite-percentage，默认是100%)
		创建新的aof文件替换老的aof 文件 rewriteAppendOnlyFileBackground
		子进程完成之后通知父进程，父进程 backgroundRewriteDoneHandler
	2.2 aof 文件类型		    
		BASE：表示基础 AOF，它一般由子进程通过重写产生，该文件最多只有一个。		
    	INCR：表示增量 AOF，它一般会在 AOFRW 开始执行时被创建，该文件可能存在多个。
    	HISTORY：表示历史 AOF，它由 BASE 和 INCR AOF 变化而来，每次 AOFRW 成功完成时，本次 AOFRW 之前对应的 BASE 和 INCR AOF 都将变为 HISTORY ，HISTORY类型的AOF 会被Redis自动删除。

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
