# ebpf

## map types

```c
                  enum bpf_map_type {
                      BPF_MAP_TYPE_UNSPEC,  /* Reserve 0 as invalid map type */
                      BPF_MAP_TYPE_HASH,
                      BPF_MAP_TYPE_ARRAY,
                      BPF_MAP_TYPE_PROG_ARRAY,
                      BPF_MAP_TYPE_PERF_EVENT_ARRAY,
                      BPF_MAP_TYPE_PERCPU_HASH,
                      BPF_MAP_TYPE_PERCPU_ARRAY,
                      BPF_MAP_TYPE_STACK_TRACE,
                      BPF_MAP_TYPE_CGROUP_ARRAY,
                      BPF_MAP_TYPE_LRU_HASH,
                      BPF_MAP_TYPE_LRU_PERCPU_HASH,
                      BPF_MAP_TYPE_LPM_TRIE,
                      BPF_MAP_TYPE_ARRAY_OF_MAPS,
                      BPF_MAP_TYPE_HASH_OF_MAPS,
                      BPF_MAP_TYPE_DEVMAP,
                      BPF_MAP_TYPE_SOCKMAP,
                      BPF_MAP_TYPE_CPUMAP,
                  };
```

1. BPF_MAP_TYPE_HASH

```c
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, type1);         // key的类型
    __type(value, type2);       // value类型
    __uint(max_entries, 1024);  // 最大 entry 数量
} hash_map SEC(".maps");
```

2. BPF_MAP_TYPE_ARRAY

   ```c
   struct {
       __uint(type, BPF_MAP_TYPE_ARRAY);
       __type(key, u32);
       __type(value, type1);
       __uint(max_entries, 256);
   } my_map SEC(".maps");
   ```

   可以发现和hash的声明没啥区别，但是在操作的时候key的语义是index。

3. ### BPF_MAP_TYPE_PERCPU_HASH / BPF_MAP_TYPE_PERCPU_ARRAY

   此类型会在每一个CPU上创建一个哈希map或者array map，每个 CPU 将拥有该Map的单独副本，副本不会以任何方式保持同步。

4. ### BPF_MAP_TYPE_LRU_HASH / BPF_MAP_TYPE_LRU_PERCPU_HASH

   普通 hash map 的问题是有大小限制，超过最大数量后无法再插入了。LRU map 可以避 免这个问题，如果 map 满了，再插入时它会自动将最久未被使用（least recently used）的 entry 从 map 中移除。

5. ### BPF_MAP_TYPE_HASH_OF_MAPS / BPF_MAP_TYPE_ARRAY_OF_MAPS

   map结构中的结构是一个map

6. ### BPF_MAP_TYPE_PERF_EVENT_ARRAY

   用户态程序使用 bpf(BPF_MAP_UPDATE_ELEM) 将由 sys_perf_event_open() 取得的文件描述符传递给 eBPF 程序，ebpf程序可以调用`bpf_perf_event_output`从内核向用户态程序传递数据，用户态可以调用`perf_buffer__new`，`perf_buffer__poll()`监听

7. ### BPF_MAP_TYPE_RINGBUF

   `BPF_MAP_TYPE_PERF_EVENT_ARRAY`的升级版，是一个多生产者、单消费者 (MPSC) 队列，API与`BPF_MAP_TYPE_PERF_EVENT_ARRAY`大体相同，但是写入被划分为三个API，`bpf_ringbuf_reserve()`/`bpf_ringbuf_commit()`/`bpf_ringbuf_discard()` ，同时允许用户态监听

8. ### BPF_MAP_TYPE_LPM_TRIE

   LPM(Longest prefix match)的语义就是最长的前缀匹配，[8]中提到key的限制是八的倍数，范围为[8,2048]，可以使用`bpf_lpm_tire_key`来作为key，value随意。

