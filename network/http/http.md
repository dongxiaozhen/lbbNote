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

````
Access-Control-Allow-Headers
Access-Control-Allow-Methods
Access-Control-Allow-Origin
Access-Control-Allow-Credentials // 允许浏览器读取response
````





## Server Sent Event(SSE) 务器发送事件

SSE 一般只用来传送文本，二进制数据需要编码后传送，WebSocket 默认支持传送二进制数据

每一次发送的信息，由若干个message组成，每个message之间用\n\n分隔。每个message内部由若干行组成，每一行都是如下格式。

[field]: value\n
上面的field可以取四个值。
  data
  event
  id
  retry

https://www.ruanyifeng.com/blog/2017/05/server-sent_events.html

服务器实现：

```go
func eventsHandler(w http.ResponseWriter, r *http.Request) {
 // Set CORS headers to allow all origins. You may want to restrict this to specific origins in a production environment.
 w.Header().Set("Access-Control-Allow-Origin", "*")
 w.Header().Set("Access-Control-Expose-Headers", "Content-Type")

 w.Header().Set("Content-Type", "text/event-stream")
 w.Header().Set("Cache-Control", "no-cache")
 w.Header().Set("Connection", "keep-alive")

 // Simulate sending events (you can replace this with real data)
 for i := 0; i < 10; i++ {
  fmt.Fprintf(w, "data: %s\n\n", fmt.Sprintf("Event %d", i))
  time.Sleep(2 * time.Second)
  w.(http.Flusher).Flush()
 }

 // Simulate closing the connection
 closeNotify := w.(http.CloseNotifier).CloseNotify()
 <-closeNotify
}
```

注意事项：数据发送开头标志 <data: > 

 客户端：

````
<!DOCTYPE html>
<html>
<head>
    <title>SSE Example</title>
</head>
<body>
    <div id="sse-data"></div>

    <script>
        const eventSource = new EventSource('http://localhost:8080/events');
        eventSource.onmessage = function(event) {
            const dataElement = document.getElementById('sse-data');
            dataElement.innerHTML += event.data + '<br>';
        };
    </script>
</body>
</html>
````

