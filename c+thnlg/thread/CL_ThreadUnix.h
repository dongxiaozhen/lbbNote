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

#ifndef CXX_CL_THREADUNIX_H
#define CXX_CL_THREADUNIX_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef CXX_CL_THREADBASE_H
#   include "CL_ThreadBase.h"
#endif
#ifndef CXX_CL_LIST_H
#   include "CL_List.h"
#endif
#ifndef CXX_CL_SYNCOBJ_H
#   include "CL_SyncObj.h"
#endif

#include <pthread.h>

// Class declaration
class CL_ThreadUnix;
class CL_Thread;
class CL_SyncObj;

class CL_ThreadUnix : public CL_ThreadBase
{
public:
    CL_ThreadUnix(CL_Thread*);

    virtual ~CL_ThreadUnix();

    virtual VOID ExtendStackSize();

    VOID StartThread(const XCHAR *name = XTEXT("Unknown Thread"));

    virtual CL_BOOL StopThread(DWORD msec = INFINITE);

    virtual CL_BOOL Terminate();

    virtual CL_BOOL Join(DWORD msec = INFINITE);

    virtual CL_BOOL Wait(DWORD msec = INFINITE);

    virtual VOID Notify();

    virtual VOID ResetName(const XCHAR* name);

    virtual INT GetPriorityExt();

    virtual CL_BOOL SetPriorityExt(INT pri);

    virtual CL_BOOL SetPriorityNormal();

    virtual CL_BOOL SetPriorityLow();

    virtual CL_BOOL SetPriorityHigh();

    virtual CL_BOOL IsAlive();

    virtual CL_BOOL IsWait();

    virtual CL_BOOL NotifyMsg(LPVOID pAddr);

    static uint64_t GetCurrentThreadId();

protected:
    virtual VOID ExecRun();

    VOID SetThreadName();

private:
    CL_SyncObj         m_cSyncObj;
    CL_BOOL            m_bSignalFlg;
    CL_BOOL            m_isAlive;
    pthread_t          m_tid;
    pthread_attr_t     m_attr;
    pthread_mutex_t    m_mutex;
    pthread_cond_t     m_cond;
#if defined(_LINUX)
    pthread_condattr_t m_condattr;
#endif
#ifdef _FOR_APPLE_
    VOID               *m_pPool;
#endif
    CL_List <VOID>    m_cMsgQue;
    CL_SyncObj        m_cSyncMSg;
};

#endif // end of CXX_CL_THREAD_H
