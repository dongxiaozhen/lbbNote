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

#include "stdafx.h"

#ifndef CXX_TRF_DS_TMCINFOMEMORY_H
#   include "TRF_DS_TMCInfoMemory.h"
#endif

#ifndef CXX_TRF_SUB_LOG_H
#   include "TRF_COMM_Log.h"
#endif

#ifndef CXX_TRF_SUB_MEMORY_H
#   include "TRF_COMM_Memory.h"
#endif

#ifndef CXX_BL_AUTOSYNC_H
#   include "BL_AutoSync.h"
#endif

#ifndef CXX_TRF_DS_TMCEVENTHASHTABLE_H
#   include "TRF_DS_TMCEventHashTable.h"
#endif

#ifndef CXX_TRF_DS_TMCLOCCACHE_H
#   include "TRF_DS_TMCLocCache.h"
#endif

#ifndef CXX_TRF_TRFTMCDECODESERVICE_H
#   include "TRF_DS_TrfTmcDecodeService.h"
#endif

#ifndef CXX_TRF_DM_TMCADAPTER_H
#   include "TRF_DM_TMCAdapter.h"
#endif

#ifndef CXX_TRF_DS_TMCLOCITEMCACHE_H
#   include "TRF_DS_TMCLocItemCache.h"
#endif

const size_t TRF_SUB_UTIL_ITEM_OBJSIZE[TRF_SUB_UTIL_TYPE_MAX] = {

    // TRAFFIC
    sizeof(TRF_DS_TMCEventHashTable::HashBucket),        // TRF_SUB_UTIL_TYPE_TRAFFIC_EVENT_ITEM
    sizeof(TRF_DS_TMCLocCache::HashBucket),     // TRF_SUB_UTIL_TYPE_TRAFFIC_LOC_ITEM
    sizeof(TRF_DS_TMCLocItemCache::HashBucket),       // TRF_SUB_UTIL_TYPE_TRAFFIC_LOC_REC_ITEM
};

TRF_DS_TMCInfoMemory::TRF_DS_TMCInfoMemory()
{
    INT iTypeIndex = 0;
    for (iTypeIndex = 0; iTypeIndex < TRF_SUB_UTIL_TYPE_MAX; ++iTypeIndex) {
        m_wUsedHighestIndex[iTypeIndex] = 0;
        m_pbyItemBuffer[iTypeIndex] = NULL;
        m_pwIndexBuffer[iTypeIndex] = NULL;
        m_wFirstUnusedIndex[iTypeIndex] = TRF_SUB_INVALID_VALUE_WORD;
        m_wOldestItemIndex[iTypeIndex] = 0;
    }

    Intialize();
}

TRF_DS_TMCInfoMemory::~TRF_DS_TMCInfoMemory()
{
}

VOID
TRF_DS_TMCInfoMemory::Intialize()
{
    WORD wItemType = 0;
    for (wItemType = 0; wItemType < TRF_SUB_UTIL_TYPE_MAX; ++wItemType) {
        Intialize(static_cast<TRF_SUB_UTIL_TYPE>(wItemType));
    }
}

