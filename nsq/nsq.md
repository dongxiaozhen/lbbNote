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

新建一个topic,会启动一个messagePumb携程，负责把topic的数据复制到每一个channel中,以及对topic的控制。
(tcp 协议)client链接上来,就会创建两个携程，一个读取客户的输入并执行命令，一个读取channel的memoryMsgChan(如果sub了一个channel)，把数据发送出去,以及对client的控制。
