DNS

A: 地址记录
NS：域名服务器记录
MX：邮件记录（Mail eXchange），返回接收电子邮件的服务器地址。
CNAME：规范名称记录（Canonical Name），返回另一个域名，即当前查询的域名是另一个域名的跳转，详见下文。
PTR：逆向查询记录（Pointer Record），只用于从IP地址查询域名
   dig -x xxx.xxx.x.x(ip)
      
跟踪查询
  dig +trace math.stackexchange.com
指定查询  
  dig a/ns/mx com
  dig a/ns/mx stackexchange.com

其他工具
  host github.com
  host xxx.xxx.x.x(ip)
  nslookup
  whois github.com



  # URL的层次结构
  ![www.baidu.com](./dns_baidu.webp)
    com被称为一级域或顶级域，其他常见的顶级域还有cn，co等，baidu是二级域，www则是三级域


# DNS 原理 
    * 利用层级结构去拆分服务
    * 加入多级缓存


# /etc/resolv.conf 配置

# ![协议](./dns_protocol.webp) 基于udp
Transsaction ID是事务ID，对于一次请求和这个请求对应的应答，他们的事务ID是一样的，类似于微服务系统中的log_id。
flag字段是指标志位，有2个Byte，16个bit，需要关注的是QR，OpCode,  RCode。

    QR用来标志这是个查询还是响应报文，0是查询，1是响应。
    OpCode用来标志操作码，正常查询都是0，不管是域名查ip，还是ip查域名，都属于正常查询。可以粗暴的认为我们平时只会看到0。
    RCode是响应码，类似于HTTP里的404, 502 这样的status code。用来表示这次请求的结果是否正常。0是指一切正常。1是指报文格式错误，2服务域名服务器内部错误。

    Queries字段，是指你实际查询的内容。这里其实包含三部分信息，Name, Type, Class。
    Name可以放域名或者IP。比如你要查的是baidu.com这个域名对应的IP，那里面放的就是域名，反过来通过IP查对应的域名，那Name字段里放的就是IP。
    Type是指你想查哪种信息，比如你想查这个域名对应的IP地址是什么，那就是填A（address），如果你想查这个域名有没有其他别名，就填CNAME（Canonical Name）。如果你想查 xiaobaidebug@gmail.com对应的邮箱服务器地址是什么（比如 gmail.com），那就填MX（Mail Exchanger）。除此之外还有很多类型，下面是常见的Type表格。
    Class字段就比较有意思了，你可以简单的认为，我们只会看到它填IN （Internet）



# ![dhcp](./dhcp_net.webp)

    DHCP Discover：在联网时，本机由于没有IP，也不知道DHCP服务器的IP地址是多少，所以根本不知道该向谁发起请求，于是索性选择广播，向本地网段内所有人发出消息，询问"谁能给个IP用用"。
    DHCP Offer：不是DHCP服务器的机子会忽略你的广播消息，而DHCP服务器收到消息后，会在自己维护的一个IP池里拿出一个空闲IP，通过广播的形式给回你的电脑。
    DHCP Request：你的电脑在拿到IP后，再次发起广播，就说"这个IP我要了"。
    DHCP ACK：DHCP服务器此时再回复你一个ACK，意思是"ok的"。你就正式获得这个IP在一段时间（比如24小时）里的使用权了。后续只要IP租约不过期，就可以一直

    得到DHCP ACK之后,会先发三条ARP消息,无偿ARP（Gratuitous ARP），特点是它会把IP和mac地址都填好了，而且填的还是自己的IP和mac地址。
        目的有两个。
            一个是为了告诉本地网段内所有机子，从现在起，xx IP地址属于xx mac地址，让大家记录在ARP缓存中。
            另一个就是看下本地网段里有没有其他机子也用了这个IP，如果有冲突的话，那需要重新再走一次DHCP流程。


# 可以强行让电脑的en0网卡重新走一遍DHCP流程
    sudo ipconfig set en0 DHCP

# 协议 基于传输层UDP
