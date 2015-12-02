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

#ifndef CXX_CL_USECOUNTER_H
#   include "CL_UseCounter.h"


CL_UseCounter::CL_UseCounter()
{
    use_count = 0;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
}

CL_UseCounter::~CL_UseCounter()
{
    pthread_mutexattr_destroy(&attr);
    pthread_mutex_destroy(&mutex);
}

CL_BOOL
CL_UseCounter::Lock(LONG msec)
{
    if (msec != static_cast<LONG>(INFINITE)) {
        INT wait_count = msec/ 50;
        for (INT i = 0; i < wait_count; i++) {

            if (0 == pthread_mutex_trylock(&mutex))
            {
                if (use_count <= 0) {
                    use_count = 0;
                    return CL_TRUE;
                }

                pthread_mutex_unlock(&mutex);

            }

            Sleep(50);

        }
    } else {
        while (1) {
            if (0 == pthread_mutex_trylock(&mutex))

            {
                if (use_count <= 0) {
                    use_count = 0;
                    return CL_TRUE;
                }

                pthread_mutex_unlock(&mutex);

            }

            Sleep(50);

        }
    }
    return CL_FALSE;
}

VOID
CL_UseCounter::Unlock()
{
    pthread_mutex_unlock(&mutex);
}

VOID
CL_UseCounter::Use()
{
    pthread_mutex_lock(&mutex);
    use_count++;
    pthread_mutex_unlock(&mutex);
    return;
}

VOID
CL_UseCounter::Unuse()
{

    pthread_mutex_lock(&mutex);
    use_count--;
    pthread_mutex_unlock(&mutex);

}

/* EOF */
