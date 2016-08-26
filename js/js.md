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


json
	复合类型的值只能是数组或对象，不能是函数、正则表达式对象、日期对象。
	简单类型的值只有四种：字符串、数值（必须以十进制表示）、布尔值和null（不能使用NaN, Infinity, -Infinity和undefined）。
	字符串必须使用双引号表示，不能使用单引号。
	对象的键名必须放在双引号里面。

	JSON.stringify(obj,selectedprop,) 将一个值($1)的指定属性($2)按指定的显示规定($3)转为字符串,如果对象包含toJSON方法，则会使用该方法得到一个值
	JSON.parse(str,f)  将字符串转为对象,如果有$2参数，则会调用f函数。


Event 事件传播 捕获capture(进入)-->冒泡bubbling(离开)
	创建event event = new Event(typeArg, eventInit);  var ev = new Event("look", {"bubbles":true, "cancelable":false}); 事件对象是否冒泡,事件是否可以被取消
	event属性 
			  e.bubbles 是否冒泡，
			  event.eventPhase 事件目前所处的节点
			  		0，事件目前没有发生
					1，事件目前处于捕获阶段
					3，事件处于冒泡阶段
		      event.cancelable 事件是否可以取消
			  e.defaultPrevented 事件是否调用过preventDefault方法
			  e.currentTarget  当前所在的节点,==this
			  e.target 回触发事件的那个节点,可能是子节点
			  event.type 事件类型
			  e.detail  返回一个数值，表示事件的某种信息
			  event.timeStamp 返回一个毫秒时间戳，表示事件发生的时间
			  e.preventDefault() 取消浏览器对当前事件的默认行为
			  e.stopPropagation() 阻止事件在DOM其他节点中继续传播


	event 类型
			mouseover/mouseenter mouseout/mouseleaver mousemove mousedown mouseup mouseclick mousedbclick
			keydown keypress keyup
			进度事件 <img>、<audio>、<video>、<style>、<link> <ajax>, abort error load loadstart loadend progress timeout
			表单事件 表单成员：input select change 表单:reset submit
			文档事件 beforeunload unload load err cut copy paster focus blur
	1注册监听 1. target.addEventListener(type,listener,[usecapture])  document.addEventListener('click', hello, false);   通过此方法添加的函数里使用的this指向target
			 2. html的on-属性 <body onload="doSomething()">  el.setAttribute('onclick', 'doSomething()'); 缺点html javascript 没有分离,参数里使用的this是html对象
			 3. div.onclick = dosomthing; 缺点多次定义会覆盖前面的 ,函数里的this指向全局

    2删除监听 target.removeEventListener(type,listener,[usecapture])
	3手动触发 target.dispatchEvent(new Event('click'))
