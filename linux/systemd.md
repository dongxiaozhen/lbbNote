https://systemd-by-example.com/


服务启动相关
systemctl enable/disable/status/stop/start xxx
journalctl -u xx 查看服务日志


查看服务依赖
systemctl list-dependencies xxx

查看进程服务启动时间
systemd-analyze blame 

systemctl
  systemctl status sshd.service
  systemctk list-units
  systemctl list-unit-files 
  systemctl cat sshd.service 查看配置文件路径和信息
  systemctl edit sshd.service 编辑文件



