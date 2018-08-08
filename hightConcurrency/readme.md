# Goroutine
1、10万级别Goroutine调度不均衡
    如果一个进程Goroutine数量是10^3 级别，Go的问题不大。如果是10^4级别，Go的调度不一定高效。10^4级别的Goroutine数量调度不均衡。
2、大量Goroutine对象创建和销毁，GC影响系统响应速度
    大量Goroutine时，GC影响特别明显，因为一旦停顿一下，就会阻塞很多任务，一旦放开就会像洪水一样过来。
3、Goroutine调度没有优先级之分
    在一些高并发场景下，大量请求Goroutine和其他工作Goroutine数量级差别较大，他们之前调度是没有优先级之分，所以有时会导致调度不合理。

综上所诉：goroutine的数量不能太大，goroutine的创建不能频繁(最好用池)
          连接上来之后，最好不要开多个携程(一个读，一个写，一个业务),而是放在一个携程里完成所有的工作

#UnixSocket
  同一台机器上的数据传输使用管道，效率高于网络

#分层
   * 业务水平分层
   * 逻辑垂直分层

#防洪
    * 使用chan

#文章
   * https://mp.weixin.qq.com/s/tKwN4wucIE0SprC-ToRQcA
