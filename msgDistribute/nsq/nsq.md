	 ___________________________________
	|                   				|
	|   ___			 ___    			|
	|  |   |		|   |   			|
	|  |___|		|___|   			|
	|  topic		topic   			|
	|_________________|_________________|
	|   ______________|____________		|
	|   _|__     _|__   _|__      _|__
	|  |   |    |   |  |   |      |   |	|
	|  |_C_|    |_C_|  |_C_|      |_C_|	| Chan
	|    |        |   				|   |
	|    |     	  |    				|	|
	|    |        |   				|	|
	|    |        |   				|	|
	|  	 |        |      			|	|
	|____|________|_________________|___|
		 |        |                 |
		 |	      |            _____|_____
       client    client       |           |
                            client      client


Topic/Channel是发布/订阅模型的一种实现。Topic对应于发布，Channel对应于订阅。消费者通过在Topic下生成不同的Channel来接收来自该Topic的消息。通过生成相同的Channel来实现消费者负载均衡。
Channel本身在投递消息给消费者时维护两个队列，一个是inFlight队列，该队列存储正在投递，但还没被标记为投递成功的消息。另一个是deferred队列，用来存储需要被延时投递的消息。
inFlight队列中消息可能因为投递超时而失败，deferred队列中的消息需要在到达指定时间后进行重新投递。如果为两个队列中的每个消息都分别指定定时器，无疑是非常消耗资源的。因此nsq采用定时扫描队列的做法。 
在扫描时采用多个worker分别处理。这种类似多线程的处理方式提高了处理效率。nsq在扫描策略上使用了Redis的probabilistic expiration算法，同时动态调整worker的数量，这些优化平衡了效率和资源占用。
	出自：http://blog.csdn.net/hurray123/article/details/50038419
	
一个nsqd可以有多个topic,一个topic可以有多个channel,一个channel可以有多个client
topic 会把每一个数据都复制到每个channel中
channel会把数据均衡的放到每一个注册的client中

新建一个topic,启动一个回写到文件的G，启动一个messagePumb的G，负责把topic的数据复制到每一个channel中,以及对topic的控制(读取发送过来的数据，channel的增删，topic暂停，程序退出)。
(tcp 协议)client链接上来,就会创建两个G，一个读取客户的输入并执行命令，一个读取channel的memoryMsgChan(如果sub了一个channel)，把数据发送出去,以及对client的控制。

一个nsqd会启动一个lookuploop,和lookup交互,iditify,regist,unregist

nsqd -------------->	nsqlookup	<------------------nsqadmin
	nsqlookupd 		tcp 4160 http 4161
	nsqd 			TCP:4150 HTTP:4151
	nsqadmin		HTTP:4171

										1.请求top信息，
nsqd <--------------------- nsq_to_file------------->nsqlookup
	2 连接nsqd,发送sub命令

	nsq_to_file 
		-个topic类型，启动一个G处理从nsqd接受到的数据，启动一个G写文件

nsqd
		nsqd连接nsqlookup,连接后发送IDENTIFY cmd,然后发送Register cmd,注册所有topic,channel
		启动一个G，向nsqlookup注册，销毁topic、channel
		nsqd根据客户端发送的RDY数量,以及nsqd记录的InFifht数据控制发给客户的数据流量
		inFlightPqueue 根据过期时间建立的小堆，


协议
    IDENTIFY  客户端连接到nsqd后首先发送该命令(只能发送一次)，更新服务器上的客户端元数据和协商功能，压缩(snappy,deflate),tls,msgTimeout,heatbeat,,
    CLS       客户端发送后服务器端会将客户端的rdyCount设置为0，不再发送数据。
    RDY       客户端通知服务器可以接受的消息数量。客户端会起一个G，更新RDY
    SUB       订阅一个topic/channel,,一个consume只能订阅一个
	consume发送FIN cmd确认数据收到，REQ cmd请求数据重发


go-nsq 
	一个consume 启动一个rdyLoop,设置是否可以让nsqd继续发送数据


go-diskqueue
	"%s.diskqueue.meta.dat" 保存元数据 格式"%d\n%d,%d\n%d,%d\n" depth\nreadFileNum,readPos\nwriteFileNum,writePos
	"%s.diskqueue.%06d.dat" 保存数据 格式 "%s%s" 数据长度,数据
	定时或者根据文件读写操作次数来sync同步文件
	-个G监控同步，读，写。

