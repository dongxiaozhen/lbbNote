## 链式梯度

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-20-02-47-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-20-02-56-image.png)

图像分类 f(x,w)=wx+b ![](./4CE55B12-6F41-4DEF-AA7D-A91FBC9129FC.png)

损失函数计算![](./ECEF4DAD-B211-4B45-9488-116952D738BE.png)

![](./58BC4FBC-37EA-42D5-9ADE-B4E2DFB3C093.png)

![](./F306C64D-7EF0-4817-B42C-2A0D37A5475E.png)

![](./5205322F-1380-4DE2-B112-00FD9924A778.png)

一个一个参数的调整w，计算梯度，太慢 )![](./A7381E2E-0FC9-4AC0-9F66-28A771A80ACB.png)

新计算梯度)![](./BC032D75-FA61-4E8D-900B-7C7672C93404.png)

![](./15DE650C-397E-4EB4-9A3A-3ADAED06DAD7.png)

反向梯度![](./4E82B65D-B613-45D5-80F7-394AD5960857.png)

矩阵![](./BD39848C-17DB-4BC8-BCD0-1051EC4BAC16.png)

![](./7092EED0-BCEB-4850-8A8D-343D4D79F7C2.png)

![](./3B7F4AE3-29D6-4204-9C07-D0DF6452544B.png)

反向传播

1. 一个输入，会产生多个输出，每个输出，都会期待前一层的梯度变化，然后汇总计算一个值

![](./47841916906735782.png)

2. 多个输入计算对一个w的平均值

3. ![](54561616906736492.png)

4. 将一批数据分成多个批次，一个批次有多个数据，(随机梯度的随机含义)![](60892216906737122.png)

## 简单两层模型

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-02-10-16-24-image.png)

O是激活函数，sigmod

第一层 z2 = O(w1x+b1)

第二层 z1 = O(w2(z2) + b2)

损失函数 (z1-y)^2 / 2 

-------------------

第二层梯度
delta_b2 = (z1-y)* sigmoid_prime
delta_w2 = delta_b2 * z2

第一层梯度
delta_b1 = delta_w2 * w2  * sigmoid_prime 
delta_w1 = x * delta_b1

### 损失函数

1. 二次损失函数 quadratic cost function 
   
       1. (z-y)^2
   
       2. 梯度变化越来越小，变化不明显

2. 交叉商损失函数 cross-entropy cost function
   
   ![](cross.png)

            ![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-02-10-18-03-image.png)

## 激活函数

### softmax

![](/Users/libinbin/Downloads/4235416914202822.png)

![](/Users/libinbin/Downloads/12651616914203812.png)

## CNN

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-20-07-14-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-20-07-22-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-20-07-28-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-20-07-35-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-20-07-59-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-20-08-07-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-20-08-16-image.png)

## RNN

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-20-10-21-image.png)

word2vec 每个词使用一个50维的向量表示，

通过训练，同时更新 w, x (普通模型只更新w), 训练是通过输入两个词，输出是相似度

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-26-22-27-52-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-26-22-33-11-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-26-22-35-01-image.png)

### text-CNN

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-15-09-07-image.png)



LSTM 

 遗忘门，输入门，输出门

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-20-05-25-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-20-09-17-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-20-09-55-image.png)

## GAN 对抗生成网络

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-08-27-16-20-49-image.png)

## attention 自注意模型q, k, v

q 问题，维度和k 一样。query

k 参数,keys 

v 答案,values

如果，q,k, v 是一样的，那就是自注意模型

t = q·k 计算q和所有k的相关度

t · v 挑选相关度里最相关的内容

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-02-20-47-31-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-02-20-49-35-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-02-20-50-10-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-03-20-13-02-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-03-20-44-28-image.png)

## input embeding

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-02-20-57-12-image.png)

encoding 

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-02-21-00-40-image.png)

多层

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-02-21-01-10-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-02-21-16-31-image.png)

decode is auto regressive 自回归的(输出作为下一次的输入)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-02-21-18-03-image.png)

decode 计算attention和encode区别是，decode层attention 前面的词时不考虑后面的词 

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-02-21-19-07-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-10-22-08-57-38-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-10-22-08-59-10-image.png)



## GNN 图深度学习 GCN

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-22-14-06-22-image.png)

1. 消息传递

2. 消息聚合

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-22-14-17-56-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-21-18-00-22-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-21-17-59-18-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-21-19-27-13-image.png)

全链接层来做神经元数量变化，512-> 256

每一层的多个输入做求和平均操作，输入运算层

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-21-19-49-33-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-21-20-59-44-image.png)

加入自引用，听到自己的声音

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-21-21-04-25-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-21-21-10-15-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-09-21-21-11-10-image.png)
