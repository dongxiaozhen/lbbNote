

标志：

1. PersistentFlags： 持久标志，可以被子命令继承

2. 全局标志： root命令的持久标志就是全局的

3. Flag: 本地标志



参数

1. args []string

2. command.Args 参数校验函数

3. command.ValidArgs 参数限定


