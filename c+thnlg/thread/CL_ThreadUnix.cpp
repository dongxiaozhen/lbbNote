/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#include "stdafx.h"

#ifndef CXX_CL_THREADUNIX_H
#   include "CL_ThreadUnix.h"
#endif
#ifndef CXX_CL_APLERROR_H
#   include "CL_AplError.h"
#endif
#ifndef CXX_CL_THREAD_H
#   include "CL_Thread.h"
#endif
#ifndef CXX_CL_TIMER_H
#   include "CL_Timer.h"
#endif
#ifndef CXX_CL_TIME_H
#   include "CL_Time.h"
#endif
#ifndef CXX_CL_DEBUG_H
#   include "CL_Debug.h"
#endif
#ifndef CXX_CL_THREADKEY_H
#   include "CL_ThreadKey.h"
#endif
#if defined(_LINUX)
#   define _GUN_SOURCE // open GUN features
#   define TASKNAME_COMLEN 16 // thread name could be only 16 byte long
#   include <sys/resource.h>
#   include <sys/types.h>
#elif defined(_FOR_ANDROID_)
#   include <sys/resource.h>
#   include <sys/prctl.h>
#elif defined(_FOR_APPLE_)
#   include "CL_ThreadMac.h"
#endif
#   include <string.h>
#include "CL_AutoSync.h"

CL_ThreadUnix::CL_ThreadUnix(CL_Thread *pThread)
: CL_ThreadBase(pThread)
, m_bSignalFlg(CL_FALSE)
, m_isAlive(CL_FALSE)
, m_tid(0)
{
    m_dwThreadID = 0;

#if defined(_FOR_APPLE_)
    m_pPool = NULL;
#endif
    INT ret = pthread_attr_init(&m_attr);
    if (0 != ret)
    {
        CL_PERROR("Thread m_attr init failed %d [%s].", ret, strerror(ret));
    }
    /*
    ret = pthread_attr_setscope(&m_attr, PTHREAD_SCOPE_SYSTEM);
    if (0 != ret)
    {
        CL_PERROR("thread m_attr scope init failed %d [%s].", ret, strerror(ret));
    }

    ret = pthread_attr_setschedpolicy(&m_attr, SCHED_RR);
    if (0 != ret)
    {
        CL_PERROR("thread m_attr policy init failed %d [%s].", ret, strerror(ret));
    }
    */

    ret = pthread_attr_setstacksize(&m_attr, 64*1024);
    if (0 != ret)
    {
        CL_PERROR("Thread m_attr stacksize init failed %d [%s].", ret, strerror(ret));
    }

    ret = pthread_mutex_init(&m_mutex, NULL);
    if (0 != ret)
    {
        CL_PERROR("Thread m_mutex init failed %d [%s].", ret, strerror(ret));
    }

#if defined(_LINUX)
    pthread_condattr_init(&m_condattr);
    int err = pthread_condattr_setclock(&m_condattr, CLOCK_MONOTONIC);
    if (err)
    {
        CL_PERROR("Thread m_condattr setting clock: %d [%s]\n", err, strerror(err));
    }
    ret = pthread_cond_init(&m_cond, &m_condattr);
#else
    ret = pthread_cond_init(&m_cond, NULL);
#endif
    if (0 != ret)
    {
        CL_PERROR("Thread m_cond init failed %d [%s].", ret, strerror(ret));
    }

    m_nPrioIdx = CL_THREAD_PRIORITY_IDX_NORMAL;
    m_aPrioTable[CL_THREAD_PRIORITY_IDX_NORMAL] = 0; // 251;
    m_aPrioTable[CL_THREAD_PRIORITY_IDX_LOW]    = 0; // 251;
    m_aPrioTable[CL_THREAD_PRIORITY_IDX_HIGH]   = 0; // 251;
}

CL_ThreadUnix::~CL_ThreadUnix()
{
    StopThread();
    pthread_attr_destroy(&m_attr);
    pthread_cond_destroy(&m_cond);
#if defined(_LINUX)
    pthread_condattr_destroy(&m_condattr);
#endif
}

