producer
	只处理FrameTypeResponse，FrameTypeError两种类型的返回，FrameTypeMessage不处理	
	从nsq返回的消息是和发送出去的消息顺序一直的


consume
	NewConsumer 创建
	consumer.AddHandler(f) 添加回调
	DisableAutoResponse 取消自动回复finish,requeue命令
	ConnectToNSQLookupds 连接lookupd,取到所有topic的nsqd地址，连接，发送sub命令

	message.Finish() 回复FINISH命令




