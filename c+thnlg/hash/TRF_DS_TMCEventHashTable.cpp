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
 * @file TRF_DS_TMCEventHashTable.cpp.h
 *
 * @brief Declaration file of class TRF_DS_TMCEventHashTable.cpp.
 *
 * This file includes the declaration of class TRF_DS_TMCEventHashTable.cpp, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#include "stdafx.h"

#ifndef CXX_TRF_DS_TMCEVENTHASHTABLE_H
#   include "TRF_DS_TMCEventHashTable.h"
#endif

#ifndef CXX_TRF_SUB_MEMORY_H
#   include "TRF_COMM_Memory.h"
#endif

#ifndef CXX_TRF_SUB_LOG_H
#   include "TRF_COMM_Log.h"
#endif

#ifndef CXX_TRF_DS_TMCINFOMEMORY_H
#   include "TRF_DS_TMCInfoMemory.h"
#endif

#ifndef CXX_TRF_TRFTMCDECODESERVICE_H
#   include "TRF_DS_TrfTmcDecodeService.h"
#endif

#ifndef CXX_TRF_DM_TMCADAPTER_H
#   include "TRF_DM_TMCAdapter.h"
#endif


TRF_DS_TMCEventHashTable::HashBucket::HashBucket()
    : m_dwId(0)
    , m_pcNext(NULL)
{
}

TRF_DS_TMCEventHashTable::HashBucket::~HashBucket()
{
}

void*
TRF_DS_TMCEventHashTable::HashBucket::AllocBuffer()
{
    TRF_DS_TMCInfoMemory *pUtilContainer = TRF_DS_TrfTmcDecodeService::Instance()->GetUtilContainer();
    if (NULL == pUtilContainer) {
        RETURN_NULL;
    }

    return pUtilContainer->Alloc(TRF_SUB_UTIL_TYPE_TRAFFIC_EVENT_ITEM);
}

void
TRF_DS_TMCEventHashTable::HashBucket::FreeBuffer(void* m)
{
    TRF_DS_TMCInfoMemory *pUtilContainer = TRF_DS_TrfTmcDecodeService::Instance()->GetUtilContainer();
    if (NULL != pUtilContainer) {
        pUtilContainer->Free(m, TRF_SUB_UTIL_TYPE_TRAFFIC_EVENT_ITEM);
    }
}

TRF_DS_TMCEventHashTable::Iterator::Iterator(const Iterator & rcIterator)
{
    m_pcHashtable = rcIterator.m_pcHashtable;
    m_pcCurrent = rcIterator.m_pcCurrent;
    m_wCurrentHashSlot = rcIterator.m_wCurrentHashSlot;
}

TRF_DS_TMCEventHashTable::Iterator::Iterator(TRF_DS_TMCEventHashTable* pcHashtable)
    : m_wCurrentHashSlot(0)
    , m_pcHashtable(pcHashtable)
    , m_pcCurrent(NULL)
{
    if (NULL != m_pcHashtable) {
        while (m_wCurrentHashSlot < m_pcHashtable->m_wSize) {
            m_pcCurrent = m_pcHashtable->m_ppcArray[m_wCurrentHashSlot];
            if (NULL != m_pcCurrent) {
                break;
            }
            m_wCurrentHashSlot++;
        }
    }
}

TRF_DS_TMCEventHashTable::Iterator::~Iterator(VOID)
{
}

TRF_DS_TMCEventHashTable::Iterator &
TRF_DS_TMCEventHashTable::Iterator::operator=(const Iterator & rcIterator)
{
    if (this != &rcIterator) {
        m_pcHashtable = rcIterator.m_pcHashtable;
        m_pcCurrent = rcIterator.m_pcCurrent;
        m_wCurrentHashSlot = rcIterator.m_wCurrentHashSlot;
    }

    return *this;
}

TRF_SUB_Event *
TRF_DS_TMCEventHashTable::Iterator::CurrentItem(VOID) const
{
    if (CL_TRUE == IsDone()) {
        RETURN_NULL;
    }

    return &(m_pcCurrent->m_cItem);
}

VOID
TRF_DS_TMCEventHashTable::Iterator::First(VOID)
{
    if (NULL != m_pcHashtable) {
        m_pcCurrent = (m_pcHashtable->m_ppcArray)[0];
    }
    else {
        // should never reach here
        TRF_TMC_LOG_E("LMI::  Get First fail by NULL == m_pcHashtable");
    }
}

VOID
TRF_DS_TMCEventHashTable::Iterator::Next(VOID)
{
    if (NULL == m_pcCurrent) {
        return;
    }

    if (NULL != m_pcCurrent->m_pcNext) {
        m_pcCurrent = m_pcCurrent->m_pcNext;
    }
    else {
        if (NULL == m_pcHashtable) {
            TRF_TMC_LOG_E("LMI::  Get Nest fail by NULL == m_pcHashtable");
            return;
        }

        while (m_wCurrentHashSlot < m_pcHashtable->m_wSize) {
            m_wCurrentHashSlot++;
            if (NULL != m_pcHashtable->m_ppcArray[m_wCurrentHashSlot]) {
                m_pcCurrent =  m_pcHashtable->m_ppcArray[m_wCurrentHashSlot];
                break;
            }
        }

        if (m_wCurrentHashSlot == m_pcHashtable->m_wSize) {
            m_pcCurrent = NULL;
        }
    }
}

