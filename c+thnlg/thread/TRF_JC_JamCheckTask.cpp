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

#ifndef TRF_TRAFFICLOG_H
#   include "TRF_COMM_TrafficLog.h"
#endif
#include "TRF_JC_JamCheckTask.h"
#include "TRF_JC_JamCheckProcess.h"
#include "TRF_COMM_TrafficReq.h"

TRF_JC_JamCheckTask::TRF_JC_JamCheckTask(TRF_JC_JamCheckProcess* pJamCheckProcess,
    TRF_TrafficReq* req, CL_BOOL bUnique, INT nGroupId, INT nPrority)
    : BL_ThreadPoolTask(bUnique, nGroupId, nPrority)
    , m_pJamCheckProcess(pJamCheckProcess)
    , m_pTrafficReq(req)
{

}

TRF_JC_JamCheckTask::~TRF_JC_JamCheckTask()
{
    if (m_pTrafficReq) {
        delete m_pTrafficReq;
        m_pTrafficReq = NULL;
    }

}

INT TRF_JC_JamCheckTask::Run()
{
    if (m_pJamCheckProcess) {
        m_pJamCheckProcess->doAction(m_pTrafficReq);
        return 0;
    }
    else {
        TRF_TT_LOG_E("m_pJamCheckProcess is NULL!");
        return -1;
    }
}


/* EOF */


