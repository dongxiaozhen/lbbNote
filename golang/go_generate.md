命令替换 foo = go tool foo
//go:generate -command foo go tool foo


//go:build race  说明此文件编译到时候需要是 go build -trace 才会生效
//go:build !race 说明：go build 生效

linkname
 //go:linkname是初看go源码常见的一个编译器指示，因为有时候你跟着跟着就发现函数只有声明没有函数体，也没有汇编实现。
 //go:linkname localname linkname. Using this its possible to introduce new symbols for linkage (export), or bind to existing symbols (import).
 // export 导出，将本地函数导出到 remote
 // import 导入，将remote函数导入到本地函数, 使用导入功能必须引入 unsafe包，提供.s 文件Provide .s file stub to allow compiler to bypass -complete check to allow partially defined functions
noescape
 //go:noescape指示后面必须跟没有主体的函数声明(意味着该函数具有非Go编写的实现)，它指定函数不允许作为参数传递的任何指针逃逸到堆中或函数返回值中。编译器在对调用该函数的Go代码进行逃逸分析时，可以使用此信息。
noinline
 inline是编译期将函数调用处替换为被调用函数主体的一种编译优化手段,//go:noinline意思就是不要内联。
cgo_import_dynamic

generate
nosplit
 //go:nosplit作用是跳过栈溢出检测。
norace
 //go:norace表示禁止进行竞态检测。它指定竞态检测器必须忽略函数的内存访问。除了节约了点编译时间没发现啥其他好处。
uintptrescapes
yeswritebarrierrec
nowritebarrierrec
//go:nowritebarrierrec告诉编译器当前函数及其调用的函数（允许递归）直到发现//go:yeswritebarrierrec为止，若期间遇到写屏障则触发一个错误。

nowritebarrier
 //go:nowritebarrier告诉编译器如果跟着的函数包含写屏障则触发一个错误，但并不会阻止写屏障的生成。
cgo_unsafe_args
notinheap
 //go:noinheap适用于类型声明，表示一个类型必须不能分配到GC堆上。好处是runtime在底层结构中使用它来避免调度器和内存分配中的写屏障以避免非法检查或提高性能。
systemstack
 //go:systemstack表示函数必须在系统栈上运行。
cgo_import_static
cgo_ldflag
cgo_export_dynamic
cgo_export_static
nocheckptr


