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

#ifndef CXX_CL_THREAD_H
#   include "CL_Thread.h"
#endif
#if defined(_WIN32) || defined(_WIN32_WCE)
#ifndef CXX_CL_THREADWIN_H
#   include "CL_ThreadWin.h"
#endif
#else
#ifndef CXX_CL_THREADUNIX_H
#   include "CL_ThreadUnix.h"
#endif
#endif
#ifndef CXX_CL_THREADBASE_H
#   include "CL_ThreadBase.h"
#endif

#if defined(_WIN32) || defined(_WIN32_WCE)
#define CL_ThreadImpl CL_ThreadWin
#else
#define CL_ThreadImpl CL_ThreadUnix
#endif

CL_Thread::CL_Thread()
{
    m_pThreadImpl = new(MEM_Base) CL_ThreadImpl(this);
    m_pThreadSystem = CL_GetThreadSystem();
}

CL_Thread::CL_Thread(CL_ThreadSystemIF* sp)
{
    m_pThreadImpl = new(MEM_Base) CL_ThreadImpl(this);
    m_pThreadSystem = sp;
}

CL_Thread::~CL_Thread()
{
    delete m_pThreadImpl;
    m_pThreadImpl = NULL;
}

VOID
CL_Thread::ExtendStackSize()
{
    StopThread();
    m_pThreadImpl->ExtendStackSize();
}

VOID CL_Thread::StartThread(const XCHAR* p)
{
    m_pThreadImpl->StartThread(p);
}

CL_BOOL
CL_Thread::StopThread(DWORD msec)
{
    return m_pThreadImpl->StopThread(msec);
}

CL_BOOL
CL_Thread::Terminate()
{
    return m_pThreadImpl->Terminate();
}

CL_BOOL
CL_Thread::Join(DWORD msec)
{
    return m_pThreadImpl->Join(msec);
}

CL_BOOL
CL_Thread::Wait(DWORD msec)
{
    return m_pThreadImpl->Wait(msec);
}

VOID CL_Thread::Notify()
{
    m_pThreadImpl->Notify();
}

CL_BOOL CL_Thread::IsAlive()
{
    return m_pThreadImpl->IsAlive();
}

CL_BOOL CL_Thread::IsWait()
{
    return m_pThreadImpl->IsWait();
}

CL_BOOL CL_Thread::CheckQuit()
{
    return m_pThreadImpl->CheckQuit();
}

XCHAR* CL_Thread::GetName()
{
    return m_pThreadImpl->GetName();
}

VOID CL_Thread::ResetName(const XCHAR* name)
{
    m_pThreadImpl->ResetName(name);
}

DWORD CL_Thread::GetThreadID()
{
    return m_pThreadImpl->GetThreadID();
}

INT CL_Thread::GetPriorityExt()
{
    return m_pThreadImpl->GetPriorityExt();
}

CL_BOOL CL_Thread::SetPriorityExt(INT pri)
{
    return m_pThreadImpl->SetPriorityExt(pri);
}

CL_BOOL CL_Thread::SetPriorityNormal()
{
    return m_pThreadImpl->SetPriorityNormal();
}

CL_BOOL CL_Thread::SetPriorityLow()
{
    return m_pThreadImpl->SetPriorityLow();
}

CL_BOOL CL_Thread::SetPriorityHigh()
{
    return m_pThreadImpl->SetPriorityHigh();
}

CL_BOOL CL_Thread::NotifyMsg(LPVOID pAddr)
{
    return m_pThreadImpl->NotifyMsg(pAddr);
}

VOID CL_Thread::Run()
{

}

CL_ThreadSystemIF* CL_Thread::GetThreadSystem()
{
    if (NULL == m_pThreadSystem) {
        m_pThreadSystem = CL_GetThreadSystem();
    }
    return m_pThreadSystem;
}

uint64_t CL_Thread::GetCurrentThreadId()
{
    return CL_ThreadImpl::GetCurrentThreadId();
}

/* EOF */