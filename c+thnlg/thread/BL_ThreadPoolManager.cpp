#include "stdafx.h"
#ifndef CXX_BL_THREADPOOLMANAGER_H
#   include "BL_ThreadPoolManager.h"
#endif

#ifndef CXX_BL_LOG_H
#   include "BL_Log.h"
#endif

#ifndef CXX_APLTHREADNAME_H
#   include "AplThreadName.h"
#endif

static const INT g_nMonitorListMax = 1000;

BL_ThreadPoolManager::BL_ThreadPoolManager(struct BL_ThreadPoolConfig& rcConfig)
: m_cConfig(rcConfig)
, m_bStopFlag(TRUE)
, m_nTaskSchedule(rcConfig.TaskSchedule)
, m_cMonitor(this, g_nMonitorListMax)
, m_pcPendingTask(NULL)
, m_TaskDispatcher(this)
{
    m_nNowCnt = m_cConfig.InitCnt;
    m_cMonitor.RegisterName(XTEXT("THREAD_POOL_MONITOR"));
}

BL_ThreadPoolManager::~BL_ThreadPoolManager()
{
    m_cWorkThreadList.clear();
}

DWORD BL_ThreadPoolManager::GetCurrentSequenceIndex()
{
#ifdef _MSC_VER
    static volatile LONG s_dwSequenceIndex = 0;
    return static_cast<DWORD>(InterlockedIncrement(&s_dwSequenceIndex));
#else
    static volatile DWORD s_dwSequenceIndex = 0;
    return __sync_fetch_and_add(&s_dwSequenceIndex, 1);
#endif
}

DWORD BL_ThreadPoolManager::GetNextTaskID()
{
#ifdef _MSC_VER
    static volatile LONG s_dwTaskID = 0;
    return static_cast<DWORD>(InterlockedIncrement(&s_dwTaskID));
#else
    static volatile DWORD s_dwTaskID = 0;
    return __sync_fetch_and_add(&s_dwTaskID, 1);
#endif
}

VOID BL_ThreadPoolManager::Start()
{
    m_bStopFlag = FALSE;

    m_cMonitor.Start();

    if (m_cWorkThreadList.empty())
    {
        for (INT i = 0; i < m_nNowCnt; ++i)
        {
            BL_ThreadPoolWorkThread* pcThread = new(MEM_Base) BL_ThreadPoolWorkThread(this);
            if (!pcThread)
            {
                BL_Log::Output(BL_LOG_KIND_BASE, "[BL_ThreadPool]Create Work Thread Failed!");
                return;
            }

            pcThread->RegisterName(BL_THREADPOOLWORK_THREAD);
            pcThread->SetPriorityExt(m_cConfig.Priority);
            pcThread->Start();

            BL_SPWORKTHREAD spworkthread(pcThread);

            m_cWorkThreadList.push_back(spworkthread);
        }
    }
    else
    {
        WorkThreadList::iterator it;
        for (it = m_cWorkThreadList.begin(); it != m_cWorkThreadList.end(); ++it) {
            (*it)->Start();
        }
    }

    StartRegistThread();
}

VOID BL_ThreadPoolManager::Stop()
{
    m_bStopFlag = TRUE;

    if (IsAlive())
    {
        // printf("BL_ThreadPoolManager StopThread\n");
        StopThread();
    }

    WorkThreadList::iterator it;
    for (it = m_cWorkThreadList.begin(); it != m_cWorkThreadList.end(); ++it) {
        if ((*it)->IsAlive())
        {
            // printf("WorkThreadList StopThread\n");
            (*it)->Stop();
        }
    }

    if (m_cMonitor.IsAlive())
    {
        // printf("m_cMonitor StopThread\n");
        m_cMonitor.Stop();
    }
}

