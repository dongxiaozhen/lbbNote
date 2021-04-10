

#注意事项
1. 服务入k8s后，需要设置goroutine数量上限，需要设置CPUNUM(gc时间长，等待其他process stw)
    * http://www.dockone.io/article/9387 


2 使用https://github.com/uber-go/automaxprocs 的限制process 数量