CL_BOOL
TRF_DS_TMCEventHashTable::Iterator::IsDone(VOID) const
{
    if ((NULL == m_pcHashtable) || (NULL == m_pcCurrent)) {
        return CL_TRUE;
    }

    return CL_FALSE;
}

CL_BOOL
TRF_DS_TMCEventHashTable::Iterator::operator==(const Iterator& rcIterator) const
{
    if ((m_pcHashtable == rcIterator.m_pcHashtable)
        && (m_pcCurrent == rcIterator.m_pcCurrent)
        && (m_wCurrentHashSlot == rcIterator.m_wCurrentHashSlot)) {
        return CL_TRUE;
    }

    return CL_FALSE;
}

CL_BOOL
TRF_DS_TMCEventHashTable::Iterator::operator!=(const Iterator& rcIterator) const
{
    if (*this == rcIterator) {
        return CL_FALSE;
    }

    return CL_TRUE;
}

TRF_DS_TMCEventHashTable::TRF_DS_TMCEventHashTable(WORD wSz, WORD wMaxNrOfItems)
    : m_wSize(wSz)
    , m_wMaxNrOfItems(wMaxNrOfItems)
    , m_wCount(0)
    , m_ppcArray(NULL)
    , m_pcEmptyList(NULL)
{
    m_ppcArray = NULL;
    WORD wCnt = 0;

    if (wSz != 0) {
        TRF_DM_TMCAdapter * pcDataManager = TRF_DS_TrfTmcDecodeService::Instance()->GetDataManager();
        if (NULL == pcDataManager) {
            TRF_SUB_MEMORY_CHECK(static_cast<TRF_SUB_LogType>(TRF_SUB_LOGTYPE_TRAFFIC|TRF_SUB_LOGTYPE_ERROR),
                                    XTEXT("TRF_DM_TMCAdapter inaccessible!"));
            return;
        }

        TRF_SUB_Memory *pcMemory = pcDataManager->GetTrafficMemoryInfo(TRF_SUB_EVENT_TABLE_MEMSIZE);

        if (NULL == pcMemory) {
            TRF_SUB_MEMORY_CHECK(static_cast<TRF_SUB_LogType>(TRF_SUB_LOGTYPE_TRAFFIC|TRF_SUB_LOGTYPE_ERROR),
                XTEXT("TRF_DS_TMCEventHashTable get traffic memory information failed!"));
            return;
        }

        m_ppcArray = reinterpret_cast<HashBucket **>(pcMemory->GetAddress());

        if (pcMemory->GetSize() < sizeof(HashBucket*) * wSz) {
            TRF_TMC_LOG_E("MEM INIT FAILED [Alloc:%d Needed:%d]", pcMemory->GetSize(), sizeof(HashBucket*) * wSz);
        }

        if (NULL != m_ppcArray) {
            for (wCnt = 0; wCnt < m_wSize; wCnt++) {
                m_ppcArray[wCnt] = NULL;
            }
        }
        else {
            TRF_SUB_MEMORY_CHECK(static_cast<TRF_SUB_LogType>(TRF_SUB_LOGTYPE_TRAFFIC|TRF_SUB_LOGTYPE_ERROR),
                                    XTEXT("TRF_DS_TMCEventHashTable memory initialization failed!"));
        }
    }
}

TRF_DS_TMCEventHashTable::~TRF_DS_TMCEventHashTable()
{
    WORD wCnt = 0;
    HashBucket * pcFreeMe = NULL;

    for (wCnt = 0; wCnt < m_wSize; wCnt++) {
        HashBucket *pcBucket = m_ppcArray[wCnt];

        while (NULL != pcBucket) {
            pcFreeMe = pcBucket;
            pcBucket = pcBucket->m_pcNext;
            HashBucket::FreeBuffer((void*)pcFreeMe);
            pcFreeMe = NULL;
        }
    }

    while (NULL != m_pcEmptyList) {
        pcFreeMe = m_pcEmptyList;
        m_pcEmptyList = m_pcEmptyList->m_pcNext;
        delete pcFreeMe;
        pcFreeMe = NULL;
    }
}