VOID BL_ThreadPoolManager::Run()
{
    m_pcPendingTask = NULL;

    if (CheckQuit())
    {
        return;
    }

    while (TRUE)
    {
        if (CheckQuit() && GetCountOfWaitListTask() == 0 && !m_pcPendingTask)
        {
            break;
        }

        while (TRUE)
        {
            BL_AutoSync sync(m_cWaitTaskListSyncObj);

            BL_ThreadPoolTask* pcTaskItem = NULL;

            if (NULL != m_pcPendingTask)
            {
                pcTaskItem = m_pcPendingTask;
            }
            else
            {
                if (!GetTaskFromWaitList(pcTaskItem))
                {
                    break;
                }
                m_pcPendingTask = pcTaskItem;
            }

            if (NeedIgnoreTask(pcTaskItem))
            {
                m_pcPendingTask->OnAbandon(BL_TASKABANDON_UNIQUETASK);
                delete pcTaskItem;
                pcTaskItem = NULL;
                m_pcPendingTask = NULL;
                continue;
            }

            BL_SPWORKTHREAD pcWorkThread = m_TaskDispatcher.GetTaskRunThread(pcTaskItem);
            if (!pcWorkThread) {
                break;
            }

            if (NULL != pcTaskItem) {
                pcWorkThread->StartTask(pcTaskItem);
            }

            m_pcPendingTask = NULL;
        }

        DeleteWorkThread();

        // Wait for notify
        Wait(200);
    }
}

CL_BOOL BL_ThreadPoolManager::StartTask(BL_ThreadPoolTask* pcTask)
{
    if (m_bStopFlag || NULL == pcTask)
    {
        return FALSE;
    }
    else {
        pcTask->m_nSequenceIndex = GetCurrentSequenceIndex();

        if (GetCountOfWaitListTask() > m_cConfig.WaitListMax)
        {
            BL_Log::Output(BL_LOG_KIND_BASE, "[BL_ThreadPool]Task Wait List had overflowed!");

            BaseMap<INT, DWORD>::iterator it = m_TaskMissedMap.find(pcTask->GetTaskType());
            if (it == m_TaskMissedMap.end())
            {
                m_TaskMissedMap[pcTask->GetTaskType()] = 1;
            }
            else
            {
                m_TaskMissedMap[pcTask->GetTaskType()] = m_TaskMissedMap[pcTask->GetTaskType()] + 1;
            }

            delete pcTask;

            return FALSE;
        }
        else {
            if (pcTask->IsUnique()) {
                m_cSyncUniqueMap.SynchronizeStart();
                m_cUniqueMap[pcTask->GetTaskType()] = pcTask->GetSequenceIndex();
                m_cSyncUniqueMap.SynchronizeEnd();
            }

            AddTaskToWaitList(pcTask);

            Notify();

            return TRUE;
        }
    }
}

VOID BL_ThreadPoolManager::CancelTaskByType(BL_TASKTYPE nTaskType)
{
    DWORD dwCancelSequenceIndex = GetCurrentSequenceIndex();
    // find task in waitlist and canceled
    {
        BL_AutoSync sync(m_cWaitTaskListSyncObj);
        BaseList<BL_ThreadPoolTask*>::iterator it;
        for (it = m_cWaitTaskList.begin(); it != m_cWaitTaskList.end(); ++it) {
            BL_ThreadPoolTask* pcTask = *it;
            if (NULL != pcTask
                && pcTask->GetTaskType() == nTaskType
                && pcTask->GetSequenceIndex() < dwCancelSequenceIndex) {
                pcTask->m_bCanceledFlag = CL_TRUE;
            }
        }
        if (NULL != m_pcPendingTask
            && m_pcPendingTask->GetTaskType() == nTaskType
            && m_pcPendingTask->GetSequenceIndex() < dwCancelSequenceIndex) {
            m_pcPendingTask->m_bCanceledFlag = CL_TRUE;
        }
    }
    // find task in threads and canceled it
    WorkThreadList cTempThreadList;
    {
        // find threadlist
        BL_AutoSync sync(m_WorkThreadListSyncObj);
        WorkThreadList::iterator it;
        for (it = m_cWorkThreadList.begin(); it != m_cWorkThreadList.end(); ++it) {
            BL_SPWORKTHREAD spworkthread = *it;
            if (NULL != spworkthread && spworkthread->HasTaskOfType(nTaskType)) {
                cTempThreadList.push_back(spworkthread);
            }
        }
    }
    {
        // canceled the task
        WorkThreadList::iterator it;
        for (it = cTempThreadList.begin(); it != cTempThreadList.end(); ++it) {
            BL_SPWORKTHREAD spworkthread = *it;
            if (NULL != spworkthread) {
                spworkthread->CancelTaskByType(nTaskType, dwCancelSequenceIndex);
            }
        }
    }
}

