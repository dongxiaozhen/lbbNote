#runtime内容文章
 https://www.purewhite.io/2019/11/28/runtime-hacking-translate/
 
 #go poller, go schedure,
 https://mp.weixin.qq.com/s/HNPeffn08QovQwtUH1qkbQ



 runtime 代码经常通过调用 systemstack、mcall 或者 asmcgocall 临时性的切换到系统栈去执行一些特殊的任务
如果要判断当前正在系统栈上执行还是用户栈上执行，可以使用 getg() == getg().m.curg。
堆上内存，gc 管理的
不被 gc 所管理的堆外内存
有三种方式可以申请堆外内存 sysAlloc persistentalloc fixalloc ,普遍来说，使用以上三种方法分配内存的类型都应该被标记为 //go:notinheap


schedule
sysmon
Go runtime 在程序启动的时候会创建一个独立的 M 作为监控线程，叫 sysmon ，这个线程为系统级的 daemon 线程，无需 P 即可运行， sysmon 每 20us~10ms 运行一次。sysmon 中以轮询的方式执行以下操作（如上面的代码所示）

调用 retake ，抢占长时间处于 _Psyscall 状态的 P。
