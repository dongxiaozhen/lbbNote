https://morven.life/posts/k8s-api-1/

文档： https://kubernetes.io/zh-cn/docs/concepts/


aufs OverlayFS
cri -> containerd
oci -> runc
cni -> flannel


	pod
		自主式pop
		控制器管理的pod

	控制器 controller
		replicaset (rc/rs) 支持标签
		deployment 声明式 / 滚动升级，扩容，缩容
		daemonset 确保全部node 上运行一个pod副本
		statefulset 有序部署/收缩，稳定的网络标志，稳定的持久化存储
		job/cronjob
		horizontal pod autoscaling

	标签
		pod 数量是统计相同标签的pod 有多少个

	service
		clusterip 使用ipvs，将cluster_ip 映射到后端, 使用iptable实现的，将虚拟ip 映射到后端的pod容器
		nodeport ,使用ipvs ，将每个node的特定端口映射到后端
		loadbalance  比nodeport多了一个lb 
		externalname 

	ingress 七层代理

	configmap
	secret
		service account
		opaque  base64编码
		dockerconfigjson 存储 docker regist 的认证信息
	volumn
		emptydir
		hostpath

	persistentvolumn


	调度
		prediccate 过滤不满足条件的node
		priorities 优选
		亲和性
			nodeAffinity node 亲和性
			podAffinity podAntiAffinity pod 亲和性
		排斥
			taint 污点
				noschedule k8s 不会将pod 调度到该node
				prefernoschedule k8s 尽量调度到该node
				noexecute k8s 不会将pod 调度到该node,如有有pod就删除

			toleration 容忍
		固定节点
			nodename
			nodeselector



    kubectl expose deployment hello-nginx --type=LoadBalancer --port=80
    kubectl expose deployment hello-nginx --type=NodePort --port=80   // node 网络可以访问cluster_ip
    kubectl expose deployment hello-nginx --type=ClusterIP --port=80  // 只是有cluster_ip


namespace
    kubectl get ns
    kubectl get namespaces


cmd
    kubectl [command] [TYPE] [NAME] [flags]

    command部分描述了要执行的操作类型，主要的操作类型有：
        create 从文件或命令行输入提供的配置，生成资源对象。
        describe 检索资源对象的详细信息
        get 获取各种资源在集群里的信息
        delete 从集群中删除需要擦除的资源对象
        apply 搭配声明文件使用，把资源对象的定义提交给集群，由集群进行应用。
    TYPE 部分用于制定 kubectl 发起的操作，所针对的资源类型。常用的资源类型有pod，service，deployment， statefulset ，node这些。
    NAME 部分区分大小写，是K8s里资源对象的唯一标识，用于制定指定TYPE部分指明的相关资源的名称。将名称附加到命令操作上会将该命令操作只对该资源对象游泳
    flags 部分表示对特定资源的特殊选项或请求。它们是用作覆盖默认值或环境变量
        -n xx 指定命名空间
        --all-namespaces 全部命名空间
        —field-selector 能根据资源的属性查出各种在某个状态、拥有某个属性值的资源。
    
kubectl apply -f resources.yaml --record
    让K8s记住每个版本都提交了什么，这个功能可以通过--record选项开启,用K8s中-- Deployment资源的回滚能力

