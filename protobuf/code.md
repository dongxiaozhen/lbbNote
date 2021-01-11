
格式化：
field_num|field_type field_encode_bytes


1 可变长度数值类型是 1<<7 位循环读取的，第八位标示是否有结束
2 field_num 不要设置太大



1 proto 利用反射将结构体信息缓存到内存中，主要是字段的解析函数，结构体偏移量、
2 解析的过程就是利用保存下来的Properties 递归解析。
3 内部结构体标示是bytes, 




example
----------解析结构体
```go
    infoId := &FlushResourceInfo.ResourceInfoID{}
	infoId.UserId = proto.Uint64(1)
	infoId.ResourceInfo = &FlushResourceInfo.ResourceInfo{}
	infoId.ResourceInfo.WorldId = proto.Uint64(1)

```go
    marshal后数据 [8 1 18 2 16 1]
    8 8&0x7==0 类型是可变长度整数 8>>3 == 1 tag是1的字段
    1   数值是1
    18  18&0x7==2 类型是字符串 18>>3 == 2 tag是2的字段
    2   字符串的长度
    16  16&0x7==0 类型是可变长度整数 16>>3 == 2 tag是2的字段
    1  数值是1

------------数组解析例子
```go
	mm := &MailBoxBase.AnnexItem{}
	mm.Items = append(mm.Items, &Common.Item{ItemId: proto.Int32(1)})
	mm.Items = append(mm.Items, &Common.Item{ItemId: proto.Int32(2)})
	mm.Items = append(mm.Items, &Common.Item{ItemId: proto.Int32(3)})
```
    [26 2 8 1 26 2 8 2 26 2 8 3]
    26 26&0x7==2 类型是字符串 26>>3 == 3 tag是3的字段
    2 字符串长度
    8 8&0x7==0 类型是可变长度整数 8>>3 == 1 tag是1的字段
    1 数值1
    26 26&0x7==2 类型是字符串 26>>3 == 3 tag是3的字段
    2 字符串长度
    8 8&0x7==0 类型是可变长度整数 8>>3 == 1 tag是1的字段
    2 数值2
    .
    .


