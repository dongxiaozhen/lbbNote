#include "stdafx.h"

#ifndef CXX_BL_THREADPOOLMANAGER_H
#   include "BL_ThreadPoolManager.h"
#endif

#ifndef CXX_BL_THREADPOOLWORKTHREAD_H
#   include "BL_ThreadPoolWorkThread.h"
#endif

BL_ThreadPoolWorkThread::BL_ThreadPoolWorkThread(BL_ThreadPoolManager* pcManager)
: m_pcManager(pcManager)
, m_StopFlag(CL_FALSE)
, m_pCurrentRunTask(NULL)
, m_dwRunStartCount(0)
, m_dwRunFinishCount(0)
, m_dwRunStat(BL_THREADPOOL_TASKSTAT_RUNFINISH)
, m_sRunTaskClassName("None")
{
    m_dwIdleTimeStamp = ::GetTickCount();
}

BL_ThreadPoolWorkThread::~BL_ThreadPoolWorkThread()
{

}

CL_BOOL BL_ThreadPoolWorkThread::StartTask(BL_ThreadPoolTask* pcTask)
{
    INT nGroupId = pcTask->GetTaskGroupId();

    // update group map id map
    m_sGroupIdMapSyncObj.SynchronizeStart();

    GroupIdMap::iterator it = m_cGroupIdMap.find(nGroupId);
    if (it != m_cGroupIdMap.end()) {
        it->second = it->second + 1;
    }
    else {
        m_cGroupIdMap[nGroupId] = 1;
    }

    m_sGroupIdMapSyncObj.SynchronizeEnd();


    m_cSyncObj.SynchronizeStart();

    m_cRunTaskList.push_back(pcTask);

    m_cSyncObj.SynchronizeEnd();

    Notify();

    return TRUE;
}

INT BL_ThreadPoolWorkThread::GetRunTaskCnt()
{
    BL_AutoSync sync(m_cSyncObj);

    INT iWaitListCount = m_cRunTaskList.size();
    if (NULL != m_pCurrentRunTask) {
        return iWaitListCount + 1;
    }
    else {
        return iWaitListCount;
    }
}

DWORD  BL_ThreadPoolWorkThread::GetIdleTimeStamp()
{
    return m_dwIdleTimeStamp;
}

CL_BOOL BL_ThreadPoolWorkThread::hasTaskOfGroup(INT nGroupId)
{
    BL_AutoSync sync(m_sGroupIdMapSyncObj);

    GroupIdMap::iterator it = m_cGroupIdMap.find(nGroupId);
    if (it != m_cGroupIdMap.end()) {
        if (it->second > 0) {
            return CL_TRUE;
        }
        else {
            return CL_FALSE;
        }
    }
    else {
        return CL_FALSE;
    }
}

VOID BL_ThreadPoolWorkThread::Run()
{
    if (CheckQuit())
    {
        return;
    }

    while (TRUE)
    {
        if (CheckQuit())
        {
            CL_BOOL flag = CL_FALSE;
            m_cSyncObj.SynchronizeStart();
            flag = m_cRunTaskList.empty();
            m_cSyncObj.SynchronizeEnd();

            if (flag)
            {
                break;
            }
        }

        while (GetRunTaskCnt() > 0)
        {
            m_cSyncObj.SynchronizeStart();
            m_pCurrentRunTask = m_cRunTaskList.front();
            m_cRunTaskList.pop_front();
            m_cSyncObj.SynchronizeEnd();

            m_sRunSyncObj.SynchronizeStart();
            // Needn't be locked by m_cSyncObj, for the current task point is deleted by this thread.
            if (m_pCurrentRunTask != NULL)
            {
                if (m_pCurrentRunTask->IsCanceled()) {
                    m_pCurrentRunTask->OnAbandon(BL_TASKABANDON_CANCELBYUSER);
                }
                else {
                    RunTask(m_pCurrentRunTask);
                }

            }
            m_sRunSyncObj.SynchronizeEnd();

            m_dwIdleTimeStamp = ::GetTickCount();


            // update group map id map
            m_sGroupIdMapSyncObj.SynchronizeStart();
            if (NULL != m_pCurrentRunTask) {
                INT nGroupId = m_pCurrentRunTask->GetTaskGroupId();
                GroupIdMap::iterator it = m_cGroupIdMap.find(nGroupId);
                if (it != m_cGroupIdMap.end()) {
                    it->second = it->second - 1;
                    if (0 == it->second) {
                        m_cGroupIdMap.erase(it);
                    }
                }
            }
            m_sGroupIdMapSyncObj.SynchronizeEnd();

            m_cSyncObj.SynchronizeStart();
            if (NULL != m_pCurrentRunTask) {
                delete m_pCurrentRunTask;
                m_pCurrentRunTask = NULL;
            }
            m_cSyncObj.SynchronizeEnd();
        }

        // Notify manage thread
        m_pcManager->Notify();

        // Wait for notify
        if (!m_StopFlag)
        {
            Wait();
        }
    }
}

