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
 * @file TRF_DS_TMCEventHashTable.h
 *
 * @brief Declaration file of class TRF_DS_TMCEventHashTable.
 *
 * This file includes the declaration of class TRF_DS_TMCEventHashTable, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

// 用链表解决冲突

#ifndef CXX_TRF_DS_TMCEVENTHASHTABLE_H
#define CXX_TRF_DS_TMCEVENTHASHTABLE_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef CXX_TRF_SUB_COMMON_H
#   include "TRF_DS_DiffCommon.h"
#endif

#ifndef CXX_TRF_SUB_EVENT_H
#   include "TRF_DS_DiffEvent.h"
#endif

/**
 * Summary description
 *
 * Details description of the class
 * (Temporarily fix for cpplint check. Add specific description later.)
 */
class TRF_DS_TMCEventHashTable
{
public:
    enum
    {
        TRF_SUB_HT_DEFAULT_SIZE = 253
    };

public:
    class Iterator;

public:
    /**
    * Summary description
    *
    * Details description of the class
    * (Temporarily fix for cpplint check. Add specific description later.)
    */
    class HashBucket
    {
    private:

        HashBucket();

        ~HashBucket();

        static void* AllocBuffer();

        static void  FreeBuffer(void* m);

    private:
        HashBucket(const HashBucket&);

        HashBucket& operator = (const HashBucket&);

    private:

        void* operator new[](size_t sz) throw();    ///< forbidden

        void operator delete[](void* m);    ///< forbidden

        DWORD               m_dwId;         ///< bucket id
        TRF_SUB_Event  m_cItem;        ///< associated event
        HashBucket          *m_pcNext;      ///< pointer to the next bucket

        friend class TRF_DS_TMCEventHashTable;
        friend class Iterator;
    };

public:
    /**
    * Summary description
    *
    * Details description of the class
    * (Temporarily fix for cpplint check. Add specific description later.)
    */
    class Iterator
    {
    public:

        friend class TRF_DS_TMCEventHashTable;

        Iterator(const Iterator & rcIterator);

        Iterator(TRF_DS_TMCEventHashTable* pcHashtable);

        virtual ~Iterator(VOID);

        Iterator & operator=(const Iterator & rcIterator);

        TRF_SUB_Event * CurrentItem(VOID) const;

        VOID First(VOID);

        VOID Next(VOID);

        CL_BOOL IsDone(VOID) const;

        CL_BOOL operator==(const Iterator& rcIterator) const;

        CL_BOOL operator!=(const Iterator& rcIterator) const;

    private:

        WORD                        m_wCurrentHashSlot; ///< The current hash slot the iterator is iterating through
        TRF_DS_TMCEventHashTable *m_pcHashtable;     ///< Pointer to the hash table the iterator is iterating through
        HashBucket                  *m_pcCurrent;       ///< The current element the iterator is pointing at
    };


public:

    /*-----------------------------------------------------------------------*/
    /**
    * TRF_DS_TMCEventHashTable
    *
    * Constructor
    *
    * @param WORD  wSz = TRF_DS_TMCEventHashTable::TRF_SUB_HT_DEFAULT_SIZE
    * @param WORD wMaxNrOfItems = 0
    * @return NONE
    */
    /*-----------------------------------------------------------------------*/
    TRF_DS_TMCEventHashTable(WORD  wSz = TRF_DS_TMCEventHashTable::TRF_SUB_HT_DEFAULT_SIZE, WORD wMaxNrOfItems = 0);

    virtual ~TRF_DS_TMCEventHashTable();


    /*-----------------------------------------------------------------------*/
    /**
    * Insert
    *
    * Insert
    *
    * @param DWORD dwId
    * @param const TRF_SUB_Event & rcItem
    * @return CL_BOOL
    */
    /*-----------------------------------------------------------------------*/
    CL_BOOL Insert(DWORD dwId, const TRF_SUB_Event & rcItem);


    /*-----------------------------------------------------------------------*/
    /**
    * Remove
    *
    * Remove
    *
    * @param DWORD dwId
    * @return VOID
    */
    /*-----------------------------------------------------------------------*/
    VOID Remove(DWORD dwId);


    /*-----------------------------------------------------------------------*/
    /**
    * Lookup
    *
    * Lookup
    *
    * @param DWORD dwId
    * @return CL_BOOL
    */
    /*-----------------------------------------------------------------------*/
    CL_BOOL Lookup(DWORD dwId) const;


    /*-----------------------------------------------------------------------*/
    /**
    * Value
    *
    * Value
    *
    * @param DWORD dwId
    * @return TRF_SUB_Event*
    */
    /*-----------------------------------------------------------------------*/
    TRF_SUB_Event* Value(DWORD dwId) const;


    /*-----------------------------------------------------------------------*/
    /**
    * GetCount
    *
    * Get Count
    *
    * @param VOID
    * @return WORD
    */
    /*-----------------------------------------------------------------------*/
    WORD GetCount(VOID) const;


    /*-----------------------------------------------------------------------*/
    /**
    * Begin
    *
    * Begin
    *
    * @param VOID
    * @return Iterator
    */
    /*-----------------------------------------------------------------------*/
    Iterator Begin(VOID);

    friend class Iterator;

private:

    /*-----------------------------------------------------------------------*/
    /**
    * LookupBucket
    *
    * Look up Bucket
    *
    * @param DWORD dwId
    * @return HashBucket*
    */
    /*-----------------------------------------------------------------------*/
    HashBucket* LookupBucket(DWORD dwId) const;


    /*-----------------------------------------------------------------------*/
    /**
    * Hash
    *
    * Hash
    *
    * @param DWORD dwId
    * @return DWORD
    */
    /*-----------------------------------------------------------------------*/
    DWORD Hash(DWORD dwId) const;

private:
    TRF_DS_TMCEventHashTable(const TRF_DS_TMCEventHashTable&);
    TRF_DS_TMCEventHashTable& operator = (const TRF_DS_TMCEventHashTable&);

private:
    WORD        m_wSize;            ///< The number of hash slots
    WORD        m_wMaxNrOfItems;    ///< The maximum number of items in the hash table
    WORD        m_wCount;           ///< The current number of items
    HashBucket  **m_ppcArray;       ///< The hash table
    HashBucket  *m_pcEmptyList;     ///< The list of empty hash buckets
};

#endif // CXX_TRF_DS_TMCEVENTHASHTABLE_H
/* EOF */
