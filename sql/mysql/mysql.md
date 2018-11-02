
测试数据下载  https://launchpad.net/test-db/+download


mysql 执行了查询数据，但是没有读取(exec代替query)，会出现busy buffer的错误提示。
mariadb -- mysql 配置文件 /etc/my.cnf 修改密码 mysql_secure_installation 空密码输入'' systemctl start/enable mariadb.service
MyISAM 三个文件保存表信息.frm格式，.MYD数据，.MYI索引，速度快， b+,保存索引
InnoDB 支持事务 b+树，保存行号

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

join 笛卡尔积（交叉组合）,左表和右表没有组合的不显示
left join 左表和右表没有组合的,也显示左表的行
right join 左表和右表没有组合的,也显示右表的行

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



