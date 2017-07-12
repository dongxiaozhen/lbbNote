生成秘钥 ssh-keygen
交换秘钥 ssh-copy-id user@host

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

