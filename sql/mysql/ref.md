'

Shared and Exclusive Locks
    A shared (S) lock permits the transaction that holds the lock to read a row.
    An exclusive (X) lock permits the transaction that holds the lock to update or delete a row.

Intention Locks(Intention locks are table-level locks that indicate which type of lock (shared or exclusive) a transaction requires later for a row in a table.)
    An intention shared lock (IS) indicates that a transaction intends to set a shared lock on individual rows in a table.
    An intention exclusive lock (IX) indicates that a transaction intends to set an exclusive lock on individual rows in a table.

The intention locking protocol is as follows:
Before a transaction can acquire a shared lock on a row in a table, it must first acquire an IS lock or stronger on the table.
Before a transaction can acquire an exclusive lock on a row in a table, it must first acquire an IX lock on the table.

Record Locks
    A record lock is a lock on an index record.
    example: For example, SELECT c1 FROM t WHERE c1 = 10 FOR UPDATE

Gap Locks
    A gap lock is a lock on a gap between index records, or a lock on the gap before the first or after the last index record.
    example:SELECT c1 FROM t WHERE c1 BETWEEN 10 and 20 FOR UPDATE

Gap locking is not needed for statements that lock rows using a unique index to search for a unique row

Next-Key Locks
    A next-key lock is a combination of a record lock on the index record and a gap lock on the gap before the index record.

Insert Intention Locks
    An insert intention lock is a type of gap lock set by INSERT operations prior to row insertio

AUTO-INC Locks
    An AUTO-INC lock is a special table-level lock taken by transactions inserting into tables with AUTO_INCREMENT columns

Predicate Locks for Spatial Indexes

Transaction Isolation Levels
InnoDB offers all four transaction isolation levels described by the SQL:1992 standard: 
READ UNCOMMITTED, SELECT statements are performed in a nonlocking fashion, but a possible earlier version of a row might be used
READ COMMITTED, This level is like REPEATABLE READ, but InnoDB implicitly converts all plain SELECT statements 
                to SELECT ... FOR SHARE if autocommit is disabled
REPEATABLE READ, 
SERIALIZABLE. 

The default isolation level for InnoDB is REPEATABLE READ.
可重复读：一个事务多次读取的数据肯定一致，但是可能不是最新的(其他事物修改了).(如果两个事务同时修改一个数据，会有阻塞)

At any time, issue the SHOW ENGINE INNODB STATUS command to determine the cause of the most recent deadlock. That can help you to tune your application to avoid deadlocks.
