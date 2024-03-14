app
	el 入口
	data 数据
	methods 方法
	components 组件

v-bind 
	js->html 单向数据绑定

v-model  
	js <-> html 双向数据绑定


v-for 循环
	{{ index }} {{ item }} 数组
	{{ key }}  {{ value }} 字典

v-on 事件
	click
	mouseover
	change

v-if  条件为false的时候不会添加元素
v-show 条件为false的时候会添加元素，但不会渲染


axios http网络
	method
	url
	params
	data
	headers

	then 成功回调函数
	catch 失败回调函数


组件
	data
	template
	method
	created 页面创建的时候出发，可以用来查询数据， 不能操作DOM	
		this.$route.params
		this.$route.query 参数传递

	mounted 可以操作DOM
	watch 监听router数值变化

vue-router 单页面展示多功能页
	router-link 
		to 页面
			path 基于path
			name 基于name
			params
			query

	router-view 页面位置
	VaeRouter 路由
		routes 
			path 
			component
			children 内部且套组件


() => 箭头函数，不会修改this的含义




template
	在模板中使用变量是利用 $ 符号
		{{ $obj := 数据 }}

	循环中使用其他变量需要使用$ $.xx

	去空白字符 
		{{- -}}

	修改默认标识符
		需要使用Delims方法