VOID
TRF_DS_TMCInfoMemory::Intialize(TRF_SUB_UTIL_TYPE eItemType)
{
    TRF_DM_TMCAdapter * pcDataManager = TRF_DS_TrfTmcDecodeService::Instance()->GetDataManager();
    if (NULL == pcDataManager) {
        TRF_SUB_MEMORY_CHECK(static_cast<TRF_SUB_LogType>(TRF_SUB_LOGTYPE_TRAFFIC|TRF_SUB_LOGTYPE_ERROR),
                                XTEXT("TRF_DM_TMCAdapter inaccessible!"));
        return;
    }

    // Get Memory
    TRF_SUB_Memory* pcMemory = NULL;
    switch (eItemType) {
    case TRF_SUB_UTIL_TYPE_TRAFFIC_EVENT_ITEM:
        pcMemory = pcDataManager->GetTrafficMemoryInfo(TRF_SUB_EVENT_BUCKET_MEMSIZE);
        break;
    case TRF_SUB_UTIL_TYPE_TRAFFIC_LOC_ITEM:
        pcMemory = pcDataManager->GetTrafficMemoryInfo(TRF_SUB_LOC_BUCKET_MEMSIZE);
        break;
    case TRF_SUB_UTIL_TYPE_TRAFFIC_LOC_REC_ITEM:
        pcMemory = pcDataManager->GetTrafficMemoryInfo(TRF_SUB_LOC_REC_BUCKET_MEMSIZE);
        break;
    default:
        return;
    }

    if (NULL == pcMemory) {
        TRF_SUB_MEMORY_CHECK(static_cast<TRF_SUB_LogType>(TRF_SUB_LOGTYPE_TRAFFIC|TRF_SUB_LOGTYPE_ERROR),
            XTEXT("TRF_DS_TMCInfoMemory get traffic memory information failed!"));
        return;
    }

    BYTE* pbyBuffer = pcMemory->GetAddress();
    DWORD dwSize = pcMemory->GetSize();
    if ((NULL == pbyBuffer) || (0 == dwSize)) {
#ifdef TRF_SUB_MEMORY_CHECK_DEBUG
        XCHAR cMsg[MAX_PATH];
        xsprintf(cMsg, XTEXT("TRF_DS_TMCInfoMemory get memory failed! TRF_SUB_UTIL_TYPE:%d"), eItemType);
        TRF_SUB_MEMORY_CHECK(static_cast<TRF_SUB_LogType>(TRF_SUB_LOGTYPE_DEFAULT | TRF_SUB_LOGTYPE_ERROR), cMsg);
#endif // TRF_SUB_MEMORY_CHECK_DEBUG
        return;
    }

    DWORD dwIndexBufferSize = sizeof(WORD) * TRF_SUB_UTIL_ITEM_OBJNUM[eItemType];
    DWORD dwItemBufferSize = TRF_SUB_UTIL_ITEM_OBJSIZE[eItemType] * TRF_SUB_UTIL_ITEM_OBJNUM[eItemType];
    if (dwSize < (dwIndexBufferSize + dwItemBufferSize)) {
#ifdef TRF_SUB_MEMORY_CHECK_DEBUG
        XCHAR cMsg[MAX_PATH];
        xsprintf(cMsg, XTEXT("TRF_DS_TMCInfoMemory get memory failed! TRF_SUB_UTIL_TYPE:%d"), eItemType);
        TRF_SUB_MEMORY_CHECK(static_cast<TRF_SUB_LogType>(TRF_SUB_LOGTYPE_DEFAULT | TRF_SUB_LOGTYPE_ERROR), cMsg);
#endif // TRF_SUB_MEMORY_CHECK_DEBUG

       TRF_TMC_LOG_E("MEM INIT FAILED [Alloc:%d Needed:%d]", dwSize, dwIndexBufferSize + dwItemBufferSize);

        return;
    }

    m_pwIndexBuffer[eItemType] = reinterpret_cast<WORD*>(pbyBuffer);
    m_pbyItemBuffer[eItemType]  = pbyBuffer + dwIndexBufferSize;
    // initialize index buffer
    WORD wIndex = 0;
    for (wIndex = 0; wIndex < TRF_SUB_UTIL_ITEM_OBJNUM[eItemType]; ++wIndex) {
        m_pwIndexBuffer[eItemType][wIndex] = TRF_SUB_INVALID_VALUE_WORD;
    }
}

WORD
TRF_DS_TMCInfoMemory::GetEmptyItemIndex(TRF_SUB_UTIL_TYPE eItemType)
{
    if (eItemType >= TRF_SUB_UTIL_TYPE_MAX) {
        return TRF_SUB_INVALID_VALUE_WORD;
    }

    if (TRF_SUB_INVALID_VALUE_WORD != m_wFirstUnusedIndex[eItemType]) {
        return m_wFirstUnusedIndex[eItemType];
    }

    if (m_wUsedHighestIndex[eItemType] >= TRF_SUB_UTIL_ITEM_OBJNUM[eItemType]) {
#ifdef TRF_SUB_MEMORY_CHECK_DEBUG
        XCHAR cMsg[MAX_PATH];
        xsprintf(cMsg, XTEXT("TRF_DS_TMCInfoMemory full! TRF_SUB_UTIL_TYPE:%d"), eItemType);
        TRF_SUB_MEMORY_CHECK(static_cast<TRF_SUB_LogType>(TRF_SUB_LOGTYPE_DEFAULT | TRF_SUB_LOGTYPE_ERROR), cMsg);
#endif // TRF_SUB__MEMORY_CHECK_DEBUG
        return TRF_SUB_INVALID_VALUE_WORD;
    }

    return m_wUsedHighestIndex[eItemType];
}

TRF_SUB_UtilItem*
TRF_DS_TMCInfoMemory::Alloc(TRF_SUB_UTIL_TYPE eItemType)
{
    WORD wIndex = GetEmptyItemIndex(eItemType);

    if (TRF_SUB_INVALID_VALUE_WORD == wIndex) {
        // FULL
#ifdef TRF_SUB_MEMORY_CHECK_DEBUG
        XCHAR cMsg[MAX_PATH];
        xsprintf(cMsg, XTEXT("TRF_DS_TMCInfoMemory full! TRF_SUB_UTIL_TYPE:%d"), eItemType);
        TRF_SUB_MEMORY_CHECK(static_cast<TRF_SUB_LogType>(TRF_SUB_LOGTYPE_DEFAULT | TRF_SUB_LOGTYPE_ERROR), cMsg);
#endif // TRF_SUB_MEMORY_CHECK_DEBUG
        RETURN_NULL;
    }

    if (NULL == m_pwIndexBuffer[eItemType] || NULL == m_pbyItemBuffer[eItemType]) {
        RETURN_NULL;
    }

    TRF_SUB_UtilItem* pcItem = reinterpret_cast<TRF_SUB_UtilItem*>(
        m_pbyItemBuffer[eItemType] + wIndex * TRF_SUB_UTIL_ITEM_OBJSIZE[eItemType]);

    if (wIndex == m_wFirstUnusedIndex[eItemType]) {
        m_wFirstUnusedIndex[eItemType] = m_pwIndexBuffer[eItemType][wIndex];
    }
    else if (wIndex == m_wUsedHighestIndex[eItemType]) {
        (m_wUsedHighestIndex[eItemType])++;
    }
    else {
#ifdef TRF_SUB_MEMORY_CHECK_DEBUG
        XCHAR cMsg[MAX_PATH];
        xsprintf(cMsg, XTEXT("TRF_DS_TMCInfoMemory abnormal status! TRF_SUB_UTIL_TYPE:%d"), eItemType);
        TRF_SUB_MEMORY_CHECK(static_cast<TRF_SUB_LogType>(TRF_SUB_LOGTYPE_DEFAULT | TRF_SUB_LOGTYPE_ERROR), cMsg);
#endif // TRF_SUB_MEMORY_CHECK_DEBUG
        RETURN_NULL;
    }

    m_pwIndexBuffer[eItemType][wIndex] = wIndex;

    return pcItem;
}

