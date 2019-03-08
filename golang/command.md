
go build 
	-a 强制对所有涉及到的代码包进行重构
	-v 打印出那些被编译的代码包的名字
	-work 打印出编译时生成的临时工作目录的路径

指定编译版本
go build  -gcflags "-lang=go1.9"

go vet 代码检查 
	--all

os.Exec
   vfork是为了解决fork拷贝页表项导致的性能问题, 而且大部分场景fork之后是调用exec，exec要把所有页表删除重置新的页表, 实在没必要再拷贝页表项。但由于vfork父子进程共享内存，所以使用要很小心，如果子进程修改某个变量，会影响到父进程，而且kernel会挂起父进程，让子进程先执行，这些限制基本限制vfork只适合跟exec的场景，不如fork通用。
