run

elasticsearch
   全面介绍 https://www.elastic.co/start

elasticsearch 安装
    https://www.elastic.co/guide/en/elasticsearch/reference/current/zip-targz.html

   ./bin/elasticsearch -d -Ecluster.name=my_cluster -Enode.name=node_1

kibana 安装
    https://www.elastic.co/guide/en/kibana/current/targz.html

   ./bin/kibana

elasticsearch-head
    文档说明：https://github.com/mobz/elasticsearch-head

    git clone git://github.com/mobz/elasticsearch-head.git
    cd elasticsearch-head
    npm install
    npm run start
    open http://localhost:9100/ 

文档：
   介绍查询索引原理  https://www.cnblogs.com/dreamroute/p/8484457.html

关闭https security

    config/elasticsearch.yml  

        xpack.security.enabled: true. --> false

概念介绍

bulk
     type:
         index: 创建，每次都是创建新的doc
         create: 创建，如果已经存在会返回错误
         update: 更新，更新字段，不会删除字段
             doc_as_upsert: 如果不存在，使用doc 插入数据，相当于upsert=doc

                    upsert: 如果不存在，插入数据



开发模式和生产模式

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-15-16-10-18-image.png)

角色

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-15-18-37-54-image.png)

index

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-15-18-55-45-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-10-19-59-56-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-15-22-24-11-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-15-22-33-17-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-10-19-56-07-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-10-19-59-00-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-15-22-44-26-image.png)

写流程

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-10-20-10-00-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-10-22-31-57-image.png)

写参数

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-10-22-32-45-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-10-22-33-55-image.png)

读流程

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-10-22-37-22-image.png)

数据流程

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-10-23-01-28-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-10-23-08-50-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-13-11-37-39-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-13-11-39-03-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-14-16-53-31-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-14-16-50-19-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-14-16-51-09-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-14-17-23-23-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-14-17-29-29-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-14-17-44-01-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-14-17-50-46-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-14-17-51-20-image.png)

索引模版

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-14-17-53-53-image.png)

sql

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-14-18-26-24-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-14-18-27-48-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-14-18-29-07-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-14-18-30-28-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-14-18-47-19-image.png)

## mapping 映射

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-16-11-41-55-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-16-11-49-22-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-16-12-23-44-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-16-12-24-33-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-16-12-30-04-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-16-12-30-51-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-16-12-29-11-image.png)

## mapping params

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-16-12-38-46-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-16-12-42-04-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-16-12-43-08-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-16-12-47-05-image.png)

term查询是基于词项的查询,而且当设置为term查询时,es不会对这个词做任何处理

match 和match_pharse查询都是属于全文查询,全文查询会给当前的句子进行分词,通常来讲,索引的时候咋分的词,查询的时候就是用的什么分词器,默认是不用设置的,但是如果有个别场景,也可以自己设置分词器,ok,他两的区别举个例子,"I am Pantheon",这句话如果用match来查,会搜出字段中有"I","am","Pantheon"的文档,也就是说默认是只要这段话中有这查询几个词的一个就返回文档,当然也可以设置他的'operator'属性为'and',要求查出来的必须包含"I am Pantheon"这几个词",而match_pharse可以精确的查询"I am Pantheon"这一段话,这几个词之间有没有间隔可以通过slot属性来设置,比如设置了slot为1,那就可以查出"I am not Pantheon"这样的句子,

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-17-16-39-05-image.png)

切词器 tokenizer, 包括 standard

词项过滤器：filter, 包括 uppercase， stop, 

字符过滤器： 分词之前的预处理，过滤无用的字符, 包括html_strip(删除html标签)，

分词器analyzer  是切词器和过滤器的组合，有 english, 

    standard 默认分词器，中文支持不理想

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-05-17-17-59-34-image.png)

## aggs

    count 计数 field, count(field)
    cardinality 统计去重, 计算不同值的近似计算， 使用HyperLogLog++ (HLL)算法
    avg 平均值
    max 最大值
    min 最小聚合
    sum  求和，field, sum(field)
    value_count 值计数统计
    percentiles 百分比集合，多值度量聚合
    
    
    histogram 直方图聚合，多分组聚合，可以设置间隔
    date_histogram 日期直方图，多分组聚合，可以设置间隔（year, month, week,day,,）， 设置time_zone
    date_range 时间范围聚合，设置to, from,返回 --to, from-now 两个范围
    range 范围聚合， 设置多个from-to
    terms 索引词聚合，展示 field, count

### cardinality

1. cardinality 并不是像关系型数据库 MySQL 一样精确去重的，cardinality做的是一个近似值，是 ES 帮你"估算"出的，这个估算使用的 HyperLogLog++ (HLL)算法，在速度上非常快，遍历一次即可统计去重，具体可看文档中推荐的论文。
2. ES 做cardinality估算，是可以设置估算精确度，即设置参数 precision_threshold 参数，但是这个参数在 0-40000, 这个值越大意味着精度越高，同时意味着损失更多的内存，是以内存空间换精度。
3. 在小数据量下，ES 的这个"估算"精度是非常高的，几乎可以说是等于实际数量。
