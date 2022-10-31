空间种类67个级别，按大小。
mspan  一个地址空间。
mcache  每个线程一个mcache,一个mcache管理大小为67的mspan数组
mcentral 管理空闲或者非空mspan
mheap  地址空间总管，有67个mcentral,


mallocgc
    if size <  32K {
        if noscan && size < 16B{
            // 微对象，从mcache.tiny获取
        }else{
            // 从mcache.alloc(span 获取)
        }
    }else{
        // 大对象，从mheap 获取
    }




介绍go内存
英文：https://deepu.tech/memory-management-in-golang/
中文: https://tonybai.com/2020/03/10/visualizing-memory-management-in-golang/

介绍程序的内存管理,垃圾回收
   https://deepu.tech/memory-management-in-programming/

