#include "stdafx.h"

#ifndef CXX_BL_THREADPOOLMANAGER_H
#   include "BL_ThreadPoolManager.h"
#endif

#ifndef CXX_BL_THREADPOOLMONITOR_H
#   include "BL_ThreadPoolMonitor.h"
#endif

#ifndef CXX_BL_LOG_H
#   include "BL_Log.h"
#endif
#if defined(_FOR_ANDROID_) || defined(NUTSHELL)
#include <android/log.h>
#endif
#ifndef CXX_BL_APLSYSTEM_H
#include "BL_AplSystem.h"
#endif
#if defined(NUTSHELL)
#include "ncore/NCLog.h"
#endif

void PERFORMANCELOG_FUNC(const char* Fmt, ...)
{
    va_list ap;
    char TempStr[512];
    memset(TempStr, 0, 512);
    va_start(ap, Fmt);
    vsprintf(TempStr, Fmt, ap);
    va_end(ap);
    char OutputBuf[512]= { 0 };
    sprintf(OutputBuf, "%s", TempStr);
    // FILE * fp = fopen("/tmp/log-bl.txt","a+");
    // fwrite(TempStr,strlen(TempStr),1,fp);
    // fclose(fp);
    BL_AplSystem cAplSystem;
    std::string logtag = std::string(cAplSystem.GetProcessName()) + std::string("_ThreadPoolMonitor");
#if defined(NUTSHELL)
    nutshell::NCLog::output(nutshell::NC_LOG_INFO, logtag.c_str(), OutputBuf);
#elif defined(_FOR_ANDROID_)
    __android_log_print(ANDROID_LOG_DEBUG, logtag.c_str(), OutputBuf);
#else
    printf("[%s]%s\n", logtag.c_str(), OutputBuf);
#endif
}

#define PERFORMANCELOG(...) PERFORMANCELOG_FUNC(__VA_ARGS__)

BL_ThreadPoolMonitor::BL_ThreadPoolMonitor(BL_ThreadPoolManager* pcManager, INT nListMax) :
m_nListMax(nListMax),
m_pcManager(pcManager)
{
    m_IgnoreRawInfoCount = 0;
}

BL_ThreadPoolMonitor::~BL_ThreadPoolMonitor()
{

}

VOID BL_ThreadPoolMonitor::Run()
{
    // check thread stop or not when thread run
    if (CheckQuit() == TRUE)
    {
        /* function end */
        return;
    }

    // Main loop
    while (TRUE)
    {
        // check thread stop or not
        if (CheckQuit() == TRUE)
        {
            /* exits from the loop */
            break;
        }

        BL_TaskRawInfo cRawInfo;

        while (GetTaskRawInfo(cRawInfo))
        {
            DWORD dwTickCount;
            if (cRawInfo.TickCountAfter < cRawInfo.TickCountBefore)
            {
                dwTickCount = DWORD(-1) - cRawInfo.TickCountBefore + cRawInfo.TickCountAfter;
            }
            else
            {
                dwTickCount = cRawInfo.TickCountAfter - cRawInfo.TickCountBefore;
            }

            DWORD dwThreadTime;
            if (cRawInfo.ThreadTimeAfter < cRawInfo.ThreadTimeBefore)
            {
                dwThreadTime = DWORD(-1) - cRawInfo.ThreadTimeBefore + cRawInfo.ThreadTimeAfter;
            }
            else
            {
                dwThreadTime = cRawInfo.ThreadTimeAfter - cRawInfo.ThreadTimeBefore;
            }

            TaskInfoMap::iterator it = m_cTaskInfo.find(cRawInfo.TaskType);
            if (it != m_cTaskInfo.end())
            {
                BL_TaskInfo& cTaskInfo = it->second;

                strncpy(cTaskInfo.TaskClassName, cRawInfo.TaskClassName, MAXLEN_TASKCLASSNAME);
                cTaskInfo.TickCountMin = (dwTickCount < cTaskInfo.TickCountMin) ? dwTickCount : cTaskInfo.TickCountMin;
                cTaskInfo.TickCountMax = (dwTickCount > cTaskInfo.TickCountMax) ? dwTickCount : cTaskInfo.TickCountMax;
                cTaskInfo.TickCountAverage = (cTaskInfo.TickCountAverage * cTaskInfo.RunCount + dwTickCount) / (cTaskInfo.RunCount + 1);

                cTaskInfo.ThreadTimeMin = (dwThreadTime < cTaskInfo.ThreadTimeMin) ? dwThreadTime : cTaskInfo.ThreadTimeMin;
                cTaskInfo.ThreadTimeMax = (dwThreadTime > cTaskInfo.ThreadTimeMax) ? dwThreadTime : cTaskInfo.ThreadTimeMax;
                cTaskInfo.ThreadTimeAverage = (cTaskInfo.ThreadTimeAverage * cTaskInfo.RunCount + dwThreadTime) / (cTaskInfo.RunCount + 1);

                cTaskInfo.RunCount = cTaskInfo.RunCount + 1;
            }
            else
            {
                BL_TaskInfo cTaskInfo;

                strncpy(cTaskInfo.TaskClassName, cRawInfo.TaskClassName, MAXLEN_TASKCLASSNAME);
                cTaskInfo.TickCountMin = dwTickCount;
                cTaskInfo.TickCountMax = dwTickCount;
                cTaskInfo.TickCountAverage = dwTickCount;

                cTaskInfo.ThreadTimeMin = dwThreadTime;
                cTaskInfo.ThreadTimeMax = dwThreadTime;
                cTaskInfo.ThreadTimeAverage = dwThreadTime;

                cTaskInfo.RunCount = 1;

                m_cTaskInfo[cRawInfo.TaskType] = cTaskInfo;
            }
        }

        Wait(5000);
        OutPutInfo();
    }
    OutPutInfo();
}

