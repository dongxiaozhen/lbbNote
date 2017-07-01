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


一个nsqd可以有多个topic,一个topic可以有多个channel,一个channel可以有多个client
topic 会把每一个数据都复制到每个channel中
channel会把数据均衡的放到每一个注册的client中

新建一个topic,启动一个回写到文件的G，启动一个messagePumb的G，负责把topic的数据复制到每一个channel中,以及对topic的控制(读取发送过来的数据，channel的增删，topic暂停，程序退出)。
(tcp 协议)client链接上来,就会创建两个G，一个读取客户的输入并执行命令，一个读取channel的memoryMsgChan(如果sub了一个channel)，把数据发送出去,以及对client的控制。

一个nsqd会启动一个lookuploop,和lookup交互,iditify,regist,unregist

nsqd -------------->	nsqlookup	<------------------nsqadmin
	nsqd 			tcp 4160 http 4161
	nsqlookupd 		TCP:4150 HTTP:4151
	nsqadmin		HTTP:4171

										1.请求top信息，
nsqd <--------------------- nsq_to_file------------->nsqlookup
	2 连接nsqd,发送sub命令

	nsq_to_file 
		-个topic类型，启动一个G处理从nsqd接受到的数据，启动一个G写文件

		nsqd
		向nsqlookup注册，销毁topic、channel
