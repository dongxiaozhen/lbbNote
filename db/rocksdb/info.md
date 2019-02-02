#介绍
1. 在leveldb的基础上开的新的数据库
2. 与leveldb的不同
  1. 多线程compation
  2. rocksdb 其实使用了两步：先做 fillrandom，再做 compact,leveldb 只是使用了一步：fillrandom  -- 不懂？？？


#链接
1. https://rocksdb.org.cn/doc/Home.html


go版本的实现
    go get github.com/dgraph-io/badger/...
