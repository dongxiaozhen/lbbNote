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

#ifndef CXX_CL_THREADBASE_H
#define CXX_CL_THREADBASE_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef CXX_CL_NEWTYPESDEFINE_H
#   include "CL_NewTypesDefine.h"
#endif
#ifndef CXX_CL_THREADSYSTEM_H
#   include "CL_ThreadSystem.h"
#endif

// Class declaration
class CL_ThreadBase;
class CL_Thread;

#define CL_THREAD_NAME_LEN    64

class CL_ThreadBase
{
public:
    CL_ThreadBase(CL_Thread *);

    virtual ~CL_ThreadBase();

    virtual VOID ExtendStackSize();

    virtual VOID StartThread(const XCHAR *name = XTEXT("Unknown Thread"));

    virtual CL_BOOL StopThread(DWORD msec = INFINITE);

    virtual CL_BOOL Terminate();

    virtual CL_BOOL Join(DWORD msec = INFINITE);

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

protected:
    static VOID* ThreadProc(VOID* p);

    virtual VOID ExecRun();

    CL_BOOL    m_bQuitFlg;
    XCHAR      m_szThreadName[CL_THREAD_INFO_NAME_LEN];
    DWORD      m_dwThreadID;
    INT        m_nPrioIdx;
    DWORD      m_aPrioTable[3];
    DWORD      m_dwSanityInterval;
    CL_Thread * m_pThreadIF;

private:
    CL_ThreadBase(const CL_ThreadBase&);
    CL_ThreadBase& operator= (const CL_ThreadBase&);
};

#endif // CXX_CL_THREAD_H