VOID
TRF_DS_TMCInfoMemory::Free(VOID* pcItem, TRF_SUB_UTIL_TYPE eItemType)
{
    if (NULL == pcItem
        ||  NULL == m_pwIndexBuffer[eItemType]
        ||  NULL == m_pbyItemBuffer[eItemType]) {
        return;
    }

    LONG nPointOffset = reinterpret_cast<BYTE*>(pcItem) - m_pbyItemBuffer[eItemType];
    if (0 != (nPointOffset % TRF_SUB_UTIL_ITEM_OBJSIZE[eItemType])) {
        // abnormal pointer!
#ifdef TRF_SUB_MEMORY_CHECK_DEBUG
        XCHAR cMsg[MAX_PATH];
        xsprintf(cMsg, XTEXT("TRF_DS_TMCInfoMemory free item pointer abnormal! TRF_SUB_UTIL_TYPE:%d pointer:0x%08x"),
            eItemType,
            pcItem);
        TRF_SUB_MEMORY_CHECK(static_cast<TRF_SUB_LogType>(TRF_SUB_LOGTYPE_DEFAULT | TRF_SUB_LOGTYPE_ERROR), cMsg);
#endif // TRF_SUB_MEMORY_CHECK_DEBUG
        return;
    }

    LONG lIndex = nPointOffset / TRF_SUB_UTIL_ITEM_OBJSIZE[eItemType];
    if (lIndex < 0 || lIndex >= static_cast<SHORT>(TRF_SUB_UTIL_ITEM_OBJNUM[eItemType])) {
        // abnormal pointer!
#ifdef TRF_SUB_MEMORY_CHECK_DEBUG
        XCHAR cMsg[MAX_PATH];
        xsprintf(cMsg, XTEXT("TRF_DS_TMCInfoMemory free item pointer abnormal! TRF_SUB_UTIL_TYPE:%d pointer:0x%08x"),
            eItemType,
            pcItem);
        TRF_SUB_MEMORY_CHECK(static_cast<TRF_SUB_LogType>(TRF_SUB_LOGTYPE_DEFAULT | TRF_SUB_LOGTYPE_ERROR), cMsg);
#endif // TRF_SUB_MEMORY_CHECK_DEBUG
        return;
    }

    // lIndex is valid value, cast to WORD type
    WORD wIndex = static_cast<WORD>(lIndex);

    WORD wIndexTemp = m_pwIndexBuffer[eItemType][wIndex];
    m_pwIndexBuffer[eItemType][wIndex] = m_wFirstUnusedIndex[eItemType];
    m_wFirstUnusedIndex[eItemType] = wIndexTemp;
}

// ignore by cppcheck.no used source.
#if 0

// This function should only be called when alloc fails, which means the buffer is full
TRF_SUB_UtilItem*
TRF_DS_TMCInfoMemory::FindOldestItem(TRF_SUB_UTIL_TYPE eItemType)
{
    if (NULL == m_pbyItemBuffer[eItemType]) {
        RETURN_NULL;
    }

    // points to buffer occupied by the oldest item
    TRF_SUB_UtilItem *ppBuffer = reinterpret_cast<TRF_SUB_UtilItem*>(
        m_pbyItemBuffer[eItemType] + m_wOldestItemIndex[eItemType] * TRF_SUB_UTIL_ITEM_OBJSIZE[eItemType]);

    // move the cycled index by one, the maximum can not exceed UsedHighestIndex,
    // the value of which should be the same as the maximum number of items allowed in the normal case
    m_wOldestItemIndex[eItemType] = (m_wOldestItemIndex[eItemType] + 1) % TRF_SUB_UTIL_ITEM_OBJNUM[eItemType];

    return ppBuffer;
}

#endif

VOID
TRF_DS_TMCInfoMemory::ResetState(TRF_SUB_UTIL_TYPE eItemType)
{
    m_wOldestItemIndex[eItemType] = 0;
    m_wUsedHighestIndex[eItemType] = 0;
    m_wFirstUnusedIndex[eItemType] = TRF_SUB_INVALID_VALUE_WORD;
}
/* EOF */