VOID BL_ThreadPoolManager::CancelTaskByID(BL_TASKID nTaskID)
{
    DWORD dwCancelSequenceIndex = GetCurrentSequenceIndex();
    // find task in waitlist and canceled
    {
        BL_AutoSync sync(m_cWaitTaskListSyncObj);
        BaseList<BL_ThreadPoolTask*>::iterator it;
        for (it = m_cWaitTaskList.begin(); it != m_cWaitTaskList.end(); ++it) {
            BL_ThreadPoolTask* pcTask = *it;
            if (NULL != pcTask
                && pcTask->GetTaskID() == nTaskID
                && pcTask->GetSequenceIndex() < dwCancelSequenceIndex) {
                pcTask->m_bCanceledFlag = CL_TRUE;
            }
        }
        if (NULL != m_pcPendingTask
            && m_pcPendingTask->GetTaskID() == nTaskID
            && m_pcPendingTask->GetSequenceIndex() < dwCancelSequenceIndex) {
            m_pcPendingTask->m_bCanceledFlag = CL_TRUE;
        }
    }
    // find task in threads and canceled it
    BL_SPWORKTHREAD cTempThread;
    {
        // find threadlist
        BL_AutoSync sync(m_WorkThreadListSyncObj);
        WorkThreadList::iterator it;
        for (it = m_cWorkThreadList.begin(); it != m_cWorkThreadList.end(); ++it) {
            BL_SPWORKTHREAD spworkthread = *it;
            if (NULL != spworkthread
                && spworkthread->HasTaskOfID(nTaskID)) {
                cTempThread = spworkthread;
                break;
            }
        }
    }
    // canceled the task
    if (NULL != cTempThread.get()) {
        cTempThread->CancelTaskByID(nTaskID, dwCancelSequenceIndex);
    }
}

BL_SPWORKTHREAD BL_ThreadPoolManager::AddWorkThread()
{
    BL_AutoSync sync(m_WorkThreadListSyncObj);

    BL_SPWORKTHREAD spWorkThread;

    if (m_cWorkThreadList.size() >= UINT(m_cConfig.MaxCnt))
    {
        return spWorkThread;
    }

    BL_ThreadPoolWorkThread* pcThread = new(MEM_Base) BL_ThreadPoolWorkThread(this);
    if (!pcThread)
    {
        BL_Log::Output(BL_LOG_KIND_BASE, "[BL_ThreadPool]Create Work Thread Failed!");
        return spWorkThread;
    }

    m_nNowCnt++;

    pcThread->RegisterName(BL_THREADPOOLWORK_THREAD);
    pcThread->SetPriorityExt(m_cConfig.Priority);
    pcThread->StartRegistThread();
    spWorkThread.reset(pcThread);

    m_cWorkThreadList.push_back(spWorkThread);

    return spWorkThread;
}

VOID BL_ThreadPoolManager::DeleteWorkThread()
{
    BL_AutoSync sync(m_WorkThreadListSyncObj);

    if (m_cWorkThreadList.size() <= UINT(m_cConfig.MinCnt))
    {
        return;
    }

    WorkThreadList::iterator it;
    for (it = m_cWorkThreadList.begin(); it != m_cWorkThreadList.end();) {
        BL_SPWORKTHREAD spWorkThread = *it;

        if (spWorkThread->GetRunTaskCnt() > 0)
        {
            ++it;
            continue;
        }

        DWORD dwCurrentTime = ::GetTickCount();
        if ((dwCurrentTime - spWorkThread->GetIdleTimeStamp()) > m_cConfig.KeepTime)
        {
            m_nNowCnt--;
            spWorkThread->StopThread();
            m_cWorkThreadList.erase(it++);

            if (m_cWorkThreadList.size() <= UINT(m_cConfig.MinCnt))
            {
                return;
            }
        }
        else
        {
            ++it;
        }
    }
}

