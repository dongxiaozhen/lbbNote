包装了http.Server,记录每一次连接wg.Add()/Done()，当调用close关闭一个server时，首先会关闭handle的开关(已经连接但是没有处理的连接会直接关闭)，其次会关闭listen(不接受新的连接),最后等待wg.wait所有连接都关闭。
程序需要保证所有的gracefulServe都推出后程序再关闭，wg.wait