9. ### BPF_MAP_TYPE_QUEUE / BPF_MAP_TYPE_STACK

   队列结构和栈，`bpf_map_update_elem`是push，`bpf_map_lookup_elem`是begin，`bpf_map_lookup_and_delete`是pop

10. ### BPF_MAP_TYPE_BLOOM_FILTER

正常的布隆过滤器实现

11. ### BPF_MAP_TYPE_SOCKMAP / BPF_MAP_TYPE_SOCKHASH

用于套接字的转发，value为`bpf_sock_ops`结构，在hash中key需要自己指定，调 `long bpf_sock_map_update(struct bpf_sock_ops *skops, struct bpf_map *map, void *key, u64 flags)`更新map。



调用如下函数可以实现转发：

1. `long bpf_msg_redirect_hash(struct sk_msg_buff *msg, struct bpf_map *map, void *key, u64 flags)`
2. `long bpf_sk_redirect_hash(struct sk_buff *skb, struct bpf_map*map, void *key, u64 flags)`
3. `long bpf_msg_redirect_map(struct sk_msg_buff *msg, struct bpf_map*map, u32 key, u64 flags)`
4. `long bpf_sk_redirect_map(struct sk_buff *skb, struct bpf_map *map, u32 key, u64 flags)`

分别用于`BPF_PROG_TYPE_SOCK_OPS`和`BPF_PROG_TYPE_SK_SKB`

12. ### BPF_MAP_TYPE_DEVMAP / BPF_MAP_TYPE_DEVMAP_HASH

内存储的是网络设备号ifindex，XDP程序可以将包直接转发到存储在这里的设备中，提升包转发性能

13. ### BPF_MAP_TYPE_CPUMAP

## bpf_prog_type

```c
                  enum bpf_prog_type {
                      BPF_PROG_TYPE_UNSPEC,        /* Reserve 0 as invalid
                                                      program type */
                      BPF_PROG_TYPE_SOCKET_FILTER,
                      BPF_PROG_TYPE_KPROBE,
                      BPF_PROG_TYPE_SCHED_CLS,
                      BPF_PROG_TYPE_SCHED_ACT,
                  };
```



1. #### uprobe

   1. uprobe是一种用户空间探针，uprobe探针允许在用户空间程序中动态插桩，插桩位置包括：函数入口、特定偏移处，以及函数返回处。当我们定义uprobe时，内核会在附加的指令上创建快速断点指令（x86机器上为int3指令），当程序执行到该指令时，内核将触发事件，程序陷入到内核态，并以回调函数的方式调用探针函数，执行完探针函数再返回到用户态继续执行后序的指令。
   2. uprobe基于文件，当一个二进制文件中的一个函数被跟踪时，所有使用到这个文件的进程都会被插桩，包括那些尚未启动的进程，这样就可以在全系统范围内跟踪系统调用。
   3. uprobe适用于在用户态去解析一些内核态探针无法解析的流量，例如http2流量（报文header被编码，内核无法解码）、https流量（加密流量，内核无法解密）等。
   4. bpf_program__attach_uprobe()可以将BPF程序附加到通过二进制路径和偏移量找到的用户空间函数中。可以选择指定要附加的特定进程，还可以选择将程序附加到函数出口或入口。

   ```c
   
   
   struct bpf_link *bpf_program__attach_uprobe(
       const struct bpf_program *prog,
       bool retprobe,
       pid_t pid,
       const char *binary_path,
       size_t func_offset)
   
   ```

   参数：

   - prog：要附加的 BPF 程序
   - retprobe：附加到函数出口
   - pid：附加 uprobe 的进程 ID（0 表示自身进程，-1 表示所有进程）
   - binary_path：包含函数符号的二进制文件的路径
   - func_offset ：函数符号二进制内的偏移量

   5. ```c
      SEC("uretprobe//home/work/project/libbpf-demo/examples/test/utest/build/utest:_Z9utest_subii")
      int BPF_KRETPROBE(urettest_sub, int ret)
      {
          bpf_printk("utest_sub EXIT: return = %d", ret);
          return 0;
      }
      ```

