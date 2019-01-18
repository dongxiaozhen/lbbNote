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
