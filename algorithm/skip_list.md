跳表
  核心思想，“空间来换取时间"

(1) 由很多层结构组成
(2) 每一层都是一个有序的链表
(3) 最底层(Level 0)的链表包含所有元素
(4) 如果一个元素出现在 Level i 的链表中，则它在 Level i 之下的链表也都会出现。
(5) 每个节点包含两个指针，一个指向同一链表中的下一个元素，一个指向下面一层的元素。
(6 )插入元素的时候，元素所占有的层数完全是随机的 

# 文章 
  https://www.cnblogs.com/a8457013/p/8251967.html

# 代码
  github.com/ryszard/goskiplist/skiplist


# skiplist
type SkipList struct {
    lessThan func(l, r interface{}) bool
    // header 节点的key,value不使用，每次查询都是从header.forward开始
    header   *node  
    footer   *node
    length   int
    MaxLevel int
} 

# 节点，
type node struct {
    // forward 指向后一个节点，foward的长度就是该节点的深度,深度0包含所有的节点
    forward    []*node
    backward   *node
    key, value interface{}
}

# skiplist 的操作和list 一个思想，修改操作节点(新增节点和删除节点)的前后节点的指针, 不一样在 跳表的指向后一个节点的数据结构是 slice, 


