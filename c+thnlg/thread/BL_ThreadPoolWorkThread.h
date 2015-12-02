#ifndef CXX_BL_THREADPOOLWORKTHREAD_H
#define CXX_BL_THREADPOOLWORKTHREAD_H

#ifndef CXX_BL_THREAD_H
#   include "BL_Thread.h"
#endif
#ifndef CXX_BL_STRING_H
#   include "BL_String.h"
#endif

#include "MEM_list.h"
#include "MEM_map.h"

#define BL_THREADPOOL_TASKSTAT_RUNSTART 0
#define BL_THREADPOOL_TASKSTAT_RUNFINISH 1

class BL_ThreadPoolManager;

class BL_ThreadPoolWorkThread : public BL_Thread
{
public:
    friend class BL_ThreadPoolTask;

    BL_ThreadPoolWorkThread(BL_ThreadPoolManager* pcManager);

    virtual ~BL_ThreadPoolWorkThread();

    CL_BOOL StartTask(BL_ThreadPoolTask* pcTask);

    INT GetRunTaskCnt();

    DWORD GetIdleTimeStamp();

    CL_BOOL hasTaskOfGroup(INT nGroupId);

    VOID AddGroup(INT nGroupId);

    VOID Run();
    VOID RunTask(BL_ThreadPoolTask* pcTask);

    VOID Start();

    VOID Stop();

    CL_BOOL HasTaskOfID(BL_TASKID nTaskID);
    VOID CancelTaskByID(BL_TASKID nTaskID, DWORD dwCancelSequenceIndex);

    CL_BOOL HasTaskOfType(BL_TASKTYPE nTaskType);
    VOID CancelTaskByType(BL_TASKTYPE nTaskType, DWORD dwCancelSequenceIndex);

    VOID GetRunTaskInfo(BL_String & sClassName, DWORD & dwID, DWORD & dwStat, DWORD & dwRunCount);
    VOID SetRunTaskInfo(BL_String sClassName, DWORD dwID, DWORD dwStat);

private:
    typedef BaseList<BL_ThreadPoolTask*>::type    TaskItemList;
    typedef BaseMap<INT, INT>::type               GroupIdMap;


    TaskItemList                             m_cRunTaskList;
    BL_SyncObject                            m_cSyncObj;

    BL_ThreadPoolTask*                       m_pCurrentRunTask;
    BL_SyncObject                            m_sRunSyncObj;

    BL_ThreadPoolManager*                    m_pcManager;
    GroupIdMap                               m_cGroupIdMap;
    BL_SyncObject                            m_sGroupIdMapSyncObj;

    DWORD                                    m_dwIdleTimeStamp;
    CL_BOOL                                  m_StopFlag;

    BL_ThreadPoolWorkThread(const BL_ThreadPoolWorkThread& c);
    BL_ThreadPoolWorkThread& operator=(const BL_ThreadPoolWorkThread& c);

    BL_String                                m_sRunTaskClassName;
    DWORD                                    m_dwRunTaskID;
    DWORD                                    m_dwRunStat;
    DWORD                                    m_dwRunStartCount;
    DWORD                                    m_dwRunFinishCount;
    BL_SyncObject                            m_sDumpSyncObj;
};

#endif /* >>CXX_BL_THREADPOOLWORKTHREAD_H<< */
/* EOF */
