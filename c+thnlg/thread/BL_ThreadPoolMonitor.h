#ifndef CXX_BL_THREADPOOLMONITOR_H
#define CXX_BL_THREADPOOLMONITOR_H

#include "MEM_list.h"
#include "MEM_map.h"

#ifndef CXX_BL_THREAD_H
#   include "BL_Thread.h"
#endif

#ifndef CXX_BL_AUTOSYNC_H
#   include "BL_AutoSync.h"
#endif

#define MAXLEN_TASKCLASSNAME 64

struct BL_TaskRawInfo
{
    INT     TaskType;
    CHAR    TaskClassName[MAXLEN_TASKCLASSNAME];
    DWORD   TickCountBefore;
    DWORD   TickCountAfter;
    DWORD   ThreadTimeBefore;
    DWORD   ThreadTimeAfter;
};

struct BL_TaskInfo
{
    CHAR    TaskClassName[MAXLEN_TASKCLASSNAME];
    DWORD   TickCountMin;
    DWORD   TickCountMax;
    FLOAT   TickCountAverage;
    DWORD   ThreadTimeMin;
    DWORD   ThreadTimeMax;
    FLOAT   ThreadTimeAverage;
    DWORD   RunCount;
};

class BL_ThreadPoolManager;

class BL_ThreadPoolMonitor : public BL_Thread
{
public:
    BL_ThreadPoolMonitor(BL_ThreadPoolManager* pcManager, INT nListMax);

    virtual ~BL_ThreadPoolMonitor();

    virtual VOID Run();

    VOID PostTaskRawInfo(BL_TaskRawInfo& cInfo);

    VOID OutPutInfo();

    VOID Start();

    VOID Stop();

private:
    BL_ThreadPoolMonitor(const BL_ThreadPoolMonitor&);
    BL_ThreadPoolMonitor& operator= (const BL_ThreadPoolMonitor&);


    CL_BOOL GetTaskRawInfo(BL_TaskRawInfo & cInfo);
    CL_BOOL AddTaskRawInfo(BL_TaskRawInfo & cInfo);
    ULONG GetCountOfTaskRawInfo();

    typedef BaseMap<INT, BL_TaskInfo>::type TaskInfoMap;

    INT                 m_nListMax;
    TaskInfoMap             m_cTaskInfo;
    BL_ThreadPoolManager*       m_pcManager;

    BaseList<BL_TaskRawInfo>::type m_cTaskRawInfo;
    BL_SyncObject       m_cSyncObj;

    // monitor info
    DWORD m_IgnoreRawInfoCount;
};

#endif /* >>CXX_BL_THREADPOOLMONITOR_H<< */
/* EOF */