BL_SPWORKTHREAD BL_ThreadPoolManager::GetThread(BL_TaskDispatcher * pObj, PTRFUN fp)
{
    BL_AutoSync sync(m_WorkThreadListSyncObj);

    BL_SPWORKTHREAD spWorkThread;
    if (NULL != pObj && NULL != fp) {
        return (pObj->*fp)(m_cWorkThreadList);
    }
    else {
        return spWorkThread;
    }
}

CL_BOOL BL_ThreadPoolManager::NeedIgnoreTask(BL_ThreadPoolTask* TaskItem)
{
    BL_AutoSync sync(m_cSyncUniqueMap);

    if (m_cUniqueMap.size() == 0)
    {
        return FALSE;
    }

    StampTimeMapTaskId::iterator it = m_cUniqueMap.find(TaskItem->GetTaskType());
    if (it == m_cUniqueMap.end())
    {
        return FALSE;
    }
    // if this task is latest unique task, it cannot be ignored by the BL_ThreadPoolManager.
    if (TaskItem->GetSequenceIndex() == it->second)
    {
        m_cUniqueMap.erase(it);
        return FALSE;
    }

    return TRUE;
}

CL_BOOL BL_ThreadPoolManager::GetTaskFromWaitList(BL_ThreadPoolTask* &TaskItem)
{
    if (m_cWaitTaskList.size()>0)
    {
        TaskItem = m_cWaitTaskList.front();
        m_cWaitTaskList.pop_front();
        return CL_TRUE;
    }
    else
    {
        return CL_FALSE;
    }
}

CL_BOOL BL_ThreadPoolManager::AddTaskToWaitList(BL_ThreadPoolTask* TaskItem)
{
    BL_AutoSync sync(m_cWaitTaskListSyncObj);
    m_cWaitTaskList.push_back(TaskItem);
    return CL_TRUE;
}

ULONG BL_ThreadPoolManager::GetCountOfWaitListTask()
{
    BL_AutoSync sync(m_cWaitTaskListSyncObj);
    return m_cWaitTaskList.size();
}

BL_String BL_ThreadPoolManager::dumpStatus()
{
    BL_String sStatus;

    sStatus += BL_String("\nThreadPool Running TaskList:\n");

    BL_AutoSync sync(m_WorkThreadListSyncObj);
    WorkThreadList::iterator it;
    for (it = m_cWorkThreadList.begin(); it != m_cWorkThreadList.end(); ++it) {
        // sStatus += BL_String("    WorkThread\n");
        BL_SPWORKTHREAD spworkthread = *it;
        if (NULL != spworkthread) {
            BL_String sClassName;
            DWORD dwTaskID;
            DWORD dwStat;
            DWORD dwRunTC;
            spworkthread->GetRunTaskInfo(sClassName, dwTaskID, dwStat, dwRunTC);
            if (sClassName.GetLength() > 0) {
                BL_String Info;
                if (dwStat == BL_THREADPOOL_TASKSTAT_RUNFINISH) {
                    Info.Format("    * WorkThread Task ID[%d] Class[%s] Stat[FINISH] RunTC[%d]\n",
                        dwTaskID, sClassName.GetString(), dwRunTC);
                }
                else {
                    Info.Format("    * WorkThread Task ID[%d] Class[%s] Stat[START] RunTC[%d]\n",
                        dwTaskID, sClassName.GetString(), dwRunTC);
                }

                sStatus += Info;
            }
        }
    }

    sStatus += BL_String("\n");

    return sStatus;
}
/* EOF */