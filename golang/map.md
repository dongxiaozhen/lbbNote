使用数组解决冲突

```go
// A header for a Go map.
type hmap struct {
    // Note: the format of the Hmap is encoded in ../../cmd/internal/gc/reflect.go and
    // ../reflect/type.go. Don't change this structure without also changing that code!
    count int // # live cells == size of map.  Must be first (used by len() builtin)
    flags uint8
    B     uint8  // log_2 of # of buckets (can hold up to loadFactor * 2^B items)
    noverflow uint16 // approximate number of overflow buckets; see incrnoverflow for details // 溢出桶的数量
    hash0 uint32 // hash seed

    buckets    unsafe.Pointer // array of 2^B Buckets. may be nil if count==0. ----------------------------------------------->array bmap (hashkey低位为数组index,循环判断高位)
    oldbuckets unsafe.Pointer // previous bucket array of half the size, non-nil only when growing
    nevacuate  uintptr        // progress counter for evacuation (buckets less than this have been evacuated)                                                                                                       

    extra *mapextra // optional fields
}


// mapextra holds fields that are not present on all maps.
type mapextra struct {
	// If both key and elem do not contain pointers and are inline, then we mark bucket
	// type as containing no pointers. This avoids scanning such maps.
	// However, bmap.overflow is a pointer. In order to keep overflow buckets
	// alive, we store pointers to all overflow buckets in hmap.extra.overflow and hmap.extra.oldoverflow.
	// overflow and oldoverflow are only used if key and elem do not contain pointers.
	// overflow contains overflow buckets for hmap.buckets.
	// oldoverflow contains overflow buckets for hmap.oldbuckets.
	// The indirection allows to store a pointer to the slice in hiter.
	overflow    *[]*bmap //供桶数组 buckets 使用的溢出桶
	oldoverflow *[]*bmap // 扩容流程中，供老桶数组 oldBuckets 使用的溢出桶

	// nextOverflow holds a pointer to a free overflow bucket.
	nextOverflow *bmap // 下一个可用的溢出桶
}


```

```go
// A bucket for a Go map.
type bmap struct {
    tophash [bucketCnt]uint8 ------------------------------------------------------------>hashkey高位
    // Followed by bucketCnt keys and then bucketCnt values. ----------------------------------->key1/key2/key3/../key8/value1/value2/..../value8
    // NOTE: packing all the keys together and then all the values together makes the
    // code a bit more complicated than alternating key/value/key/value/... but it allows
    // us to eliminate padding which would be needed for, e.g., map[int64]int8.
    // Followed by an overflow pointer.
    
    //keys [bucketCnt]T
    //values [bucketCnt]T
    //overflow uint8
}
```

map range  // 每次循环遍历前都会先随机一个bucket，作为开始遍历的bucket

    // decide where to start
    r := uintptr(fastrand())
    if h.B > 31-bucketCntBits {
        r += uintptr(fastrand()) << 31
    }
    it.startBucket = r & bucketMask(h.B)

map 扩容机制的核心点包括：

（1）扩容分为增量扩容和等量扩容；

（2）当桶内 key-value 总数/桶数组长度 > 6.5 时发生增量扩容，桶数组长度增长为原值的两倍；

（3）当桶内溢出桶数量大于等于 2^B 时( B 为桶数组长度的指数，B 最大取 15)，发生等量扩容，桶的长度保持为原值；

（4）采用渐进扩容的方式，当桶被实际操作到时，由使用者负责完成数据迁移，避免因为一次性的全量数据迁移引发性能抖动.

map 的扩容类型分为两类，一类叫做增量扩容，一类叫做等量扩容.

（1）增量扩容

表现：扩容后，桶数组的长度增长为原长度的 2 倍；

目的：降低每个桶中 key-value 对的数量，优化 map 操作的时间复杂度.

（2）等量扩容

表现：扩容后，桶数组的长度和之前保持一致；但是溢出桶的数量会下降.

目的：提高桶主体结构的数据填充率，减少溢出桶数量，避免发生内存泄漏.
