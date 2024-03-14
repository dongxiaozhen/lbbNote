#概念

1. nameservers 管理broker,
2. broker 管理数据和消费
3. topic 逻辑上的消息管理
4. partition 物理上的消息管理(queue)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-13-52-06-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-11-41-06-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-11-46-48-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-13-23-48-image.png)

#broker

* 参数设置
    ** deleteWhen=04  // 指定删除文件的时间点 默认凌晨4点
    ** fileReservedTime=48 // 过期时间
    ** mapedFileSizeCommitLog=1073741824 // commitLog每个文件的大小默认1G 
    ** mapedFileSizeConsumeQueue=300000  // ConsumeQueue每个文件默认存30W条，根据业务情况调整
    ** flushDiskType=SYNC_FLUSH / ASYNC_FLUSH // 刷盘
    ** maxMessageSize // 允许发送的最大消息体大小

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-13-38-13-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-13-39-18-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-13-58-22-image.png)

#queue

* 读写队列，则是在做路由信息时使用。在消息发送时，使用写队列个数返回路由信息，而消息消费时按照读队列个数返回路由信息。在物理文件层面，只有写队列才会创建文件。

#消费分发

1. producer 根据nameservers 传下来的broker,partition 管理消息分发到partition
   * QueueSelector 接口管理queue, 实现消息的转发逻辑，现在支持随机、hash、manual
   * AsyncSend  不能使用manual， 
   * WithCompressMsgBodyOverHowmuch 压缩参数
2. consumer 根据nameservers 传下俩的broker,partition 管理消息消费指定的partition, 
   * WithConsumerOrder = true 客户端消费消息有序的，一个goroutine 消费一个queue
   * WithConsumerOrder = false 客户端消费消息并行的，一个消息启动一个goroutine处理
   * 重启服务的过程中会引发再平衡rebalance操作, 或者定时拉取rebalance(一个周期20s）
   * 消费queue前，需要先上报并上锁。    
   * WithStrategy 设置分配策略

#分配策略

* AllocateByAveragely 平均分配，默认
* AllocateByAveragelyCircle 循环分配
* AllocateByConsistentHash 一致性哈希
* AllocateByMachineNearby  就近分配
* AllocateByMachineRoom   根据机房
* AllocateByConfig  根据配置进行分配
* https://blog.csdn.net/tianshouzhi/article/details/103607572?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-4.baidujs&dist_request_id=&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-4.baidujs

#集群
*RocketMQ: Master/Slave是个物理概念，1台机器，只能是Master或者Slave。在集群初始配置的时候，指定死的。其中Master的broker id = 0，Slave的broker id > 0。

#RocketMQ存储

* 1.Broker单个实例下所有的队列共用一个日志数据文件（即为CommitLog）来存储
* consumerQueue 是个消费的逻辑队列，保存了数据在commit log中的offset
* 消费读取数据，需要先读取consumerQueue，再读取commit log，消息主体都是通过CommitLog来进行读写.

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-16-56-08-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-16-55-50-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-17-00-21-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-17-14-18-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-22-51-35-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-22-59-58-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-22-59-39-image.png)

commitlog

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-09-42-19-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-06-23-05-17-image.png)

comsumequeue

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-11-18-31-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-11-18-15-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-11-20-12-image.png)

消息写入

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-11-25-39-image.png)

消息拉取

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-11-30-24-image.png)

index file

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-11-45-51-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-11-49-44-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-11-51-29-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-11-54-40-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-11-55-52-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-17-01-52-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-17-11-36-image.png)

rebalance

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-17-21-51-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-22-49-37-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-22-52-57-image.png)

幂等解决方案

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-23-06-36-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-23-31-47-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-07-23-40-01-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-08-10-15-46-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-08-11-23-27-image.png)

重试机制依赖于延迟消息队列

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-08-11-27-05-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-08-11-34-40-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-08-14-46-33-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-08-14-43-26-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-08-15-38-07-image.png)
