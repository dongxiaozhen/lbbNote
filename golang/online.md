

内嵌online 
    对于简单、短小的函数，编译器会做内嵌


注意：
    1. 如果正常流程中做内嵌，会提高性能。
    2. 如果异常流程做了内嵌，但是该case 极少进入，反而是减少了cpu cache 的代码数量,导致性能差