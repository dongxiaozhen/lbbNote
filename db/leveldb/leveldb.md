block index_block 里指向的datablock的 offset,length,但是length不包括后面的1bytes的CompressType,4个字节的CRC32校验位
meta block : 作用是快速判断对应的data block中是否存在某个key，详情可以搜索“Bloom Filter”


mCompaction: minor compaction，就是把memory的内容写入到level 0的文件
   获取zeroMem,序列化到文件中，根据文件imin,imax判断文件的所处的level,加入的version中，将本次的变化sessionRecord写入manifest中

tCompaction：major compaction，就是合并不同层级的level文件。
   level0层compaction_score_与文件数相关，其他level的则与当前层的文件总大小相关
   1.获得要Compaction的一个文件加入input_[0]，容量触发时这个文件由compaction_level_加compact_pointer_确定，否则由file_to_compact_level_和file_to_compact_确定。对于level0，由于其文件相互重合，需要将所有与当前Compaction文件重合的文件全部加入input_[0]。
   2.获得所有与level[0]有Key Range重合的level+1层文件加入input_[1]，可以看出所有input_[1]文件的Key Range可能大于level[0]，为了减少LevelDB整体Compaction次数，LevelDB会在不增加input_[1]文件数的前提下尝试增加level[0]文件数来扩大level层文件的Key Range。
   3.获得所有与当前Key Range重合的level+2层文件加入input_[2]，这里记录level+2层的文件信息是为了Compaction生成新的level+1层文件时，保证新文件不会与level+2中太多的文件有Key Range的重合，从而导致以后该文件的Compaction有太大的Merge开销，这个信息会在生成新文件的过程中不断检查。
   4.生成归并Iterator，接下来就是用上面收集的信息生成归并Iterator，之后遍历这个Iterator生成新的文件，Iterator相关的内容会在之后一篇博客详细介绍
 
tFile overlaps:
    给出的key范围是否和另一个key的范围有交集,(5,8) 分别和 (1,6),(6,7),(6,9)有交集

tFiles overlaps
    给出的key范围是否包含另一个key的范围,(5,8) 包含 (6,7)
    给出的范围会根据原来的范围一直调节最大值，最小值,每次调整后会重新循环遍历。
    参数overlapped 表示是否调整最大值的范围,level0调整最大值,level1以上不调整

#文章
  1 https://www.cnblogs.com/KevinT/p/3819134.html
  2 https://www.cnblogs.com/KevinT/p/3819134.html
  3 http://www.cnblogs.com/chenny7/p/4026447.html
  4 https://m.2cto.com/database/201802/718538.html
  5 https://www.cnblogs.com/ourroad/p/5035108.html
