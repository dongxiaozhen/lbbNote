#ifndef CXX_BL_THREADPOOLMANAGER_H
#define CXX_BL_THREADPOOLMANAGER_H

#ifndef CXX_BL_THREAD_H
#   include "BL_Thread.h"
#endif

#ifndef CXX_BL_AUTOSYNC_H
#   include "BL_AutoSync.h"
#endif

#ifndef CXX_CL_WAITOBJ_H
#   include "CL_WaitObj.h"
#endif

#ifndef CXX_BL_THREADPOOL_H
#   include "BL_ThreadPool.h"
#endif

#ifndef CXX_BL_THREADPOOLMONITOR_H
#   include "BL_ThreadPoolMonitor.h"
#endif

#ifndef CXX_BL_THREADPOOLWORKTHREAD_H
#   include "BL_ThreadPoolWorkThread.h"
#endif

#ifndef CXX_BL_TASKDISPATCHER_H
#   include "BL_TaskDispatcher.h"
#endif

#ifndef CXX_BL_STRING_H
#   include "BL_String.h"
#endif

#include "MEM_map.h"
#include "MEM_list.h"


class BL_ThreadPoolManager : public BL_Thread
{
public:
    friend class BL_ThreadPoolWorkThread;
    friend class BL_TaskDispatcher;

    typedef BaseMap<INT, DWORD>::type        StampTimeMapTaskId;

    BL_ThreadPoolManager(struct BL_ThreadPoolConfig& rcConfig);

    virtual ~BL_ThreadPoolManager();

    static DWORD GetCurrentSequenceIndex();
    static DWORD GetNextTaskID();

    VOID Start();

    VOID Stop();

    virtual VOID Run();

    CL_BOOL StartTask(BL_ThreadPoolTask* pcTask);

    VOID CancelTaskByType(BL_TASKTYPE nTaskType);
    VOID CancelTaskByID(BL_TASKID nTaskID);

    BL_ThreadPoolMonitor* GetMoniter() { return &m_cMonitor; }
    const BL_ThreadPoolConfig GetConfig() { return m_cConfig; }
    const INT GetCurrentWorkThreadCount() { return m_nNowCnt; }

    BaseMap<INT, DWORD>::type& GetMissedTaskMap()
    {
        return m_TaskMissedMap;
    }

    BL_String dumpStatus();

private:

    BL_SPWORKTHREAD AddWorkThread();
    VOID DeleteWorkThread();

    CL_BOOL NeedIgnoreTask(BL_ThreadPoolTask* TaskItem);

    CL_BOOL GetTaskFromWaitList(BL_ThreadPoolTask* &TaskItem);
    CL_BOOL AddTaskToWaitList(BL_ThreadPoolTask* TaskItem);
    ULONG GetCountOfWaitListTask();

    typedef BL_SPWORKTHREAD (BL_TaskDispatcher::*PTRFUN)(WorkThreadList & cThreadList);
    BL_SPWORKTHREAD GetThread(BL_TaskDispatcher * pObj, PTRFUN fp);

    BL_ThreadPoolTask*                  m_pcPendingTask; // save the pending task to post to the workthread

    BaseList<BL_ThreadPoolTask*>::type  m_cWaitTaskList; // task wait list
    BL_SyncObject                       m_cWaitTaskListSyncObj;

    WorkThreadList                      m_cWorkThreadList;
    BL_SyncObject                       m_WorkThreadListSyncObj;

    StampTimeMapTaskId                  m_cUniqueMap;    // task with unique map
    BL_SyncObject                       m_cSyncUniqueMap;    // Sync for m_cUniqueMap

    BaseMap<INT, DWORD>::type           m_TaskMissedMap;

    BL_ThreadPoolConfig                 m_cConfig;
    INT                                 m_nNowCnt;    // default thread count in a threadpool
    CL_BOOL                             m_bStopFlag;    // whether thread pool stoped
    BL_TaskSchedulePolicy               m_nTaskSchedule;    // FIFO, IFO or RIO
    BL_ThreadPoolMonitor                m_cMonitor;    // performance monitor for thread pool
    BL_TaskDispatcher                   m_TaskDispatcher;

    BL_ThreadPoolManager(const BL_ThreadPoolManager& c);
    BL_ThreadPoolManager& operator=(const BL_ThreadPoolManager& c);
};

#endif /* >>CXX_BL_THREADPOOLMANAGER_H<< */
/* EOF */