
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


join 笛卡尔积（交叉组合）,左表和右表没有组合的不显示
left join 左表和右表没有组合的,也显示左表的行
right join 左表和右表没有组合的,也显示右表的行

sum 算数 count 行数
hour(timevar) 返回值是可以大于23的小时
非自然时间计算
 	select id,mod(hour(timediff(pay_time,'2016-06-23T12:30:00')),24) as 'index' ,user_id,product_id from t_order limit 10;


可设置的参数列表
	mysqld --verbose --help
