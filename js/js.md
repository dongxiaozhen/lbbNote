iterable型，即使用for (var x of ..)的类型有set, map, array
function arguments获取可变参数，也可以使用 (a,b,...rest)
变量提升，即代码快后面定义的变量，编译器会在自动提升变量声明到代码块前面
let 变量声明，使在function ff(){ for (let x = 0;...) {...}..}，在花括号里可以访问x，ff范围里不能访问。
对象的方法中的this,只有在obj.xxx()形式的调用中才会正确。解决方法有两种，提起this到新的变量，然后使用新变量来表示，另一种事调用apply,call‘
array高阶函数，map, reduce(前面的运算结果作为后面运算的第一个参数),filter(true,false), sort(-1,0,1)
generator, yield
RegExp
	1. /正则表达式/
    2. new RegExp('正则表达式')

DOM
	1. getEventByID,name,,
 	2. append,,
	3. remove,,
	4. value, checked

XMLHttpRequest
	设置回调，检查readyState,status,

CORS 跨域请求
	Access-Control-Allow-Origin 服务器返回的参数

jquery        $本质上是函数
	$('#id'), $('p'), $('.red') $([name == 'xx'])
	$('div' 'p')
	$('div'>'p')
	:input, :checkbox, :radio, :checked, 	
	text, html
	append after remove
	$.ajax

	$(function(){....})  document对象的ready事件处理函数
	数组 var cars=new Array() / var cars=['benchi','bmw','volvo']
	对象 ar person={firstname:"Bill", lastname:"Gates", id:5566};
