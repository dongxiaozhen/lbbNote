测试数据下载  https://launchpad.net/test-db/+download

优秀文摘
    https://mp.weixin.qq.com/s/B4Yu8KWTkm1Jnwc5B5l9lg

    https://dataschool.com/how-to-teach-people-sql/how-sql-subqueries-work/

Server 层：主要包括连接器、查询缓存、分析器、优化器、执行器等，所有跨存储引擎的功能都在这一层实现，比如存储过程、触发器、视图，函数等，还有一个通用的日志模块 binglog 日志模块。
存储引擎： 主要负责数据的存储和读取，采用可以替换的插件式架构，支持 InnoDB、MyISAM、Memory 等多个存储引擎，其中 InnoDB 引擎有自有的日志模块 redolog 模块。

浅分叶 增加sort的字段索性
深分页
   1 增加查询字段的联合索性
   2 手动回表，强制走索性
   3 客户端带当前页最后一个id和排序字段，查询

子查询优化

1. 使用表连接进行替换，尤其是子查询数据量大的时候
2. 优化复合索引，包含where和关键字段
3. 复合索引最左侧匹配原则
4. select * from a whre xx=(select * from b)
5. 标量子查询 =, <, >
6. 列子查询 in, not in, any, some, all
7. select * from (select * from b) e join c on e.xx = c.xx （将子查询结果作为一个表）

原则 1：加锁的基本单位是 next-key lock。希望你还记得，next-key lock 是前开后闭区间。
原则 2：查找过程中访问到的对象才会加锁。
优化 1：索引上的等值查询，给唯一索引加锁的时候，next-key lock 退化为行锁。
优化 2：索引上的等值查询，向右遍历时且最后一个值不满足等值条件的时候，next-key lock 退化为间隙锁。
一个 bug：唯一索引上的范围查询会访问到不满足条件的第一个值为止。

mysql 执行了查询数据，但是没有读取(exec代替query)，会出现busy buffer的错误提示。
mariadb -- mysql 配置文件 /etc/my.cnf 修改密码 mysql_secure_installation 空密码输入'' systemctl start/enable mariadb.service
MyISAM 三个文件保存表信息.frm格式，.MYD数据，.MYI索引，速度快， b+,保存索引
InnoDB 支持事务 b+树，保存行号

mysql
整个update语句中牵涉到写redo log和binlog，并且redo log在前，binlog在后，并且redo log的写入被拆分成了prepare和commit两个步骤，这就是两阶段提交在数据库中的应用。
假设binlog写完，但是redo log还没commit之前发生了crash，此时MySQL在崩溃恢复时会有一定的处理逻辑？
    如果redo log里面的事务是完整的（有commit标识），则直接提交
    如果redo log里面只有完整的prepare，则判断对应的事务binlog是否存在且完整，如果完整则提交事务，否则回滚事务。

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-09-18-27-01-image.png)

![](/Users/libinbin/Library/Application%20Support/marktext/images/2023-04-09-18-28-50-image.png)

redo log和binlog有一个共同的数据字段是XID，在崩溃恢复时，会顺序扫描redo log：
    如果redo log既有prepare，又有commit，则直接提交
    如果redo log只有prepare，则会拿着XID去找binlog，如果binlog里面有则提交，否则回滚

redolog
    mysql是如何保证事务里的数据不丢失的？没错，就是通过redo log
    redo保证了事务的原子性和持久性。
    1.保证数据的持久性。众所周知，数据是记录在磁盘上的，如果事务提交后mysql突然挂了（比如断电之类的），那么内存里的数据，是不是就丢失了？我们数据恢复时候，通过redo 就可以恢复回来。
    2.redo只是记录了对数据的修改，数据会比一页数据小得多。大大减少了IO频率。
    3.redo日志的写入是顺序IO。而修改磁盘的B+树是随机IO。

    redo写入磁盘全流程
    1.将上文提到的一组redo log写入redo log buffer（依然在内存中，依旧是为了调节CPU与磁盘写入速度的矛盾）。
    2.redo log buffer写入page cache（操作系统里的页，操作系统会找个时间自动刷到磁盘。或者你调用fsync函数就能确保刷盘）。
    3.page cache的数据刷到磁盘。
        上述第2,3步是我们可以控制的，通过设置innodb_flush_log_at_trx_commit的值就可以控制，该变量可选值如下：（跟redis的差不多）
        0：不立即写入page cache。交给后台线程处理(1s一次，不仅写入page cache还会调用fsync)。这种情况太危险，但是性能最好。
        1：每次都要严格落盘。会调用fsync,保证了数据落盘，但是性能较差。
        2：redo log buffer的数据写入page cache就行了。这个时候如果mysql挂了但是操作系统没挂，是可以等待后续落盘的。

