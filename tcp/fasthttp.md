
#fasthttp
* Request 
		1. URI 需要设置的url,对于要设置参数的GET请求，可以设置queryString,也可以设置args
		2. Args postArgs,不需要设置，解析body得到的
		3. RequesHead 设置http key-value的head需要的

*RequestCtx
	1. PostArgs , QueryArgs,分别代表uri和body里的提交查询参数
	2. FormValue 从PostArgs和QueryArgs里查询某个字段的值

