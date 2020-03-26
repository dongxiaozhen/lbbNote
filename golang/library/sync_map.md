
sync.Map 包括两个map ，
    load() 如果 read里面没有，并且dity和read不同步(read.amended),则去dity里读
    store()如果 read里有则更新entry,如果read没有， 则去dirty里找，找到就更新entry，如果没有，看read.amended是否为false,如果是则说明一致，如果dity是nil，用read赋值dirty, 最后加入新key

    read给dirty赋值时，删除里的可以不会出现在dity里
    
    entry
        expunged 说明该key已经被删除，而且read和dirty数据不一致，dirty里面已经删除了
        nil      说明该key被删除，而且read和dirty里的数据一致，只需要修改p指针的值就行
    




