

文章
 1 存储相关的
   https://www.cnblogs.com/vovlie/p/7709312.html?utm_source=debugrun&utm_medium=referral
   https://www.jianshu.com/p/93412a925da2


基础
https://www.ctolib.com/docs/sfile/prometheus-book/promql/prometheus-metrics-types.html
https://www.zhihu.com/topic/20223143/hot

Counter：只增不减的计数器
    例如，通过rate()函数获取HTTP请求量的增长率：
        rate(http_requests_total[5m])
    查询当前系统中，访问量前10的HTTP地址：
        topk(10, http_requests_total)

Gauge：可增可减的仪表盘
    对于Gauge类型的监控指标，通过PromQL内置函数delta()可以获取样本在一段时间返回内的变化情况。例如，计算CPU温度在两个小时内的差异：
        delta(cpu_temp_celsius{host="zeus"}[2h])
    还可以使用deriv()计算样本的线性回归模型，甚至是直接使用predict_linear()对数据的变化趋势进行预测。例如，预测系统磁盘空间在4个小时之后的剩余情况：
        predict_linear(node_filesystem_free{job="node"}[1h], 4 * 3600)

Histogram和Summary主用用于统计和分析样本的分布情况
Histogram通过histogram_quantile函数是在服务器端计算的分位数。 而Sumamry的分位数则是直接在客户端计算完成

PromQL的时间范围选择器支持其它时间单位：
s - 秒
m - 分钟
h - 小时
d - 天
w - 周
y - 年

内置函数
increase(v range-vector)函数是PromQL中提供的众多内置函数之一
    increase(node_cpu[2m])
rate函数可以直接计算区间向量v在时间窗口内平均增长速率
    rate(node_cpu[2m])
irate同样用于计算区间向量的计算率，但是其反应出的是瞬时增长率
    irate(node_cpu[2m])

Histogram的分位数计算需要通过histogram_quantile(φ float, b instant-vector)函数进行计算。其中φ（0<φ<1）表示需要计算的分位数
histogram_quantile(0.5, http_request_duration_seconds_bucket)


最大值
histogram_quantile(0.99, sum(rate(paipai_server_method_histogram_bucket{service=~"^($service)$",cluster=~"^($cluster)$",group=~"^($group)$",method=~"^($method)$"}[2m])) by(cluster,group,method,le))

请求量
sum by (group) (rate(paipai_server_method_histogram_count{service=~"^($service)$",cluster=~"^($cluster)$",group=~"^($group)$",method=~"^($method)$"}[1m]))

平均
rate(paipai_server_method_histogram_sum{service=~"^($service)$",cluster=~"^($cluster)$",group=~"^($group)$",method=~"^($method)$",shardingId=~"^($shardingId)$"}[1m])/rate(paipai_server_method_histogram_count{service=~"^($service)$",cluster=~"^($cluster)$",group=~"^($group)$",method=~"^($method)$",shardingId=~"^($shardingId)$"}[1m])

