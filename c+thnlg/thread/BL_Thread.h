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

#ifndef CXX_BL_THREAD_H
#define CXX_BL_THREAD_H

#ifndef CXX_APLCOMMONLIBAPI_H
#   include "AplCommonLibAPI.h"
#endif
#ifndef CXX_CL_THREAD_H
#   include "CL_Thread.h"
#endif
#ifndef CXX_CL_SYNCOBJ_H
#include "CL_SyncObj.h"
#endif

#define NTHREAD_INFO_NAME_LEN (64)

/**
 * @code
 * //declare the user thread class
 * class XXX_Thread : public BL_Thread
 * {
 *  public:
 *     XXX_Thread(){}
 *     virtual ~XXX_Thread(){}
 *
 *     virtual const TCHAR* GetThreadName();
 *
 *     virtual void Run();
 *
 *     //Start the Thread
 *     VOID XXX_Start()
 *     {
 *          BL_Thread::StartRegistThread();
 *     }
 *
 *     //Stop the Thread
 *     VOID XXX_Stop()
 *     {
 *          BL_Thread::StopThread();
 *     }
 *
 * };
 *
 * //implement the user thread class
 * const TCHAR*
 * XXX_Thread::GetThreadName()
 * {
 *     //return the registered Thread Name, please see AplThreadName.h
 *     return XXX_THREAD;
 * }
 *
 * void
 * XXX_Thread::Run()
 * {
 *    while(1){
 *        //check quit flag
 *        if(CheckQuit()) break;
 *
 *      //TODO: add user codes here!
 *      //call break; to quit thread
 *      //call Wait(msec); to sleep a while
 *      //call Wait(INFINITE); to wait for Notify()
 *  }
 *
 * }
 *
 * @endcode
 *
 */

class APLCOMMONLIB_API BL_Thread : public CL_Thread
{
public:
     BL_Thread()
     {
        memset(m_chThreadName, 0, sizeof(XCHAR)*NTHREAD_INFO_NAME_LEN);
     };

    virtual ~BL_Thread() {};

    virtual VOID RegisterName(LPCTSTR chThreadName);

    virtual VOID StartRegistThread(VOID);

    virtual const XCHAR* GetThreadName(VOID);

    virtual VOID SynchronizeStart() { m_cSyncObj.SyncStart(); }

    virtual BOOL SynchronizeStart(BOOL blTrySync)
    {
        if (blTrySync)
            return m_cSyncObj.TrySyncStart();
        else
            m_cSyncObj.SyncStart();
        return TRUE;
    }

    virtual VOID SynchronizeEnd() { m_cSyncObj.SyncEnd(); }

    CL_SyncObj& GetSyncObj() { return m_cSyncObj; }

protected:
    XCHAR    m_chThreadName[NTHREAD_INFO_NAME_LEN];
    CL_SyncObj m_cSyncObj;
};

#endif /* >>CXX_BL_THREAD_H<< */
/* EOF */