2. kprobe

   1. 利用 kprobes 技术，用户可以定义自己的回调函数，然后在内核或者模块中几乎所有的函数中（有些函数是不可探测的，例如kprobes自身的相关实现函数，后文会有详细说明）动态地插入探测点，当内核执行流程执行到指定的探测函数时，会调用该回调函数动态地插入探测点，当内核执行流程执行到指定的探测函数时，会调用该回调函数，用户即可收集所需的信息了，同时内核最后还会回到原本的正常执行流程。如果用户已经收集足够的信息，不再需要继续探测，则同样可以动态地移除探测点。因此 kprobes 技术具有对内核执行流程影响小和操作方便的优点。
   2. kprobes 技术包括的3种探测手段分别时 kprobe、jprobe 和 kretprobe

      - kprobe  
        是最基本的探测方式，是实现后两种的基础，它可以在任意的位置放置探测点（就连函数内部的某条指令处也可以），它提供了探测点的调用前、调用后和内存访问出错3种回调方式，分别是  
        pre_handler、post_handler 和 fault_handler，其中 pre_handler  
        函数将在被探测指令被执行前回调，post_handler 会在被探测指令执行完毕后回调（注意不是被探测函数），fault_handler  
        会在内存访问出错时被调用
      - jprobe 基于 kprobe 实现，它用于获取被探测函数的入参值
      - kretprobe 从名字中就可以看出其用途了，它同样基于 kprobe 实现，用于获取被探测函数的返回值。


3. tc

   tc的全程是`traffic control`,即流量控制.通过`tc`可以对网络流量进行控制,例如限速,限流,负载均衡等等.从`Kernel 4.1`开始，TC支持加载eBPF程序到子系统的Hook点，并且在之后的`Kernel 4.4`中引入了`direct-action`模式，Cilium、Calico等网络插件大量使用TC Hook来控制网络包的转发。

   子系统包括qdisc、class、classifier（filter）、action等概念，eBPF程序可以作为classifier被挂载

4. xdp

XDP机制的主要目标是在接收数据包时尽早处理它们，以提高网络性能和降低延迟。它通过将eBPF程序附加到网络设备的接收路径上来实现这一目标。具体而言，XDP程序会在数据包进入网络设备的接收队列之前执行，这样可以在内核中进行快速的数据包过滤和处理，而无需将数据包传递给用户空间。

`tc`（Traffic Control）和`xdp`（eXpress Data Path）是Linux网络中两种不同的数据包处理机制，他们的区别如下：

1. **位置不同**: `tc`位于Linux网络协议栈的较高层，主要用于在网络设备的出入口处对数据包进行分类、调度和限速等操作。而`xdp`位于网络设备驱动程序的接收路径上，用于快速处理数据包并决定是否将其传递给协议栈。
2. **执行时机不同**: `tc`在数据包进入或离开网络设备时执行，通常在内核空间中进行。而`xdp`在数据包进入网络设备驱动程序的接收路径时执行，可以在内核空间中或用户空间中执行。
3. **处理能力不同**: `tc`提供了更复杂的流量控制和分类策略，可以实现各种QoS（Quality of Service）功能。它可以对数据包进行过滤、限速、排队等操作。而`xdp`主要用于快速的数据包过滤和处理，以降低延迟和提高性能。
4. `bpf_perf_event_output`：用于通知用户态拷贝数据
5. bpf_printk ： 打日志，调试

   会将[字符串输出](https://so.csdn.net/so/search?q=%25E5%25AD%2597%25E7%25AC%25A6%25E4%25B8%25B2%25E8%25BE%2593%25E5%2587%25BA&spm=1001.2101.3001.7020)到文件/sys/kernel/debug/tracing/trace_pipe

   


### go library

1. github.com/cilium/ebpf
2. github.com/cilium/ebpf/cmd/bpf2go
3. github.com/gojue/ebpfmanager
4. 