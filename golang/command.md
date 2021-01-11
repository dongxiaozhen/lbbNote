
go build 
	-a 强制对所有涉及到的代码包进行重构
	-v 打印出那些被编译的代码包的名字
	-work 打印出编译时生成的临时工作目录的路径

指定编译版本
go build  -gcflags "-lang=go1.9"

在编译过程中，Go进行了逃逸分析，以确定哪些可以放入栈（静态数据），哪些需要放入堆（动态数据）。我们可以通过运行带有
go build -gcflags '-m'

go vet 代码检查 
	--all

os.Exec
   vfork是为了解决fork拷贝页表项导致的性能问题, 而且大部分场景fork之后是调用exec，exec要把所有页表删除重置新的页表, 实在没必要再拷贝页表项。但由于vfork父子进程共享内存，所以使用要很小心，如果子进程修改某个变量，会影响到父进程，而且kernel会挂起父进程，让子进程先执行，这些限制基本限制vfork只适合跟exec的场景，不如fork通用。

查看go设置代理
   go help goproxy  
   设置GOPROXY 环境变量


包依赖解决
   go help modules
   go mod help 
     	download    download modules to local cache
	    edit        edit go.mod from tools or scripts
	    graph       print module requirement graph
	    init        initialize new module in current directory
	    tidy        add missing and remove unused modules
	    vendor      make vendored copy of dependencies
	    verify      verify dependencies have expected content
	    why         explain why packages or modules are needed




go build , go install
     新版本的gomode机制下载的文件都是只读的，所以，go build 是时候，会报 permission denied的错误提示，使用go install 代替


指定编译, 
    文件开头// +build x1,x2 y1,!y2 ((x1 and x2) 或 (y1 and !y2))
    后面有换行符
    go build -tags x1,x2
    
