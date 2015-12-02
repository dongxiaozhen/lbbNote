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

#ifndef CXX_CL_USECOUNTER_H
#define CXX_CL_USECOUNTER_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#   include <pthread.h>

/**
 *  class CL_UseCounter
 *
 */
class COMMONLIB_API CL_UseCounter
{
protected:
    LONG use_count;

    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;

private:
    CL_UseCounter(const CL_UseCounter& src) {}

    CL_UseCounter& operator = (const CL_UseCounter& src) { return *this; }

public:
    /**
    * Construction.
    */
    CL_UseCounter();

    /**
    * Destruction.
    */
    virtual ~CL_UseCounter();

    /**
    * Lock
    *
    * @param msec : The time of locking
    *
    * @return CL_BOOL : CL_TRUE means lock succeed, and CL_FALSE failed.
    */
    CL_BOOL Lock(LONG msec = INFINITE);

    /**
    * Unlock.
    */
    VOID Unlock();

    /**
    * Reference count increase.
    */
    VOID Use();

    /**
    * Reference count decrease.
    */
    VOID Unuse();

    /**
    * Get the use state.
    *
    * @return CL_BOOL
    * - CL_TRUE : Using
    * - CL_FALSE : Unused
    */
    inline CL_BOOL IsUsed() const { return (use_count != 0) ? CL_TRUE : CL_FALSE; }
};


#endif // CXX_CL_USECOUNTER_H
/* EOF */
