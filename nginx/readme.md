# 线上配置
https://nginx-playground.wizardzines.com/

# 可视化配置
    https://www.digitalocean.com/community/tools/nginx?global.app.lang=zhCN


配置文档

    Alphabetical index of directives
    Alphabetical index of variables


全局配置
user 配置work 进程的用户组
worker_processes work进程数量，auto -> cpu数量
worker_cpu_affinity cpu 亲和
worker_priority 进程优先级，nice 优先序 (priority) 为作业系统用来决定 CPU 分配的参数，Linux 使用『回合制(round-robin)』的演算法来做 CPU 排程，优先序越高，所可能获得的 CPU时间就越多,负数优先级高
worker_rlimit_core 打开的文件数量,和ulimie 里的open files 配合使用
daemon off: 前台启动， on: 后台启动

events{}  处理连接请求配置
  worker_connections 单个worker进程最大并发量
  use epoll 
  accept_mutex on:  同一时刻一个请求轮流由woker进程处理，防止唤醒所有的worker进程，off: 唤醒所有的进程
  multi_accept on: 单个worker进程可以接受多个新的网络连接

http{} 
  include mime.types 导致支持的类型
  default_type 默认的文件类型
  charset uft-8 字符集
  server_token off:关闭header 里的server
  虚拟主机
    server{}
      listen 后端服务地址
      server_name m.xx.com
      root  配置根目录，可以放在server里，也可以放在location里
      location{}
        alias 别名，只能放在location里
        = 精确匹配
        ^～ 前缀匹配
        ~ 正则匹配
        ～* 不区分大小写

  allow 白名单 addr
  deny 黑名单addr
  (allow,deny 先小范围，然后大范围)

auth_basic 认证提示
auth_basic_user_file 密码文件，使用htpasswd 生成

error_page 404 =200 /empty.gif; 错误重定向，错误
try_files 尝试文件匹配


auto_index on:自动文件索引功能， 开启文件下载功能
limit_rate 下载速率


stub_status 显示nginx状态

htpasswd -c(create file) -b file user1 paswd1
