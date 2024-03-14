# with



## 1. with语句作用



with语句是Python引入的一种与异常处理相关的功能，它可以看做是对try...finally语句的简化

with语句适用于对资源进行访问的场合，它可以确保在对资源的操作过程中不管是否发生异常都会自动执行释放资源的操作

## 2.  with语句语法格式

````
with expression [as variable]:
    with-block
````



3. 上下文管理器

   开发中，资源操作(文件、数据库等)都是很常见的操作，我们在使用这些资源后必须要将其释放，不然很容易造成资源泄露，轻则使得系统处理缓慢，重则会使系统崩溃；为了解决这个问题，python提供的解决方式就是try...finally和上下文管理器，上下文管理器是用来简化try...finally的，它能够帮我们自动分配并释放资源

   上下文管理器：实现了上下文管理器协议的类的实例对象


4. ## 上下文管理器协议

   - __enter__()：上文方法，负责进入运行时上下文的准备操作，返回的值可以是上下文管理器对象，也可以是与运行时上下文相关的其他对象，如果with语句有as，会把其返回的值赋值给as后的变量上



   <br/>

   - __exit__()：下文方法，负责退出运行时上下文的清理操作，返回一个布尔值，这个布尔值标明如果with-block的退出是由异常引发的，该异常是否需要被忽略(返回值为False：这个异常将被重新引发一次；返回值为True：这个异常就会被忽略，继续执行后面的代码)