VOID
CL_ThreadUnix::ExtendStackSize()
{
    pthread_attr_setstacksize(&m_attr, 128*1024);
}

VOID CL_ThreadUnix::StartThread(const XCHAR* name)
{
    if (m_isAlive) // make sure do not chock in the CL_AutoSync while called by mutithread.
    {
        return;
    }

    CL_AutoSync cSync(m_cSyncObj);

    if (m_tid) return;

    // thread already started, meanwhile not ended, do not start again
    // thread already ran, but quit, then join the thread to release the resource
    if (m_isAlive)
    {
        return;
    }
    else if (m_tid)
    {
        Join();
    }

    if (name == NULL)
    {
        name = XTEXT("Unknown Thread");
        CL_PERROR("<<<< [%s] thread cannot be started.", name);
        return;
    }

    xcsncpy(m_szThreadName, name, CL_THREAD_NAME_LEN);
    m_szThreadName[CL_THREAD_NAME_LEN - 1] = TEXT('\0');

    m_bQuitFlg = FALSE;
    m_bSignalFlg = CL_FALSE;

    CL_ThreadSystemIF* pThreadSystem = NULL;
    if ((m_pThreadIF != NULL) && ((pThreadSystem = m_pThreadIF->GetThreadSystem()) != NULL))
    {
        BOOL ret = pThreadSystem->GetThreadTableInfo(m_szThreadName, m_aPrioTable, m_dwSanityInterval);
        if (!ret) {
            CL_PERROR("<<<< [%s] Get thread table info failed.", name);
            return;
        }
    }

    INT ret = pthread_create(&m_tid, &m_attr, ThreadProc,  this);

    if (0 == ret)
    {
        m_isAlive = CL_TRUE;
        CL_PRINTF(("<<<< Start thread [%s] TC: %lu.",  name, ::GetTickCount()));
    }
}

CL_BOOL
CL_ThreadUnix::StopThread(DWORD msec)
{
    if (!m_tid)
    {
        return CL_TRUE;
    }

    m_bQuitFlg = CL_TRUE;

    Notify();

    return Join();
}

CL_BOOL
CL_ThreadUnix::Terminate()
{
#if defined(_LINUX)
    if (!m_tid)
    {
        return CL_TRUE;
    }

    m_bQuitFlg = CL_TRUE;

    Notify();

    if (!m_tid)
    {
        return CL_TRUE;
    }

    INT ret = pthread_cancel(m_tid);

    if (0 == ret)
    {
        return CL_TRUE;
    }
    else
    {
        CL_PERROR("<<<< Stop thread [%s] (%lu, 0x%lx) join failed %d [%s].", m_szThreadName, m_dwThreadID, m_dwThreadID, ret, strerror(ret));
        return CL_FALSE;
    }
    return CL_TRUE;
#else
    return CL_FALSE;
#endif
}

CL_BOOL
CL_ThreadUnix::Join(DWORD msec)
{
    if (!m_tid)
    {
        return CL_TRUE;
    }

    INT ret = pthread_join(m_tid, NULL);

    if (0 == ret)
    {
        // for thread ended ;set thread_t threadid to initliaze stats
        m_tid = 0;
        return CL_TRUE;
    }
    else
    {
        CL_PERROR("<<<< Stop thread [%s] (%lu, 0x%lx) join failed %d [%s].", m_szThreadName, m_dwThreadID, m_dwThreadID, ret, strerror(ret));
        return CL_FALSE;
    }
}

VOID CL_ThreadUnix::ExecRun()
{
#if defined (_FOR_ANDROID_) || defined (_FOR_APPLE_) || defined (_LINUX)
    m_dwThreadID = GetCurrentThreadId();
#else
    m_dwThreadID = (DWORD)m_tid;
#endif

#if defined(_LINUX)
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);            // 允许退出线程
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,   NULL);     // 设置立即取消
#endif

    SetPriorityNormal();
    SetThreadName();

    CL_Printf("<<<< Thread [%s] run (%lu, 0x%x) prio: %d, TC: %lu.", m_szThreadName, m_dwThreadID, m_dwThreadID, GetPriorityExt(), ::GetTickCount());

