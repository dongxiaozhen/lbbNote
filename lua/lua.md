中文网赞
	https://wiki.luatos.com


require
	运行指定的文件
	末尾不带扩展名
	目录层级用"."分割
	只会运行一次
	从package.path中的路径查找


元方法
	setmetatable 	
	__add +
	__sub
	__mul
	__div
	__mod
	__pow
	__unm

	__index 索引table[key], 当不是table或者 成员不存在时候调用


面向对象
	语法糖 t:add  t.add(t,10) 第一个参数不用写了


协程	coroutine, 单线程，并发
	coroutine.create 创建
	coroutine.resume 运行
	coroutine.yield 挂起，再次resume 从上次yield地方开始   
	coroutine.wrap 返回一个函数， 每次调用相当于调用resume
	coroutine.status co的状态， suspended, dead, running(携程内查询)
