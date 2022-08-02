#book
    https://course.rs/first-try/installation.html
    
#command
	cargo new greeting
	cargo build 
	cargo run 
    cargo clippy: 类似eslint，lint工具检查代码可以优化的地方
    cargo fmt: 类似go fmt，代码格式化
    cargo tree: 查看第三方库的版本和依赖关系
    cargo bench: 运行benchmark(基准测试,性能测试)
    cargo udeps(第三方): 检查项目中未使用的依赖
    cargo check是我们在代码开发过程中最常用的命令，它的作用很简单：快速的检查一下代码能否编译通过. 因此该命令速度会非常快，能节省大量的编译时间.

另外 cargo build/run --release 使用 release 编译会比默认的 debug 编译性能提升 10 倍以上，但是 release 缺点是编译速度较慢，而且不会显示 panic backtrace 的具体行号



let mut a = 123; // let 定义变量， mut 定义可变变量
重影就是刚才讲述的所谓"重新绑定
元组用一对 ( ) 包括的一组数据，可以包含不同种类的数据：
数组用一对 [ ] 包括的同类型数据。




 所有权有以下三条规则：

    Rust 中的每个值都有一个变量，称为其所有者。
    一次只能有一个所有者。
    当所有者不在程序运行范围时，该值将被删除。


    引用不会获得值的所有权。

引用只能租借（Borrow）值的所有权。

    let s2 = &mut s1;
    // s2 是可变的引用

    Rust 中有两种常用的字符串类型：str 和 String。str 是 Rust 核心语言类型，就是本章一直在讲的字符串切片（String Slice），常常以引用的形式出现（&str）
