docker login
docker images
docker search 命令来查找官方仓库中的镜像

Dockerfile

    使用#来注释
    FROM 指令告诉 Docker 使用哪个镜像作为基础
    接着是维护者的信息
    RUN开头的指令会在创建中运行，比如安装一个软件包，在这里使用 apt-get 来安装了一些软件
    ADD 命令复制本地文件到镜像；
    EXPOSE 命令来向外部开放端口；
    CMD 命令来描述容器启动后运行的程序等。
    VOLUME 来添加一个或者多个新的卷到由该镜像创建的任意容器

    sudo docker build -t="ouruser/sinatra:v2" .
        -t 标记来添加 tag，指定新的镜像的用户信息。 “.” 是 Dockerfile 所在的路径（当前目录）


docker tag 命令来修改镜像的标签。

docker commit -m "Added json gem" -a "Docker Newbee" 0b2616b0e5a8 ouruser/sinatra:v2
docker import 要从本地文件系统导入一个镜像		 sudo cat ubuntu-14.04-x86_64-minimal.tar.gz  |docker import - ubuntu:14.04
docker push  把自己创建的镜像上传到仓库中来共享 Docker Hub			 sudo docker push ouruser/sinatra
docker save    导出镜像到本地文件 	 sudo docker save -o ubuntu_14.04.tar ubuntu:14.04
docker load 从导出的本地文件中再导入到本地镜像库 sudo docker load --input ubuntu_14.04.tar

docker run    新建并启动容器
    -t 选项让Docker分配一个伪终端（pseudo-tty）并绑定到容器的标准输入上， -i 则让容器的标准输入保持打开。
	--volumes-from name 挂载name docker的所有容器
    --link 参数可以让容器之间安全的进行交互 --link name:alias 与容器name连接,不用暴露端口
	-p 暴露端口
	-h 容器操作系统命名
	--name 容器命名
docker stop 停止一个容器,(停止的容器还在，可以用start命令再次激活，如果不用，需要用rm命令删除)
docker rm 命令是移除容器。
docker attach docker_name 连接到前一个docker命令的Stdin,  
docker exec -it docker_name cmd  运行另一个命令

docker ps 命令来查看容器运行情况信息
docker inspect 查看容器所有信息
docker port 查看导出的post映射	
docker volumn 查看卷
docker top 查看容器进程
docker logs 要获取容器的输出信息

docker export    导出本地某个容器
    sudo docker export 7691a814370e > ubuntu.tar    (这样将导出容器快照到本地文件。)

docker import     从容器快照文件中再导入为镜像
    cat ubuntu.tar | sudo docker import - test/buntu:v1.0


    -d 守护态运行
    -m 来指定提交的说明信息，跟我们使用的版本控制工具一样；
    -a 可以指定更新的用户信息；
    -v 标记来创建一个数据卷并挂载到容器里, 默认权限是读写,可以通过 :ro 指定为只读。  -v /src/webapp:/opt/webapp:ro
    -P 或 -p 参数来指定端口映射。
    --name 标记可以为容器自定义命名。
    --link 参数可以让容器之间安全的进行交互 --link name:alias 与容器name连接

    之后是用来创建镜像的容器的 ID；最后指定目标镜像的仓库名和 tag 信息。创建成功后会返回这个镜像的 ID 信息。
