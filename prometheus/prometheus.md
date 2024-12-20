prometheus

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-19-10-07-48-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-19-10-09-30-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-19-10-09-38-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-19-10-28-21-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-19-10-41-11-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-19-10-41-54-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-19-13-42-17-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-19-13-44-09-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-19-13-47-57-image.png)

使用ignoreing可以在匹配时忽略某些便签。而on则用于将匹配行为限定在某些便签之内。

一对一

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-19-19-33-27-image.png)

多对一

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-19-19-32-37-image.png)



注意：聚合操作和内置函数是不一样的。

![](/Users/libinbin/Library/Application%20Support/bluestone/assets/Kdg9xFe6dKuVI.png)

例如： topk(5,sum(server_handled_total) by (account))



`v range-vector: 数据形式：`

![](/Users/libinbin/Library/Application%20Support/bluestone/assets/DN7Pm8QstAx4k.png)

![](/Users/libinbin/Library/Application%20Support/bluestone/assets/ZNPaMAUu4bpHY.png)

increase, irate 等内置函数都是基于 此种数据类型



- 有时你想新增一个标签，从而将一个粗粒度的指标进行拆分，但切记添加的标签的值应该做到尽量收敛，不然会导致同一指标名的标签数量巨大而导致 Prometheus 严重的性能问题（OOM）。
- 所以请您牢记：指标上标签的每一种唯一组合都会自动创建一个对应时间序列，Prometheus 都会对其进行摄取、索引、存储和处理。
- 如果将时间窗口设置得太小，则在该窗口下可能只有一个或零个样本，在这种情况下，输出结果为空。

  例如，如果您采用 20s 的时间窗口对一个 15s 抓取间隔的指标进行 rate，那么在这 20 秒时间窗口内很可能不会涵盖两个样本，因此您会得到一个与实际结果差异很大的比率：
