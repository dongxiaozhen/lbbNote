#分布式事务

github.com/dtm-lab/dtm

SAGA事务模式 核心思想是将长事务拆分为多个短事务，由Saga事务协调器协调，如果每个短事务都成功提交完成，那么全局事务就正常完成，如果某个步骤失败，则根据相反顺序一次调用补偿操作。
TCC事务模式 TCC是Try、Confirm、Cancel三个词语的缩写
	Try 阶段：尝试执行，完成所有业务检查（一致性）, 预留必须业务资源（准隔离性）
    Confirm 阶段：如果所有分支的Try都成功了，则走到Confirm阶段。Confirm真正执行业务，不作任何业务检查，只使用 Try 阶段预留的业务资源
    Cancel 阶段：如果所有分支的Try有一个失败了，则走到Cancel阶段。Cancel释放 Try 阶段预留的业务资源。
XA务模式
	第一阶段（prepare）：即所有的参与者RM准备执行事务并锁住需要的资源。参与者ready时，向TM报告已准备就绪。 
	第二阶段 (commit/rollback)：当事务管理者(TM)确认所有参与者(RM)都ready后，向所有参与者发送commit命令。

