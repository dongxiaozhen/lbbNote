filebeat
   type : log
   include_lines: 日志包含过滤  正则表达式
   exclude_lines: 日志删除过滤  正则表达式
   ignore_older： 时间过滤，多长时间不更新的文件会被忽略
   tags: 给日志加标签，key形式
   fields: 给日志加字段，key-value形式
   close_*  配置关闭监控文件的时间
       close_inactive: 文件多久不更新后关闭handler
       close_rename: 文件被重命名后关闭handler
       close_remove: 文件被删除后关闭handler
       close_eof:    读取到文件末尾关闭handler, （适用于文件创建后就不再更新的情况）
   clean_* 配置清理registry 里面记录的策略
       clean_inactive:  must be greater than ignore_older + scan_frequency
       clean_removed:   
   scan_frequency： 扫描目录时间
   json 日志文件格式是json的处理
        json.keys_under_root: true
        json.add_error_key: true
        json.message_key: log
   tail_files 从文件尾部开始读取，默认是false, 新文件的第一行读取不到
   multiline
       pattern  匹配的格式
       negate   true:不满足pattern,  false:满足pattern
       match    after， before
       flush
