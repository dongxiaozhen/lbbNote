#include "stdafx.h"
#ifndef CXX_BL_THREADPOOLTASK_H
#   include "BL_ThreadPoolTask.h"
#endif
#include "BL_ThreadPoolManager.h"

BL_ThreadPoolTask::BL_ThreadPoolTask(CL_BOOL bUnique, INT nGroupId, INT nPrority)
: m_nGroupId(nGroupId)
, m_nPrority(nPrority)
, m_bUnique(bUnique)
, m_bCanceledFlag(CL_FALSE)
, m_nTaskId(-1)
, m_nSequenceIndex(0)
{
    m_nTaskId = BL_ThreadPoolManager::GetNextTaskID();
}

BL_ThreadPoolTask::~BL_ThreadPoolTask()
{

}

INT BL_ThreadPoolTask::Run()
{
    return 0;
}

VOID BL_ThreadPoolTask::Return(const INT nErrorCode)
{

}

VOID BL_ThreadPoolTask::OnAbandon(const INT nReason)
{

}

BL_TASKID BL_ThreadPoolTask::GetTaskID()
{
    return m_nTaskId;
}

INT BL_ThreadPoolTask::GetTaskGroupId()
{
    return m_nGroupId;
}

INT BL_ThreadPoolTask::GetTaskPriority()
{
    return m_nPrority;
}

DWORD BL_ThreadPoolTask::GetSequenceIndex()
{
    return m_nSequenceIndex;
}

CL_BOOL BL_ThreadPoolTask::IsUnique()
{
    return m_bUnique;
}

CL_BOOL BL_ThreadPoolTask::IsCanceled()
{
    return m_bCanceledFlag;
}
/* EOF */