mysql /var/lib/mysql /etc/mysql/conf.d

1.显示MYSQL的版本
mysql> select version(); 
2. 显示当前时间
mysql> select now(); 
3. 显示年月日
SELECT DAYOFMONTH(CURRENT_DATE); 

显示数据库 show database;
    数据库信息 show create database name_db\G
显示表     show tables;
    信息 describe tb_name
         show columns from tb_name
         show field from tb_name
         show table status

删除表中数据 命令：delete from 表名 where 表达式
修改表中数据：update 表名 set 字段=新值,… where 条件
修改表：
    增加字段 alter table 表名 add字段 类型 其他;
    加索引
           mysql> alter table 表名 add index 索引名 (字段名1[，字段名2 …]);
    加主关键字的索引
          mysql> alter table 表名 add primary key (字段名);
    加唯一限制条件的索引
           mysql> alter table 表名 add unique 索引名 (字段名);
    删除某个索引
           mysql> alter table 表名 drop index 索引名;
    增加字段：
        mysql> ALTER TABLE table_name ADD field_name field_type;
    修改原字段名称及类型：
        mysql> ALTER TABLE table_name CHANGE old_field_name new_field_name field_type;
    修改表名
        命令：rename table 原表名 to 新表名;

导出数据 mysqldump -u root -p database_name [table_name] > dump.sql
倒入数据 mysql -u root -p database_name < dump.sql (database_name需要提前建立)

group by 只显示一行  group_concat（） 将在group结果中的其他列的数据显示出来  
    在select指定的字段要么就要包含在Group By语句的后面，作为分组的依据；要么就要被包含在聚合函数中
    没有聚合函数，相当于去重。
    group by 相当于条件过滤，已group by 后面的字段为输出总样本

join 笛卡尔积（交叉组合）,左表和右表没有组合的不显示 a*b

    1. 隐式 select * from a_table,b_table where xx;

    2. 显示 select * from a_table join b_table on xx;

left join 左表和右表没有组合的,也显示左表的行
right join 左表和右表没有组合的,也显示右表的行

union 联合查询，把多次查询的结果合并起来

    select * from a

    union

    select * from b

sum 算数 count 行数
hour(timevar) 返回值是可以大于23的小时
非自然时间计算
     select id,mod(hour(timediff(pay_time,'2016-06-23T12:30:00')),24) as 'index' ,user_id,product_id from t_order limit 10;

可设置的参数列表
    mysqld --verbose --help

update table 的时候，如果前后数据不变，row_count() == 0;(变化的行数)

查询所有procedure
select name from mysql.proc where db = 'your_db_name' and type = 'PROCEDURE'
查看procedure
show create procedure proc_name;
show create function func_name;

ON DUPLICATE KEY
insert into t_user_equip_generate_50 values(?,?) ON DUPLICATE KEY update   info = ?

锁（何登成大神解释比较清楚）：http://hedengcheng.com/?p=771
锁深入浅出一书讲解：http://blog.sina.com.cn/s/blog_53b13d950100vtks.html

row_count() 要紧跟在select,insert,update 之后，中间不能插入其他数据，然不要取出count结果不对

