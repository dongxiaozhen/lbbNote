
##leveldb skiplist
###github.com/golang/leveldb

type MemDB struct {
   mutex sync.RWMutex
   // height is the number of such lists, which can increase over time.
   height int 
   // cmp defines an ordering on keys.
   cmp db.Comparer
   // kvData is an append-only buffer that holds varint-prefixed strings.
   kvData []byte
   // nodeData is an append-only buffer that holds a node's fields.
   nodeData []int
}

kvData 存放key,value,格式是 
         |-------------------|------------------------------
         |klen|key|vlen|value|...
         |-------------------|-----------------------------


nodeData的格式：
----------------------------------------|---------------------------------|------------------------------------------
|off1|off2|off3|off4|off5|off6|...|off12|koffset|voffset|noffset1|noffset2|....
----------------------------------------|---------------------------------|-----------------------------------------
前12字节是指向后面节点的地址,总共12层
后面开始就是节点信息
   koffset  key在kvData的偏移
   voffset  value在kvData的偏移
   noffset1 level0指向下一个节点的便宜(在nodeData里的偏移) 
   noffset2 level1指向下一个节点的便宜(在nodeData里的偏移) 
   .......


##github.com/syndtr/goleveldb
 type DB struct {
    kvData []byte
    nodeData  []int
    prevNode  [tMaxHeight]int
    maxHeight int
    n         int
    kvSize    int
}

kvData
------------------------
|key|value|key|value|....
------------------------

nodeData
----------------------------------------------|---------
|kvoffset|klen|vlen|levelNum|noffset1|noffset2|.....
----------------------------------------------|----------
