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

#ifndef CXX_CL_THREAD_H
#define CXX_CL_THREAD_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef CXX_CL_NEWTYPESDEFINE_H
#   include "CL_NewTypesDefine.h"
#endif
#ifndef CXX_CL_THREADSYSTEMIF_H
#   include "CL_ThreadSystemIF.h"
#endif

// Class declaration
class CL_ThreadBase;

/**
 * @brief class represents a thread.
 *
 * @detail helper class to create thread and run code in the thread.
 */
class COMMONLIB_API CL_Thread
{
public:
    CL_Thread();

    CL_Thread(CL_ThreadSystemIF*);

    virtual ~CL_Thread();

    virtual VOID ExtendStackSize();

    virtual VOID StartThread(const XCHAR *name = XTEXT("Unknown Thread"));

    virtual CL_BOOL StopThread(DWORD msec = INFINITE);

    virtual CL_BOOL Terminate();

    virtual CL_BOOL Join(DWORD msec);

    virtual CL_BOOL Wait(DWORD msec = INFINITE);

    virtual VOID Notify();

    virtual CL_BOOL IsAlive();

    virtual CL_BOOL IsWait();

    virtual CL_BOOL CheckQuit();

    virtual XCHAR* GetName();

    virtual VOID ResetName(const XCHAR* name);

    virtual DWORD GetThreadID();

    virtual INT GetPriorityExt();

    virtual CL_BOOL SetPriorityExt(INT pri);

    virtual CL_BOOL SetPriorityNormal();

    virtual CL_BOOL SetPriorityLow();

    virtual CL_BOOL SetPriorityHigh();

    virtual CL_BOOL NotifyMsg(LPVOID pAddr);

    virtual VOID Run();

    virtual CL_ThreadSystemIF* GetThreadSystem();

    static uint64_t GetCurrentThreadId();

private:
    CL_ThreadBase* m_pThreadImpl;
    CL_ThreadSystemIF* m_pThreadSystem;

private:
    CL_Thread(const CL_Thread&);
    CL_Thread& operator=(const CL_Thread&);
};

#endif // CXX_CL_THREAD_H
