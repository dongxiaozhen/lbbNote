ingress(七层)相当于service的service(四层)，可以将外部请求通过不同规则的筛选后转发到不同的service

为了使外部的应用能够访问集群内的服务，Kubernetes目前提供了以下几种方案:
    NodePort：将service暴露在节点网络上，NodePort背后就是Kube-Proxy，Kube-Proxy是沟通service网络、Pod网络和节点网络的桥梁。
        测试环境使用还行，当有几十上百的服务在集群中运行时，NodePort的瑞口管理就是个灾难。因为每个端口只能是一种服务，端口范围只能是 30000-32767。
    LoadBalancer：通过设置LoadBalancer映射到云服务商提供的LoadBalancer地址。这种用法仅用于在公有云服务提供商的云平台上设置Servic的场景。受限于云平台，且通常在云平台部署LoadBalancer还需要额外的费用。
    Ingress：只需一个或者少量的公网IP和LB，即可同时将多个HTTP服务暴露到外网，七层反向代理。

二、配置方式
1、Deployment+LoadBalancer模式的Service
如果要把ingress部署在公有云，那用这种方式比较合适。用Deployment部署ingress-controller，创建一个type为LoadBalancer的service关联这组pod。大部分公有云，都会为LoadBalancer的service自动创建一个负载均衡器，通常还绑定了公网地址。只要把域名解析指向该地址，就实现了集群服务的对外暴露

2、Deployment+NodePort模式的Service
同样用deployment模式部署ingress-controller，并创建对应的服务，但是type为NodePort。这样，ingress就会暴露在集群节点ip的特定端口上。由于nodeport暴露的端口是随机端口，一般会在前再搭建一套负载均衡器来转发请求。该方式一般用于宿主机是相对固定的环境ip地址不变的场景。
NodePort方式暴露ingress虽然简单方便，但是NodePort多了一层NAT，在请求量级很大时可能对性能会有一定影响

3、DaemonSet+HostNetwork+nodeSelector
用DaemonSet结合nodeselector来部署ingress-controller到特定的node上（也可结合affinity亲和性部署到多个节点），然后使用HostNetwork直接把该pod与宿主机node的网络打通，直接使用宿主机的80/433端口就能访问服务。这时，ingress-controller所在的node机器就很类似传统架构的边缘节点，比如机房入口的nginx服务器。该方式整个请求链路最简单，性能相对NodePort模式更好。缺点是由于直接利用宿主机节点的网络和端口，一个node只能部署一个ingress-controller pod。比较适合大并发的生产环境使用。

// 创建路由规则
kubectl create ingress demo-localhost --class=nginx --rule="hello.localdev.me/*=hello-nginx:80"
    demo-localhost 名称
    hello.localdev.me nginx的host
    / = hello-nginx:80  路由规则

// 查看路由规则
kubectl describe ingress nginx-ingress-test
