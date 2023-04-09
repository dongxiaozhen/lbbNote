runc 是一个命令行客户端，用于运行根据 Open Container Initiative (OCI) 格式打包的应用程序，并且是 Open Container Initiative 规范的兼容实现。
runc 希望提供一个“ OCI 包”，它只是一个根文件系统和一个config.json 文件

```perl
 create the bundle
$ mkdir -p /mycontainer/rootfs

# [ab]use Docker to copy a root fs into the bundle
$ docker export $(docker create busybox) | tar -C /mycontainer/rootfs -xvf -

# create the specification, by default sh will be the entrypoint of the container
$ cd /mycontainer
$ runc spec

# launch the container
$ sudo -i
$ cd /mycontainer
$ runc run mycontainerid

# list containers
$ runc list

# stop the container
$ runc kill mycontainerid

# cleanup
$ runc delete mycontainerid

```