sudo vim /etc/mysql/mysql.conf.d/mysqld.cnf  skip-grant-tables // 不需要验证密码登录
show engines //查看数据库支持的引擎
show engine innodb status  // 查看引擎innodb的状态
    Total large memory allocated 137428992
    Dictionary memory allocated 88631
    Buffer pool size   8191
    Free buffers       7842
    Database pages     349

show @@version; // 查看mysql版本
show variables like 'innodb_version'; // innode
show variables like 'innodb_file_format';
show variables like 'innodb_buffer_pool_size';
show variables like 'innodb_log_buffer_size';
show variables like 'innodb_additional_mem_pool_size';

// 读取事务隔离级别
select @@transaction_isolation;
// 设置隔离级别
set session transaction isolation level read committed;
set session transaction isolation level repeatable read;
// 取消自动提交
set autocommit=0;
// 查询死锁表
select * from performance_schema.data_locks;
// 判断主从延迟的方法
    可以通过 show slave status 进行查看
查看mysql 数据目录
    show variables like 'datadir'
查看innodb redolog配置
    select @@innodb_log_file_size
    select @@innodb_log_files_in_group;

通过 show status 命令了解各种 sql 的执行
    show status like 'Com_%';
        Com_select : 执行 select 操作的次数，一次查询只累加 1。
            Com_insert : 执行 insert 操作的次数，对于批量插入的 insert 操作，只累加一次。
            Com_update : 执行 update 操作的次数。
            Com_delete : 执行 delete 操作的次数。

系统库 information_schema
    PARTITIONS  分区
    TRIGGERS
    TABLES
    VIEWS
    STATISTICS 索引

导入测试数据
  1 修复测试数据库数据版本问题 storage_engine 不再使用
  send -i 's/storage_engine/default_storage_engine/g' employees_partitioned3.sql 
  sed -i 's/storage_engine/default_storage_engine/g' employees_partitioned3.sql 
  sed -i 's/storage_engine/default_storage_engine/g' employees_partitioned2.sql 
  sed -i 's/storage_engine/default_storage_engine/g' test_employees_md5.sql 
  sed -i 's/storage_engine/default_storage_engine/g' employees_partitioned.sql 
  sed -i 's/storage_engine/default_storage_engine/g' employees.sql 
  sed -i 's/storage_engine/default_storage_engine/g' test_employees_sha.sql 
  mysql -t < employees.sql

索引：聚集索引=primary_key 非聚集索引=辅助索引=index
使用聚集索引的查询效率要比非聚集索引的效率要高，但是如果需要频繁去改变聚集索引的值，写入性能并不高，因为需要移动对应数据的物理位置。
非聚集索引在查询的时候可以的话就避免二次查询，这样性能会大幅提升。
不是所有的表都适合建立索引，只有数据量大表才适合建立索引，且建立在选择性高的列上面性能会更好。

分区: 
   RANGE a<10
   LIST  a in (1,3,4)
   HASH

优化前：sql1 = select * from test where val=4 limit 300000,5
优化后：sql2 = select * from test a inner join (select id from test where val=4 limit 300000,5)
优化原理: limit 的offset 比较大的时候，减少回表操作
    1 sql1 需要把所有的数据准备好之后，然后丢去300000个数据，最后取5个
    2 sql2 先找出3000005个索引，然周过滤出5会让索引，最后再查处所有数据

通过慢查询日志定位那些执行效率较低的 sql 语句，用 --log-slow-queries[=file_name] 选项启动时，mysqld 写一个包含所有执行时间超过 long_query_time 秒的 sql 语句的日志文件。

explain
    all、index、range、ref、eq_ref、const，system、null：
    type=ALL，全表扫描，mysql 遍历全表来找到匹配的行：
    type=index, 索引全扫描，mysql 遍历整个索引来查询匹配的行
    type=range,索引范围扫描，常见于<、<=、>、>=、between等操作：
    type=ref, 使用非唯一索引扫描或唯一索引的前缀扫描，返回匹配某个单独值的记录行，
    type=eq_ref,类似 ref，区别就在使用的索引时唯一索引，对于每个索引的键值，表中只要一条记录匹配；简单的说，就是多表连接中使用 primary key 或者 unique index 作为关联条件。