#ifdef _FOR_APPLE_
    m_pPool = NULL;
    pool_alloc(&m_pPool);
#endif

    m_pThreadIF->Run();

#ifdef _FOR_APPLE_
    pool_release(&m_pPool);
#endif

    CL_Printf("<<<< Thread [%s] quit (%lu, 0x%x) TC: %lu.", m_szThreadName, m_dwThreadID, m_dwThreadID, ::GetTickCount());

    m_isAlive = CL_FALSE;
    m_dwThreadID = 0;
}

CL_BOOL
CL_ThreadUnix::Wait(DWORD msec)
{
    INT ret = 0;

    pthread_mutex_lock(&m_mutex);

    if (m_bSignalFlg == CL_FALSE)
    {
        if (INFINITE != msec)
        {
#ifdef _FOR_APPLE_
            struct timespec nptime;
            nptime.tv_sec = msec/ 1000;
            nptime.tv_nsec = msec% 1000*1000000;

            ret = pthread_cond_timedwait_relative_np(&m_cond, &m_mutex, &nptime);

#elif defined(_FOR_ANDROID_)
            struct timespec nptime;
            gettimespec(&nptime, msec);

            ret = pthread_cond_timedwait_monotonic_np(&m_cond, &m_mutex, &nptime);

#else // _LINUX
            struct timespec nptime;
            gettimespec(&nptime, msec);

            ret = pthread_cond_timedwait(&m_cond, &m_mutex, &nptime);
#endif
        }
        else
        {
            ret = pthread_cond_wait(&m_cond, &m_mutex);
        }
    }

    m_bSignalFlg = CL_FALSE;

    pthread_mutex_unlock(&m_mutex);

    // DWORD *pAddr = NULL; // [0] timer id; [1] timer pointer.
    while (1)
    {
        m_cSyncMSg.SyncStart();
        intptr_t* pAddr = (intptr_t*)m_cMsgQue.Pop();
        m_cSyncMSg.SyncEnd();
        if (pAddr != NULL)
        {
            if (CL_Timer::IsValid(*pAddr) == TRUE)
            {
                reinterpret_cast<CL_Timer*>(*(pAddr+1))->DoAction();
            }
            delete[] pAddr;
        }
        else
        {
            break;
        }
    }

    // ETIMEDOUT
    if (0 == ret)
    {
        return CL_TRUE;
    }
    else
    {
        return CL_FALSE;
    }
}

VOID CL_ThreadUnix::Notify()
{
    pthread_mutex_lock(&m_mutex);
    m_bSignalFlg = TRUE;
    INT ret = pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_mutex);
}

VOID CL_ThreadUnix::ResetName(const XCHAR* name)
{
    if (!name) return;

    xcsncpy(m_szThreadName, name, CL_THREAD_NAME_LEN);
    m_szThreadName[CL_THREAD_NAME_LEN - 1] = TEXT('\0');
    SetThreadName();
}

