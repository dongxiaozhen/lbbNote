生成秘钥 ssh-keygen
交换秘钥 ssh-copy-id user@host

-R参数指定远程端口转发
    $ ssh -R sv1_port:sv2:sv2_port sv2
     在sv1上执行命令, 在sv2上创建监听sv2_port, 将流量导向sv1_port (sv1_port 是一个已经服务端口，可以提供服务), 相当于在sv2上访问sv2_port就能访问到sv1上sv1_port 服务

本地端口转发
    ssh -L sv1_port:sv2:sv2_port sv2
      在sv1上执行，将sv2上的sv2_port 映射到sv1_port上，访问sv1_port就相当于访问sv2:sv2_port

动态端口转发
    ssh -D 3000 remote_host
        发给本地端口3000的数据都转发到remote_host, 相当于sock5代理


-1，-2
    -1参数指定使用 SSH 1 协议。
    -2参数指定使用 SSH 2 协议

-4，-6
    -4指定使用 IPv4 协议，这是默认值
    -6指定使用 IPv6 协议。



 ssh 连接服务器不需要密码 
	将本机的公钥拷贝到服务器的~/.ssh/authorized_keys文件。

ssh 的配置文件 /etc/ssh/sshd_config
	Protocol 2

	PermitRootLogin no
	PermitEmptyPasswords no
	PasswordAuthentication no
	
	RSAAuthentication yes
	PubkeyAuthentication yes
	AuthorizedKeysFile .ssh/authorized_keys
	
	UseDNS no
	AllowUsers bill
	上面主要是禁止root用户登录，以及禁止用密码方式登录。指定允许登陆的用户。



在本机~/.ssh文件夹下创建config文件，内容如下,就可以通过ssh s1(ssh root@192.168.1.23)直接登陆,
	Host s1
	HostName 192.168.1.23
	User root
	Port 22
