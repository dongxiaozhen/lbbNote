


etcd 保存key/value键值对，多个etcd之间可以组成一个clustering集群，所有集群里的数据保存同步
etcd集群里会有一个leader,写操作只能在leader上进行.在其他节点上写操作，会转发到leader上，leader再同步其他节点

watch
	一直关注某个key，当值变化了以后会通知watch的程序

lease
	一组key可以放在一个租赁里，设置一个TTL生存时间，当生存时间到期后或者删除租赁(revoke)，所以的key都会删除,一可以续组(grant),延长生存周期，或者直接设置租赁为永久的(keep-alive)

version
	修改值代表新的数据版本的生成，可以获取(get),关注(watch)相应版本的数据



listern-client-url 2379 监听客户端的请求
listern-peer-url 2380 etcd服务器之间交流端口


