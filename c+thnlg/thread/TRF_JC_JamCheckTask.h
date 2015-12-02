/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */
/**
 * @file TRF_JC_JamCheckTask.h
 *
 * @brief Declaration file of class TRF_JC_JamCheckTask.
 *
 * This file includes the declaration of class TRF_JC_JamCheckTask, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#ifndef TRF_JC_JAMCHECKTASK_H
#define TRF_JC_JAMCHECKTASK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "BL_ThreadPoolTask.h"
#include "TRF_COMM_TrafficLog.h"

class TRF_JC_JamCheckProcess;
class TRF_TrafficReq;



/** Statistics Average Speed */
 /**
 * Summary description
 *
 * Details description of the class
 * (Temporarily fix for cpplint check. Add specific description later.)
 */
class TRF_JC_JamCheckTask: public BL_ThreadPoolTask
{
public:
    /** constructor */
    TRF_JC_JamCheckTask(TRF_JC_JamCheckProcess* pJameCheckProcess, TRF_TrafficReq* req, CL_BOOL bUnique = FALSE, INT nGroupId = kTRF_TrafficModuleId_JamCheck + BL_TASKGROUPID_TRAFFIC, INT nPrority = 0);

    /** destructor */
    virtual ~TRF_JC_JamCheckTask();

    virtual INT Run();
    // virtual VOID Return(const INT nErrorCode);
    // virtual VOID OnAbandon(const INT nReason);

    BL_THREADPOLLTASK_DEF(TRF_JC_JamCheckTask)
protected:

private:
    TRF_JC_JamCheckTask(const TRF_JC_JamCheckTask&);
    TRF_JC_JamCheckTask& operator = (const TRF_JC_JamCheckTask&);
    TRF_JC_JamCheckProcess*   m_pJamCheckProcess;
    TRF_TrafficReq*             m_pTrafficReq;
};



#endif
/* EOF */

