Authorization 
	Authorization header的典型数据为"Authorization: Basic jdhaHY0="，其中Basic表示基础认证， jdhaHY0=是base64编码的"user:passwd"字符串。如果没有，或者用户密码不对，则返回http code 401页面给客户端。
	使用http auth的场景不会用cookie，也就是说每次都会送帐号密码信息过去。然后我们都知道base64编码基本上等于明文。这削弱了安全。

Origin:	Origin 指示了请求来自于哪个站点，只有服务器名，不包含路径信息，浏览器自动添加到http请求 Header 中，无需手动设置。Origin字段只存在于POST请求，而Referer则存在于所有类型的请求。
	1、添加 Origin 的情况
    同源请求：POST、OPTIONS、PUT、PATCH 和 DELETE请求都会添加Origin请求头，GET或HEAD请求不会添加Origin请求头。跨域请求：所有跨域请求(CORS)都会添加Origin请求头。

Referer 指示了请求来自于哪个具体页面，包含服务器名和路径的详细URL，浏览器自动添加到http请求 Header 中，无需手动设置。
	1、不会添加 Referer 的情况
    来源页面采用 file 或 data URI 协议；来源页面采用 HTTPS 协议，而请求页面采用 HTTP 协议；


cors 跨域 跨域资源共享。是一种机制
	它使用额外的 HTTP 头来告诉浏览器  让运行在一个 origin (domain) 上的Web应用被准许访问来自不同源服务器上的指定的资源。当一个资源从与该资源本身所在的服务器不同的域、协议或端口请求一个资源时，资源会发起一个跨域 HTTP 请求。

	Access-Control-Allow-Headers
	Access-Control-Allow-Methods
	Access-Control-Allow-Origin
	Access-Control-Allow-Credentials // 允许浏览器读取response

