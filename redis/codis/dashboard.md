
服务启动后会监控redis, proxy,slot 的状态
    proxy 启动后，会发送fill_slot是api 更新代理的槽位状态。     
    slots 变化后，从pending 一直监控到finished, for循环所有的slots, 一秒一次
    redis 通过redis借口查看redis状态并显示在fe上

