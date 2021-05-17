生成秘钥 ssh-keygen
交换秘钥 ssh-copy-id user@host

-R参数指定远程端口转发
    $ ssh -R 9999:targetServer:902 local
    上面命令需在跳板服务器执行，指定本地计算机local监听自己的 9999 端口，所有发向这个端口的请求，都会转向 targetServer 的 902 端口

本地端口转发
    ssh -L 2121:host2:21 host3
        指定SSH绑定本地端口2121，然后指定host3将所有的数据，转发到目标主机host2的21端口



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