VOID BL_ThreadPoolWorkThread::RunTask(BL_ThreadPoolTask* pcTask)
{
    if (NULL == pcTask || NULL == m_pcManager || NULL == m_pcManager->GetMoniter())
    {
        return;
    }
    BL_TaskRawInfo cRawInfo;

    cRawInfo.TaskType = pcTask->GetTaskType();
    strncpy(cRawInfo.TaskClassName, pcTask->GetTaskClassName(), MAXLEN_TASKCLASSNAME);

    cRawInfo.TickCountBefore = ::GetTickCount();
    cRawInfo.ThreadTimeBefore = ::GetThreadTime();

    SetRunTaskInfo(pcTask->GetTaskClassName(), pcTask->GetTaskID(), BL_THREADPOOL_TASKSTAT_RUNSTART);

    INT errorCode = pcTask->Run();

    pcTask->Return(errorCode);

    SetRunTaskInfo(pcTask->GetTaskClassName(), pcTask->GetTaskID(), BL_THREADPOOL_TASKSTAT_RUNFINISH);

    cRawInfo.TickCountAfter = ::GetTickCount();
    cRawInfo.ThreadTimeAfter = ::GetThreadTime();

    m_pcManager->GetMoniter()->PostTaskRawInfo(cRawInfo);
}

VOID BL_ThreadPoolWorkThread::Start()
{
    StartRegistThread();
}

VOID BL_ThreadPoolWorkThread::Stop()
{
    m_StopFlag = CL_TRUE;
    StopThread();
}

CL_BOOL BL_ThreadPoolWorkThread::HasTaskOfID(BL_TASKID nTaskID)
{
    BL_AutoSync sync(m_cSyncObj);

    BaseList<BL_ThreadPoolTask*>::iterator it;
    for (it = m_cRunTaskList.begin(); it != m_cRunTaskList.end(); ++it) {
        BL_ThreadPoolTask* pcTask = *it;
        if (NULL != pcTask && pcTask->GetTaskID() == nTaskID) {
            return CL_TRUE;
        }
    }

    if (NULL != m_pCurrentRunTask && m_pCurrentRunTask->GetTaskID() == nTaskID) {
        return CL_TRUE;
    }

    return CL_FALSE;
}

VOID BL_ThreadPoolWorkThread::CancelTaskByID(BL_TASKID nTaskID,  DWORD dwCancelSequenceIndex)
{
    CL_BOOL bRunTaskCancelFlg = CL_FALSE;
    // find task in waitlist and canceled
    {
        BL_AutoSync sync(m_cSyncObj);
        BaseList<BL_ThreadPoolTask*>::iterator it;
        for (it = m_cRunTaskList.begin(); it != m_cRunTaskList.end(); ++it) {
            BL_ThreadPoolTask* pcTask = *it;
            if (NULL != pcTask
                && pcTask->GetTaskID() == nTaskID
                && pcTask->GetSequenceIndex() < dwCancelSequenceIndex) {
                pcTask->m_bCanceledFlag = CL_TRUE;
            }
        }
        // check the current run task and set the cancel flag,
        // the task is running currently or will be canceld.
        if (NULL != m_pCurrentRunTask) {
            if (m_pCurrentRunTask->GetTaskID() == nTaskID
                && m_pCurrentRunTask->GetSequenceIndex() < dwCancelSequenceIndex) {
                bRunTaskCancelFlg = CL_TRUE;
                m_pCurrentRunTask->m_bCanceledFlag = CL_TRUE;
            }
        }
    }
    if (bRunTaskCancelFlg) {
        // cancel current runtask
        // for preventing task waitlist operation chock, must lock taskrunning first.
        BL_AutoSync sync1(m_sRunSyncObj);
        BL_AutoSync sync2(m_cSyncObj);
        if (NULL != m_pCurrentRunTask) {
            if (m_pCurrentRunTask->GetTaskID() == nTaskID
                && m_pCurrentRunTask->GetSequenceIndex() < dwCancelSequenceIndex) {
                m_pCurrentRunTask->m_bCanceledFlag = CL_TRUE;
            }
        }
    }
}