// 插入到最前面
CL_BOOL
TRF_DS_TMCEventHashTable::Insert(DWORD dwId, const TRF_SUB_Event & rcItem)
{
    if ((m_wMaxNrOfItems > 0) && (m_wCount == m_wMaxNrOfItems)) {
        return CL_FALSE;
    }

    DWORD dwChain = Hash(dwId);

    HashBucket *pcBucket = LookupBucket(dwId);

    if (NULL != pcBucket) {
        pcBucket->m_cItem = rcItem;
    }
    else {
        HashBucket *pcNewBucket = NULL;

        if (NULL != m_pcEmptyList) {
            pcNewBucket = m_pcEmptyList;
            m_pcEmptyList = m_pcEmptyList->m_pcNext;
        }
        else {
            pcNewBucket = new(static_cast<TRF_DS_TMCEventHashTable::HashBucket*>(TRF_DS_TMCEventHashTable::HashBucket::AllocBuffer())) HashBucket();
            if (NULL == pcNewBucket) {
                return CL_FALSE;
            }
        }
        pcNewBucket->m_dwId = dwId;
        pcNewBucket->m_cItem = rcItem;
        pcNewBucket->m_pcNext = m_ppcArray[dwChain];
        m_ppcArray[dwChain] = pcNewBucket;
        m_wCount++;
    }

    return CL_TRUE;
}

CL_BOOL
TRF_DS_TMCEventHashTable::Lookup(DWORD dwId) const
{
    if (NULL != LookupBucket(dwId)) {
        return CL_TRUE;
    }

    return CL_FALSE;
}

TRF_SUB_Event *
TRF_DS_TMCEventHashTable::Value(DWORD dwId) const
{
    HashBucket *pcBucket = LookupBucket(dwId);

    if (NULL != pcBucket) {
        return &(pcBucket->m_cItem);
    }

    RETURN_NULL;
}

VOID
TRF_DS_TMCEventHashTable::Remove(DWORD dwId)
{
    DWORD dwChain = Hash(dwId);
    HashBucket *pcBucket = LookupBucket(dwId);

    if (NULL == pcBucket) {
        return;
    }

    if (m_ppcArray[dwChain] == pcBucket) {
        m_ppcArray[dwChain] = pcBucket->m_pcNext;
        pcBucket->m_pcNext = m_pcEmptyList;
        m_pcEmptyList = pcBucket;
    }
    else {
        /* or elsewhere...
         * At this point, back pointers would really be useful
         * in removing an item.  ASSUMPTION:  remove will be
         * an infrequent operation, so the space/time tradeoff
         * of scanning the whole chain -vs- another pointer field
         * falls to the side of more time and less space.
         *
         * We know that the bucket MUST be on this chain.
         * because Hash(symbol) pointed here  AND
         *        LookupBucket(symbol) suceeded
         */
        HashBucket *pcTmp = m_ppcArray[dwChain];

        /*   +-----+   +------+   +------+   +------+
         *   |     |-->|      |-->|      |-->|      |-->@
         *   +-----+   +------+   +------+   +------+
         *     tmp                  bucket
         */
        while ((NULL != pcTmp->m_pcNext) && (pcTmp->m_pcNext != pcBucket)) {
            pcTmp = pcTmp->m_pcNext;  // step thru till found
        }

        if (NULL == pcTmp->m_pcNext) {
            return;
        }

        /*   +-----+   +------+   +------+   +------+
         *   |     |-->|      |-->|      |-->|      |-->@
         *   +-----+   +------+   +------+   +------+
         *               tmp        bucket
         */
        pcTmp->m_pcNext = pcTmp->m_pcNext->m_pcNext;

        /*                      /----------\
         *   +-----+   +------+ | +------+ | +------+
         *   |     |-->|      |-/ |      | \>|      |-->@
         *   +-----+   +------+   +------+   +------+
         *               tmp        bucket
         */
        pcBucket->m_pcNext = m_pcEmptyList;
        m_pcEmptyList = pcBucket;
    }

    m_wCount--;
}

WORD
TRF_DS_TMCEventHashTable::GetCount(VOID) const
{
    return m_wCount;
}

TRF_DS_TMCEventHashTable::Iterator
TRF_DS_TMCEventHashTable::Begin(VOID)
{
    return Iterator(this);
}

TRF_DS_TMCEventHashTable::HashBucket *
TRF_DS_TMCEventHashTable::LookupBucket(DWORD dwId) const
{
    HashBucket *pcBp = NULL;

    for (pcBp = m_ppcArray[Hash(dwId)]; NULL != pcBp; pcBp = pcBp->m_pcNext) {
        if (NULL != pcBp) {
            if (pcBp->m_dwId == dwId) {
                return pcBp;      // Found a match!
            }
        }
    }

    RETURN_NULL;
}

DWORD
TRF_DS_TMCEventHashTable::Hash(DWORD dwId) const
{
    BYTE    byByte = 0;
    DWORD   dwHval = 0;  // hash value
    DWORD   dwG = 0;     // intermediate hash value

    INT iCnt = 3;

    for (iCnt = 3; iCnt >= 0; iCnt--) {
        byByte = static_cast<BYTE>((dwId >> 8*iCnt) & 0x000000FF);
        dwHval = (dwHval << 4) + byByte;

        dwG = dwHval & 0xF0000000;
        if (0 != dwG) {
            dwHval = dwHval ^ (dwG >> 24);
            dwHval = dwHval ^ dwG;
        }
    }

    return (m_wSize == 0)? 0:(dwHval % m_wSize);
}


/* EOF */