VOID CL_ThreadUnix::SetThreadName()
{
#if defined (_LINUX)
    // if thread is running  do reset the running threadname
    if (!m_tid) return;
    // for task name length can be only 16 byte
    char threadname[TASKNAME_COMLEN];
    strncpy(threadname, m_szThreadName, TASKNAME_COMLEN);
    threadname[TASKNAME_COMLEN - 1] = '\0';
    if (pthread_setname_np(m_tid, threadname))
    {
        CL_PERROR("<<<< Set thread [%lu] [%s] name to [%s] failed : %s.", m_dwThreadID, m_szThreadName, threadname, strerror(errno));
    }
    else
    {
        CL_Printf("<<<< Set thread [%lu] [%s] name to [%s] TC : %lu.", m_dwThreadID, m_szThreadName, threadname, ::GetTickCount());
    }
#elif defined(_FOR_ANDROID_)
    if (m_dwThreadID != (DWORD)gettid())
    {
        CL_PERROR("<<<< Set thread [%s][%lu, 0x%x] name failed, NOT in current Thread.", m_szThreadName, m_dwThreadID, m_dwThreadID);
        return;
    }
    prctl(PR_SET_NAME, (unsigned long) m_szThreadName, 0, 0, 0);
    CL_Printf("<<<< Set thread [%s] (%lu, 0x%x) name TC: %lu.", m_szThreadName, m_dwThreadID, m_dwThreadID, ::GetTickCount());
#elif defined(_FOR_APPLE_)
    if (m_dwThreadID != (DWORD)gettid())
    {
        CL_PERROR("<<<< Set thread [%s][%lu, 0x%x] name failed, NOT in current Thread.", m_szThreadName, m_dwThreadID, m_dwThreadID);
        return;
    }
    setCurrentThreadName(m_szThreadName);
#endif
}

INT CL_ThreadUnix::GetPriorityExt()
{
    int Priority = 0;
#if defined(_FOR_ANDROID_) || defined (_LINUX)
    Priority = getpriority(PRIO_PROCESS, m_dwThreadID);

#elif defined(_FOR_APPLE_)
    if (m_dwThreadID != (DWORD)gettid()) return -1;
    double macproi = getCurrentThreadPriority();
    Priority =  255 - macproi * (255 - 249);
#endif

    return Priority;
}

CL_BOOL CL_ThreadUnix::SetPriorityExt(INT pri)
{
    if (!m_tid)
    {
        return CL_FALSE;
    }
#if defined(_FOR_ANDROID_) || defined (_LINUX)
    int ret = setpriority(PRIO_PROCESS, m_dwThreadID, pri);
    if (ret < 0)
    {
        CL_PERROR("<<<< Set thread [%s][%lu, 0x%x] priority : %d failed, ret = %d [%s].", m_szThreadName, m_dwThreadID, static_cast<UINT>(m_dwThreadID), pri, ret, strerror(ret));
    }

#elif defined _FOR_APPLE_
    if (m_dwThreadID != (DWORD)gettid())
    {
        return CL_FALSE;
    }
    double macpri = (double)pri;
    setCurrentThreadPriority(macpri);

#endif
    return CL_TRUE;
}

CL_BOOL CL_ThreadUnix::SetPriorityNormal()
{
    m_nPrioIdx = CL_THREAD_PRIORITY_IDX_NORMAL;
    return SetPriorityExt(m_aPrioTable[m_nPrioIdx]);
}

CL_BOOL CL_ThreadUnix::SetPriorityLow()
{
    m_nPrioIdx = CL_THREAD_PRIORITY_IDX_LOW;
    return SetPriorityExt(m_aPrioTable[m_nPrioIdx]);
}

CL_BOOL CL_ThreadUnix::SetPriorityHigh()
{
    m_nPrioIdx = CL_THREAD_PRIORITY_IDX_HIGH;
    return SetPriorityExt(m_aPrioTable[m_nPrioIdx]);
}

CL_BOOL CL_ThreadUnix::IsAlive()
{
    return m_isAlive;
}

CL_BOOL CL_ThreadUnix::IsWait()
{
    // pthread_mutex_lock(&m_mutex);
    CL_BOOL ret = m_bSignalFlg;
    // pthread_mutex_unlock(&m_mutex);
    return (CL_FALSE == ret);
}

CL_BOOL CL_ThreadUnix::NotifyMsg(LPVOID pAddr)
{
    m_cSyncMSg.SyncStart();
    m_cMsgQue.Add(pAddr);
    m_cSyncMSg.SyncEnd();

    pthread_mutex_lock(&m_mutex);
    m_bSignalFlg = CL_TRUE;
    INT ret = pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_mutex);

    return TRUE;
}

uint64_t CL_ThreadUnix::GetCurrentThreadId()
{
    return gettid();
}

/* EOF */
