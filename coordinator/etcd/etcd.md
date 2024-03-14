![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-10-15-31-24-image.png)

create_version: 创建时的id
mod_version:    最后一次修改时的id
version:        现在的id
delete操作后，create_version，mod_version都会重置。



linearizable get请求都要到leader,在leader端点操作，默认,数据准确
Serializable get请求在连接的端点操作，快,数据可能不一致



etcd 保存key/value键值对，多个etcd之间可以组成一个clustering集群，所有集群里的数据保存同步
etcd集群里会有一个leader,写操作只能在leader上进行.在其他节点上写操作，会转发到leader上，leader再同步其他节点

watch
    一直关注某个key，当值变化了以后会通知watch的程序
    watch过程中中断了连接，一直到服务重新连接上后的所有更新都会发送过来

lease
    grant 先申请一个租约，key和租约绑定。租约过期，所有的key都会被删除
    一组key可以放在一个租赁里，设置一个TTL生存时间，当生存时间到期后或者删除租赁(revoke)，所以的key都会删除,一可以续组(grant),延长生存周期，或者直接设置租赁为永久的(keep-alive)
    LeaseRevoke 设置租约下的所有key过期
    LeaseKeepAlive 续约，保持租约一直有效，客户端一秒发送一个心跳包
    LeaseTimeToLive 查看租约剩余有效时间

version
    修改值代表新的数据版本的生成，可以获取(get),关注(watch)相应版本的数据

listern-client-url 2379 监听客户端的请求
listern-peer-url 2380 etcd服务器之间交流端口

etcd 底层有重连机制，
   put的时候服务器跌机，put会一直阻塞，直到服务重启
   watch的时候服务跌机，服务重启后，重连。

--max-request-bytes 最大发送的请求字节数，默认1.5M

local cluster
       go get github.com/mattn/goreman

etcd grpc-proxy 代理L7，
     grpc-proxy可以key的range值，grpc-proxy选择一个后端etcd-server连接，如果后端失效了，不指定revision的watch可能取到前几次的值，最新的没有缓存下来。
                 +-------------+
                 | etcd server |
                 +------+------+
                    ^ watch key A (s-watcher)
                    |
                 +-------+-----+
                 | gRPC proxy  | <-------+
                 |             |         |
                 ++-----+------+         |watch key A (c-watcher)
            watch key A ^     ^ watch key A    |
            (c-watcher) |     | (c-watcher)    |
                +-------+-+  ++--------+  +----+----+
                |  client |  |  client |  |  client |
                |         |  |         |  |         |
                +---------+  +---------+  +---------+

     grpc-proxy可以管理lease，将多个租约合并成一条请求发送到服务器端，减轻服务端的压力.
                        +-------------+
                        | etcd server |
                        +------+------+
                               ^
                               | heartbeat L1, L2, L3
                               | (s-stream)
                               v
                        +-------+-----+
                        | gRPC proxy  +<-----------+
                        +---+------+--+            | heartbeat L3
                            ^      ^               | (c-stream)
               heartbeat L1 |      | heartbeat L2  |
               (c-stream)   v      v (c-stream)    v
               +------+-+  +-+------+  +-----+--+
               | client |  | client |  | client |
               +--------+  +--------+  +--------+
    
       Namespacing
            连接到该代理的key都会自动加上一个前缀
               --namespace=my-prefix 
    
     etcd grpc-proxy start --endpoints=127.0.0.1:12379,127.0.0.1:22379,127.0.0.1:32379 --listen-addr=127.0.0.1:23790 --advertise-client-url=127.0.0.1:23790 --enable-pprof --resolver-prefix="___grpc_proxy_endpoint" --resolver-ttl=60
      --resolver-prefix 分组grpc-proxy，方便客户端使用cli.Sync()方法获取一组地址 

etcd gateway  代理L4
      只是简单的tcp代理

etcdctl  put /svr/bag/s1 444
etcdctl  get /svr/bag/s1 
etcdctl  compact -w="json" 72
etcdctl  member list -w="table"
etcdctl watch --prefix=true  /svr/
etcdctl lease grant 100
etcdctl lease timetolive  2246612d26135564
etcdctl lease timetolive  --keys 2246612D2613556F
etcdctl lease keep-alive  32696290C7D41A0A



![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-10-21-08-57-image.png)

etcd 使用b+tree实现键值对存储
     使用btree实现key-revision检索



etcd concurrency 选举机制
       1:  session就是一个租约
       2:  key的格式 /prefix/leaseId,监控/prefix下的key,取出创建时间最早的key作为Leader.
       3： 将key和leaseId绑定，lease过期，删除key 
            1> master节点      keepalive退出，然后关闭session,
            2> 其他slave节点会 其他slave节点监控到key变化后，再次选出leader.

???
  etcd does not ensure linearizability for watch operations. Users are expected to verify the revision of watch responses to ensure correct ordering.

sort  get 排序，字符比较，可以比较key, version, revisions 



![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-10-20-45-12-image.png)



![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-10-20-45-35-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-10-20-45-49-image.png)

1. cfssl 用于签名、验证、捆绑    tls

2. mkdir ssl

3. cd ssl

4. cat <<EOF| tee ca-csr.json
   
   ```json
   {
       "signing":{
       "default":{
           "expiry":"87600h"
       },
       "profiles":{
           "etcd":{
               "expiry":"87600h",
               "usages":[
                   "signing",
                   "key encipherment",
                   "server auth",
                   "client auth"
                ]
           }
       }
       }
   }
   EOF
   ```

    5. cfssl gencert -initca ca-csr.json | cfssljson-bare ca

    6. 配置ca 并创建tls 证书

```json
cat <<EOF| tee etcd-server-csr.json
{
    "CN":"etcd",
    "host":[
        "xxx",
        "xxx2"
    ],
    "key":{
        "algo":"rsa",
        "size":2048
    },
    "names":[
    {
        "C":"CN",
        "L":"Beijing",
        "ST":"Beijing"
    }
    ]
}
EOF
```

cfssl gencert  -ca=ca.pem -ca-key=ca-key.pem -config=ca-config.json -profile=etcd-server-csr.json | cfssljson-bare server

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-10-15-57-21-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-10-15-58-52-image.png)







![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-10-20-49-51-image.png)





![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-10-20-57-49-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-10-20-58-11-image.png)