1、查询SQL尽量不要使用select *，而是select具体字段。
    只取需要的字段，节省资源、减少网络开销。
    select * 进行查询时，很可能就不会使用到覆盖索引了，就会造成回表查询。

2、如果知道查询结果只有一条或者只要最大/最小一条记录，建议用limit 1
    加上limit 1后,只要找到了对应的一条记录,就不会继续向下扫描了,效率将会大大提高。

3、应尽量避免在where子句中使用or来连接条件
    使用or可能会使索引失效（存在没有索引的情况），从而全表扫描

4、优化limit分页
    当偏移量最大的时候，查询效率就会越低，因为Mysql并非是跳过偏移量直接去取后面的数据，而是先把偏移量+要取的条数，然后再把前面偏移量这一段的数据抛弃掉再返回的。
    如果使用优化方案一，返回上次最大查询记录（偏移量），这样可以跳过偏移量，效率提升不少。
    方案二使用order by+索引，也是可以提高查询效率的。
    方案三的话，建议跟业务讨论，有没有必要查这么后的分页啦。因为绝大多数用户都不会往后翻太多页。

5、优化你的like语句
    把%放前面，并不走索引，如下： 
    把% 放关键字后面，还是会走索引的。如下： 

7、尽量避免在索引列上使用mysql的内置函数
    反例
        select userId,loginTime from loginuser where Date_ADD(loginTime,Interval 7 DAY) >=now();复制代码
    正例
        explain  select userId,loginTime from loginuser where  loginTime >= Date_ADD(NOW(),INTERVAL - 7 DAY);复制代码

8、应尽量避免在 where 子句中对字段进行表达式操作，这将导致系统放弃使用索引而进行全表扫
9、Inner join 、left join、right join，优先使用Inner join，如果是left join，左边表结果尽量小
    Inner join 内连接，在两张表进行连接查询时，只保留两张表中完全匹配的结果集
    left join 在两张表进行连接查询时，会返回左表所有的行，即使在右表中没有匹配的记录。
    right join 在两张表进行连接查询时，会返回右表所有的行，即使在左表中没有匹配的记录。

21、尽量用 union all 替换 union
    如果使用union，不管检索结果有没有重复，都会尝试进行合并，然后在输出最终结果前进行排序。如果已知检索结果没有重复记录，使用union all 代替union，这样会提高效率。

27、尽可能使用varchar/nvarchar 代替 char/nchar。
    因为首先变长字段存储空间小，可以节省存储空间。
    其次对于查询来说，在一个相对较小的字段内搜索，效率更高。

28、为了提高group by 语句的效率，可以在执行到该语句前，把不需要的记录过滤掉。

假设所查询的列，刚好都是索引对应的列，不用再回表查，那么这个索引列，就叫覆盖索引。

主从复制原理，简言之，就三步曲，如下：
    主数据库有个bin-log二进制文件，纪录了所有增删改Sql语句。（binlog线程）
    从数据库把主数据库的bin-log文件的sql语句复制过来。（io线程）
    从数据库的relay-log重做日志文件中再执行一次这些sql语句。（Sql执行线程）

分库分表，
    单机上分库分表，降低因为单表数据量大查询慢问题
    多机上分库分表，降低单个机器的压力

mac 
    mysql -uroot
    brew services restart mysql

binlog

    1. 主要用于数据库恢复，主从复制，审计操作

    2. 数据格式 show vaiables linke 'binlog_format'

        2.1 statement 原生格式

        2.2 row 模式

        2.3 mixed 混合模式

    3. 配置 bin-log=dir

    4. 查看binlog文件 show binary logs;

    5. 查看binlog数据 mysqlbinlog mylog.00001

    6. 自动删除 配置expire_logs_days

    7. 手动删除 

        7.1 删除所有的 reset master

        7.2 删除指定二进制日志编号之前的日志 purge binary logs to ''bin-log.00002

        7.3 根据创建日志删除 purge binary logs befor     
