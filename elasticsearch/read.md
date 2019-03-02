
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