VOID BL_ThreadPoolMonitor::Start()
{
    StartRegistThread();
}

VOID BL_ThreadPoolMonitor::Stop()
{
    StopThread();
}

VOID BL_ThreadPoolMonitor::PostTaskRawInfo(BL_TaskRawInfo& cInfo)
{
    if (GetCountOfTaskRawInfo() > m_nListMax)
    {
        m_IgnoreRawInfoCount++;
        BL_Log::Output(BL_LOG_KIND_BASE, "[BL_ThreadPool]Thread Pool Monitor Request Quene had overflowed!");
        return;
    }

    AddTaskRawInfo(cInfo);
    Notify();
}

VOID BL_ThreadPoolMonitor::OutPutInfo()
{
    if (NULL == m_pcManager)
    {
        return;
    }
    static unsigned int s_lastTickCount = 0;
    unsigned int uiNowTickCount = ::GetTickCount();
    if (uiNowTickCount - s_lastTickCount < 5000)
    {
        return;
    }

    DWORD TotalMissedTaskCount = 0;
    BaseMap<INT, DWORD>::type& MissedMap = m_pcManager->GetMissedTaskMap();
    for (BaseMap<INT, DWORD>::iterator it = MissedMap.begin(); it != MissedMap.end(); ++it) {
        TotalMissedTaskCount = TotalMissedTaskCount + it->second;
    }
    PERFORMANCELOG("-------------*** BL_ThreadPoolMonitor OutPutInfo ***-------------");
    PERFORMANCELOG("* ThreadPool Config WorkThread Priority: %d \n", m_pcManager->GetConfig().Priority);
    PERFORMANCELOG("* ThreadPool Config WorkThread MinCnt: %d \n", m_pcManager->GetConfig().MinCnt);
    PERFORMANCELOG("* ThreadPool Config WorkThread MaxCnt: %d \n", m_pcManager->GetConfig().MaxCnt);
    PERFORMANCELOG("* ThreadPool Config WorkThread MaxCnt: %d \n", m_pcManager->GetConfig().InitCnt);
    PERFORMANCELOG("* ThreadPool Config WorkThread WaitListMax: %d \n", m_pcManager->GetConfig().WaitListMax);
    PERFORMANCELOG("* ThreadPool Config WorkThread TaskSchedulePolicy: %d \n", m_pcManager->GetConfig().TaskSchedule);
    PERFORMANCELOG("* CurrentWorkThreadCount: %d \n", m_pcManager->GetCurrentWorkThreadCount());
    PERFORMANCELOG("* TotalMissedTaskCount: %d \n", TotalMissedTaskCount);
    PERFORMANCELOG("* IgnoreRawInfoCount: %d \n", m_IgnoreRawInfoCount);
    TaskInfoMap::iterator it;
    for (it = m_cTaskInfo.begin(); it != m_cTaskInfo.end(); ++it)
    {
        BL_TaskInfo info = it->second;
        PERFORMANCELOG("TaskType[%d] TaskClassName[%s]\n", it->first, info.TaskClassName);
        PERFORMANCELOG("    | RunCount : %lu", info.RunCount);
        DWORD MissCount = 0;
        BaseMap<INT, DWORD>::iterator itcheck = MissedMap.find(it->first);
        if (itcheck != MissedMap.end())
        {
            MissCount = MissedMap[it->first];
        }
        PERFORMANCELOG("    | MissCount : %lu", MissCount);
        PERFORMANCELOG("    | TickCountMin : %lu", info.TickCountMin);
        PERFORMANCELOG("    | TickCountMax : %lu", info.TickCountMax);
        PERFORMANCELOG("    | TickCountAverage : %f", info.TickCountAverage);
        PERFORMANCELOG("    | ThreadTimeMin : %lu", info.ThreadTimeMin);
        PERFORMANCELOG("    | ThreadTimeMax : %lu", info.ThreadTimeMax);
        PERFORMANCELOG("    | ThreadTimeAverage : %f", info.ThreadTimeAverage);
    }
    PERFORMANCELOG(" ");
    PERFORMANCELOG(" ");

    s_lastTickCount = uiNowTickCount;
}

CL_BOOL BL_ThreadPoolMonitor::GetTaskRawInfo(BL_TaskRawInfo & cInfo)
{
    BL_AutoSync sync(m_cSyncObj);
    if (m_cTaskRawInfo.size()>0)
    {
        cInfo = m_cTaskRawInfo.front();
        m_cTaskRawInfo.pop_front();
        return CL_TRUE;
    }
    else
    {
        return CL_FALSE;
    }
}
CL_BOOL BL_ThreadPoolMonitor::AddTaskRawInfo(BL_TaskRawInfo & cInfo)
{
    BL_AutoSync sync(m_cSyncObj);
    m_cTaskRawInfo.push_back(cInfo);
    return CL_TRUE;
}
ULONG BL_ThreadPoolMonitor::GetCountOfTaskRawInfo()
{
    BL_AutoSync sync(m_cSyncObj);
    return m_cTaskRawInfo.size();
}

/* EOF */