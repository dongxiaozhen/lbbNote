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
 * @file TRF_DS_TMCInfoMemory.h
 *
 * @brief Declaration file of class TRF_DS_TMCInfoMemory.
 *
 * This file includes the declaration of class TRF_DS_TMCInfoMemory, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#ifndef CXX_TRF_DS_TMCINFOMEMORY_H
#define CXX_TRF_DS_TMCINFOMEMORY_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef CXX_TRF_SUB_COMMON_H
#   include "TRF_DS_DiffCommon.h"
#endif

// Class define
class TRF_DS_TMCInfoMemory;
class TRF_SUB_UtilItem;
class TRF_SUB_Memory;

enum TRF_SUB_UTIL_TYPE
{

    // TRAFFIC
    TRF_SUB_UTIL_TYPE_TRAFFIC_EVENT_ITEM,              ///< item type enum for traffic event item
    TRF_SUB_UTIL_TYPE_TRAFFIC_LOC_ITEM,                ///< item type enum for traffic locatio item
    TRF_SUB_UTIL_TYPE_TRAFFIC_LOC_REC_ITEM,            ///< item type enum for traffic location record

    TRF_SUB_UTIL_TYPE_MAX
};

// number range : 0-0xFFFE
const WORD TRF_SUB_UTIL_ITEM_OBJNUM[TRF_SUB_UTIL_TYPE_MAX] = {

    300,        // TRF_SUB_UTIL_TYPE_TRAFFIC_EVENT_ITEM
    3000,       // TRF_SUB_UTIL_TYPE_TRAFFIC_LOC_ITEM
    3000,       // TRF_SUB_UTIL_TYPE_TRAFFIC_LOC_REC_ITEM
};

/**
 * Summary description
 *
 * Details description of the class
 * (Temporarily fix for cpplint check. Add specific description later.)
 */
class TRF_DS_TMCInfoMemory
{
public:
    /*-----------------------------------------------------------------------*/
    /**
     * Intialize
     * Item Buffer memory should be set here
     */
    /*-----------------------------------------------------------------------*/
    virtual VOID Intialize();

    /**
    * Add an item
    *
    * @param eItemType [IN] : The item to add
    * @return An error code
    * @retval BL_NOERROR : The incident has been added
    * @retval APL_ERR_TRF_SUB_FULL : The DB is full, the incident can not be added
    * @retval APL_ERR_TRF_SUB_UNEXPECTED : An unexpected error occurred
    */
    TRF_SUB_UtilItem* Alloc(TRF_SUB_UTIL_TYPE eItemType);

    /**
    * Get a free index in the container
    *
    * @param VOID* eItemType
    * @return A free index, 0xFFFF if the DB is full
    */
    VOID Free(VOID* pcItem, TRF_SUB_UTIL_TYPE eItemType);

// ignore by cppcheck.no used source.
#if 0

    /**
    * FindOldestItem
    *
    * Find the oldest item in memory of a particular type, works like FIFO(without much deletion)
    * This function should only be called when alloc fails, which means the buffer is full
    * It finds the item oldest and return pointer to the buffer
    * It just concerns about content in buffer and cycled index
    *
    * @param eItemType [IN] : item type to alloc
    * @return TRF_SUB_UtilItem* : pointer to buffer alloced
    */
    TRF_SUB_UtilItem* FindOldestItem(TRF_SUB_UTIL_TYPE eItemType);
#endif

    /*-----------------------------------------------------------------------*/
    /**
    * ResetState
    *
    * Clear all states of a particular type
    *
    * @param eItemType [in] : item type
    * @return NONE
    */
    /*-----------------------------------------------------------------------*/
    VOID ResetState(TRF_SUB_UTIL_TYPE eItemType);


    /*-----------------------------------------------------------------------*/
    /**
    * Constructor
    *
    */
    /*-----------------------------------------------------------------------*/
    TRF_DS_TMCInfoMemory();

    /*-----------------------------------------------------------------------*/
    /**
    * ~TRF_DS_TMCInfoMemory.h
    *
    * Destructor
    *
    * @return NONE
    */
    /*-----------------------------------------------------------------------*/
    virtual ~TRF_DS_TMCInfoMemory();


private:
    /**
    * Get a empty index in the container
    *
    * @param Nothing
    * @return A free index, 0xFFFF if the container is full
    */
    WORD GetEmptyItemIndex(TRF_SUB_UTIL_TYPE eItemType);

    /*-----------------------------------------------------------------------*/
    /**
    * Intialize special type
    * Item Buffer memory should be set here
    */
    /*-----------------------------------------------------------------------*/
    virtual VOID Intialize(TRF_SUB_UTIL_TYPE eItemType);

private:
    // The index of the oldest item in buffer, since buffer works like a queue(item comes later with higher index,
    // suppose not much deletion), one cycled index can point out the oldest item up to now
    // Watch out: It works wrong on items with frequent deletions
    WORD    m_wOldestItemIndex[TRF_SUB_UTIL_TYPE_MAX];

    WORD    m_wUsedHighestIndex[TRF_SUB_UTIL_TYPE_MAX];    ///< The highest index used up till now
    WORD    m_wFirstUnusedIndex[TRF_SUB_UTIL_TYPE_MAX];    ///< The first index which has been unused
    BYTE*   m_pbyItemBuffer[TRF_SUB_UTIL_TYPE_MAX];        ///< memory used to save all items
    WORD*   m_pwIndexBuffer[TRF_SUB_UTIL_TYPE_MAX];        ///< memory used to save all items' index

private:
    TRF_DS_TMCInfoMemory(const TRF_DS_TMCInfoMemory&);

    TRF_DS_TMCInfoMemory& operator = (const TRF_DS_TMCInfoMemory&);
};

#endif // CXX_TRF_DS_TMCINFOMEMORY_H
/* EOF */
