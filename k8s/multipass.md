# https://www.bilibili.com/video/BV1Ed4y1u7L3?p=5&vd_source=76d419f44ea47048972c58770cf4cc52

multipass launch --name k8smaster --cpus 4 --mem 8192M --disk 30G 20.04
multipass launch --name k8snode1 --cpus 2 --mem 4096M --disk 20G 20.04
multipass launch --name k8snode2 --cpus 2 --mem 4096M --disk 20G 20.04

1. 安装docker 
    1.1 安装 apt install docker.io
    1.2 修改docker ,systemd 启动
        cat > /etc/docker/daemon.json  <<EOF
        {
        "registry-mirrors":[
        "https://docker.mirrors.ustc.edu.cn",
        "https://hub-mirror.c.163.com",
        "https://reg-mirror.qiniu.com",
        "https://registry.docker-cn.com"
        ],
        "exec-opts":["native.cgroupdriver=systemd"]
        }
        EOF
    1.3 重启
         systemctl restart docker.service

2. 安装kubeadm,kubectl,kublet // https://developer.aliyun.com/mirror/kubernetes
    apt-get update && apt-get install -y apt-transport-https
    cat <<EOF >/etc/apt/sources.list.d/kubernetes.list
    deb https://mirrors.aliyun.com/kubernetes/apt/ kubernetes-xenial main
    EOF
    apt-get update
    apt-get install -y kubelet kubeadm kubectl

3. 安装 cri-docker
    3.1 下载arm版本  https://github.com/Mirantis/cri-dockerd
    3.2 下载src
    3.3 cp /learn_go/cri-dockerd/cri-dockerd /usr/local/bin/cri-dockerd
    3.4 cp /learn_go/cri-dockerd/systemd/* /etc/systemd/system
    3.5 sed -i -e 's,/usr/bin/cri-dockerd,/usr/local/bin/cri-dockerd,' /etc/systemd/system/cri-docker.service
    3.6 systemctl daemon-reload
    3.7 systemctl enable cri-docker.service
    3.8 systemctl enable --now cri-docker.socket
    3.9 修改 /etc/systemd/system/cri-docker.service
        ExecStart=/usr/local/bin/cri-dockerd --container-runtime-endpoint fd:// --pod-infra-container-image registry.aliyuncs.com/google_containers/pause:3.7

4. 拉取k8s
    kubeadm config images list
    kubeadm config images list --image-repository registry.aliyuncs.com/google_containers
    kubeadm config images pull --image-repository registry.aliyuncs.com/google_containers --cri-socket unix:///var/run/cri-dockerd.sock

5. 启动
    kubeadm init --pod-network-cidr=10.244.0.0/16 --service-cidr=10.96.0.0/12 --image-repository registry.aliyuncs.com/google_containers --cri-socket unix:///var/run/cri-dockerd.sock --upload-certs

6. 安装flannel // https://github.com/flannel-io/flannel
    kubectl apply -f https://raw.githubusercontent.com/flannel-io/flannel/master/Documentation/kube-flannel.yml




