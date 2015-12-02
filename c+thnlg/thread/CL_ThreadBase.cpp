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

#ifndef CXX_CL_THREADBASE_H
#   include "CL_ThreadBase.h"
#endif
#ifndef CXX_CL_THREAD_H
#   include "CL_Thread.h"
#endif

#if defined(_WIN32) || defined(_WIN32_WCE)

#else
#include "CL_ThreadKey.h"
#endif

VOID* CL_ThreadBase::ThreadProc(VOID* p)
{
    if (p == NULL) return NULL;

#if defined(_WIN32) || defined(_WIN32_WCE)

#else
    CL_ThreadKey::Instance()->SetThread(p);
#ifdef _FOR_ANDROID_
    CL_ThreadKey::Instance()->AttachThread();
#endif
#endif

    (static_cast<CL_ThreadBase*>(p))->ExecRun();

#if defined(_WIN32) || defined(_WIN32_WCE)

#else
    CL_ThreadKey::Instance()->SetThread(NULL);
#ifdef _FOR_ANDROID_
    CL_ThreadKey::Instance()->DetachThread();
#endif
#endif

    return NULL;
}

CL_ThreadBase::CL_ThreadBase(CL_Thread *pThread)
: m_bQuitFlg(CL_FALSE)
, m_dwThreadID(-1)
, m_nPrioIdx(0)
, m_dwSanityInterval(0)
, m_pThreadIF(pThread)
{
    memset(m_szThreadName, 0, sizeof(m_szThreadName));
    memset(m_aPrioTable, 0, sizeof(m_aPrioTable));
}

CL_ThreadBase::~CL_ThreadBase()
{

}

VOID
CL_ThreadBase::ExtendStackSize()
{

}

VOID CL_ThreadBase::StartThread(const XCHAR* p)
{

}

CL_BOOL
CL_ThreadBase::StopThread(DWORD msec)
{
    return CL_FALSE;
}

CL_BOOL
CL_ThreadBase::Terminate()
{
    return CL_FALSE;
}

CL_BOOL
CL_ThreadBase::Join(DWORD msec)
{
    return CL_FALSE;
}

VOID CL_ThreadBase::ExecRun()
{

}

CL_BOOL
CL_ThreadBase::Wait(DWORD msec)
{
    return CL_FALSE;
}

VOID CL_ThreadBase::Notify()
{

}

CL_BOOL CL_ThreadBase::IsAlive()
{
    return CL_FALSE;
}

CL_BOOL CL_ThreadBase::IsWait()
{
    return CL_FALSE;
}

CL_BOOL CL_ThreadBase::CheckQuit()
{
    return m_bQuitFlg;
}

XCHAR* CL_ThreadBase::GetName()
{
    return m_szThreadName;
}

VOID CL_ThreadBase::ResetName(const XCHAR* name)
{

}

DWORD CL_ThreadBase::GetThreadID()
{
    return m_dwThreadID;
}

INT CL_ThreadBase::GetPriorityExt()
{
    return 0;
}

CL_BOOL CL_ThreadBase::SetPriorityExt(INT pri)
{
    return CL_FALSE;
}

CL_BOOL CL_ThreadBase::SetPriorityNormal()
{
    return CL_FALSE;
}

CL_BOOL CL_ThreadBase::SetPriorityLow()
{
    return CL_FALSE;
}

CL_BOOL CL_ThreadBase::SetPriorityHigh()
{
    return CL_FALSE;
}

CL_BOOL CL_ThreadBase::NotifyMsg(LPVOID pAddr)
{
    return CL_FALSE;
}

/* EOF */