CL_BOOL BL_ThreadPoolWorkThread::HasTaskOfType(BL_TASKTYPE nTaskType)
{
    BL_AutoSync sync(m_cSyncObj);

    BaseList<BL_ThreadPoolTask*>::iterator it;
    for (it = m_cRunTaskList.begin(); it != m_cRunTaskList.end(); ++it) {
        BL_ThreadPoolTask* pcTask = *it;
        if (NULL != pcTask && pcTask->GetTaskType() == nTaskType) {
            return CL_TRUE;
        }
    }

    if (NULL != m_pCurrentRunTask && m_pCurrentRunTask->GetTaskType() == nTaskType) {
        return CL_TRUE;
    }

    return CL_FALSE;
}

VOID BL_ThreadPoolWorkThread::CancelTaskByType(BL_TASKTYPE nTaskType, DWORD dwCancelSequenceIndex)
{
    CL_BOOL bRunTaskCancelFlg = CL_FALSE;
    // find task in waitlist and canceled
    {
        BL_AutoSync sync(m_cSyncObj);
        BaseList<BL_ThreadPoolTask*>::iterator it;
        for (it = m_cRunTaskList.begin(); it != m_cRunTaskList.end(); ++it) {
            BL_ThreadPoolTask* pcTask = *it;
            if (NULL != pcTask
                && pcTask->GetTaskType() == nTaskType
                && pcTask->GetSequenceIndex() < dwCancelSequenceIndex) {
                pcTask->m_bCanceledFlag = CL_TRUE;
            }
        }
        // check the current run task and set the cancel flag,
        // the task is running currently or will be canceld.
        if (NULL != m_pCurrentRunTask) {
            if (m_pCurrentRunTask->GetTaskType() == nTaskType
                && m_pCurrentRunTask->GetSequenceIndex() < dwCancelSequenceIndex) {
                bRunTaskCancelFlg = CL_TRUE;
                m_pCurrentRunTask->m_bCanceledFlag = CL_TRUE;
            }
        }
    }
    // cancel current runtask
    if (bRunTaskCancelFlg) {
        // for preventing task waitlist operation chock, must lock taskrunning first.
        BL_AutoSync sync1(m_sRunSyncObj);
        BL_AutoSync sync2(m_cSyncObj);
        if (NULL != m_pCurrentRunTask) {
            if (m_pCurrentRunTask->GetTaskType() == nTaskType
                && m_pCurrentRunTask->GetSequenceIndex() < dwCancelSequenceIndex) {
                m_pCurrentRunTask->m_bCanceledFlag = CL_TRUE;
            }
        }
    }
}

VOID BL_ThreadPoolWorkThread::GetRunTaskInfo(BL_String & sClassName, DWORD & dwID, DWORD & dwStat, DWORD & dwRunCount)
{
    BL_AutoSync sync(m_sDumpSyncObj);
    dwStat = m_dwRunStat;
    dwID = m_dwRunTaskID;
    sClassName = m_sRunTaskClassName;
    if (dwStat == BL_THREADPOOL_TASKSTAT_RUNSTART) {
        dwRunCount = ::GetTickCount() - m_dwRunStartCount;
    }
    else if (dwStat == BL_THREADPOOL_TASKSTAT_RUNFINISH) {
        dwRunCount = m_dwRunFinishCount - m_dwRunStartCount;
    }
}

VOID BL_ThreadPoolWorkThread::SetRunTaskInfo(BL_String sClassName, DWORD dwID, DWORD dwStat)
{
    BL_AutoSync sync(m_sDumpSyncObj);
    m_dwRunStat = dwStat;
    m_dwRunTaskID = dwID;
    m_sRunTaskClassName = sClassName;
    if (dwStat == BL_THREADPOOL_TASKSTAT_RUNSTART) {
        m_dwRunStartCount = ::GetTickCount();
    }
    else if (dwStat == BL_THREADPOOL_TASKSTAT_RUNFINISH) {
        m_dwRunFinishCount = ::GetTickCount();
    }
}

/* EOF */