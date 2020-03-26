
weechat官网
https://weechat.org/files/doc/stable/weechat_quickstart.en.html

安装 brew install weechat
    安装过程会失败，主要是权限问题。/usr/local/man 下需要访问目录的权限 chown -R libinbin:xxx tttt

操作流程
1 添加一个服务器端  
	/server add freenode chat.freenode.net
	/server add freenode 127.0.0.1/6667

2 设置聊天昵称
	/set irc.server.freenode.nicks "mynick,mynick2,mynick3,mynick4,mynick5"

3 设置用户信息
	/set irc.server.freenode.username "My user name"
	/set irc.server.freenode.realname "My real name"

4 设置自动链接
	/set irc.server.freenode.autoconnect on

5 链接服务器
	/connect freenode

6 加入会话
	/join #channel

7 退出会话
	/close

8 断开链接
	/disconnect	

9 weechat 会创建～/.weechat，记录链接信息，如果要开启多个终端使用weechat,需要在为为个weechat 创建一个目录
