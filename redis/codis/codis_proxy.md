proxy 监听主从切换
    proxy本身会监听sentinle集群的+switch-master事件，该事件发出，意味着redis集群主机出现问题，sentinel集群开始进行选举并切换主机，proxy监听了sentinel的主从切换事件，收到主从切换事件之后，proxy会做一个动作，就是把所有sentinel上的集群所感知的当前认为的主机拉取出来，选取过半sentinel认为的主机当作目前的集群主机。`
    proxy.RewatchSentinels -> proxy.SwitchMasters


proxy 初始化slot
    proxy 启动会连接dashboard,调用OnlineProxy，然后dashboard会调用proxy接口通知消息：
    proxy -> dashboard: "/api/topom/proxy/online/%s/%s" OnlineProxy
    dashboard -> store: 从zookeeper里查询信息
    dashboard -> proxy: "/api/proxy/model"
    dashboard -> proxy: "/api/proxy/fillslots/%s"
    dashboard -> proxy: "/api/proxy/start/%s"
    dashboard -> proxy: "/api/proxy/sentinels/%s"


dashboard 服务
    rewatchSentinels
    RefreshRedisStats
    RefreshProxyStats
    ProcessSlotAction
    ProcessSyncAction

dashboard SlotsRebalance // 槽位平衡

ProcessSlotAction
    SlotActionPrepareFilter 处理槽位的准备阶段，一直到迁移阶段
    processSlotAction  处理槽位的迁移阶段到迁移完成
        -> newSlotActionExecutor 
        -> SlotActionComplete
``` go
    switch m.Action.State {
	case models.ActionNothing, models.ActionPending:
		slot.BackendAddr = ctx.getGroupMaster(m.GroupId)
		slot.BackendAddrGroupId = m.GroupId
		slot.ReplicaGroups = ctx.toReplicaGroups(m.GroupId, p)
	case models.ActionPreparing:
		slot.BackendAddr = ctx.getGroupMaster(m.GroupId)
		slot.BackendAddrGroupId = m.GroupId
	case models.ActionPrepared:
		fallthrough
	case models.ActionMigrating:
		slot.BackendAddr = ctx.getGroupMaster(m.Action.TargetId)
		slot.BackendAddrGroupId = m.Action.TargetId
		slot.MigrateFrom = ctx.getGroupMaster(m.GroupId)
		slot.MigrateFromGroupId = m.GroupId
	case models.ActionFinished:
		slot.BackendAddr = ctx.getGroupMaster(m.Action.TargetId)
		slot.BackendAddrGroupId = m.Action.TargetId
	default:
		log.Panicf("slot-[%d] action state is invalid:\n%s", m.Id, m.Encode())
	}
```
