#ifndef CXX_BL_THREADPOOLTASK_H
#define CXX_BL_THREADPOOLTASK_H

#ifndef CXX_APLCOMMONLIBAPI_H
#   include "AplCommonLibAPI.h"
#endif
#ifndef CXX_CL_NEWTYPESDEFINE_H
#   include "CL_NewTypesDefine.h"
#endif
#ifndef CXX_BL_AUTOSYNC_H
#include "BL_AutoSync.h"
#endif
#include <boost/shared_ptr.hpp>

typedef DWORD BL_TASKID;
typedef DWORD BL_TASKTYPE;

class BL_ThreadPoolManager;
class BL_ThreadPoolWorkThread;

class APLCOMMONLIB_API BL_ThreadPoolTask
{
public:
    friend class BL_ThreadPoolWorkThread;
    friend class BL_ThreadPoolManager;

    BL_ThreadPoolTask(CL_BOOL bUnique = FALSE, INT nGroupId = -1, INT nPrority = 0);

    virtual ~BL_ThreadPoolTask();

    virtual INT Run();
    virtual VOID Return(const INT nErrorCode);
    virtual VOID OnAbandon(const INT nReason);

    BL_TASKID GetTaskID();
    virtual BL_TASKTYPE GetTaskType() = 0;
    virtual const char* GetTaskClassName() = 0;

    INT GetTaskGroupId();

    INT GetTaskPriority();

    DWORD GetSequenceIndex();

    CL_BOOL IsUnique();

    CL_BOOL IsCanceled();

private:
    BL_TASKID        m_nTaskId;
    INT        m_nGroupId;
    INT        m_nPrority;
    DWORD      m_nSequenceIndex;
    CL_BOOL    m_bUnique;
    CL_BOOL    m_bCanceledFlag;
};

#define BL_THREADPOLLTASK_DEF(classname) \
    virtual BL_TASKTYPE GetTaskType() \
    { \
        static const BL_TASKTYPE m_stpye = 0; \
        /* FIXME: it's used to generate unique value for each class based */ \
        /*        on the assumption that the addresses have common prefix */ \
        return static_cast<BL_TASKTYPE>(reinterpret_cast<intptr_t>(&m_stpye)); \
    } \
    virtual const char* GetTaskClassName() \
    { \
        return #classname; \
    }

#define BL_TASKABANDON_CANCELBYUSER     1
#define BL_TASKABANDON_UNIQUETASK       2

const INT BL_TASKGROUPID_BASE           =   0x00010000;
const INT BL_TASKGROUPID_GUIDE          =   0x00020000;
const INT BL_TASKGROUPID_LOC            =   0x00030000;
const INT BL_TASKGROUPID_MAP            =   0x00040000;
const INT BL_TASKGROUPID_PATH           =   0x00050000;
const INT BL_TASKGROUPID_SEARCH         =   0x00060000;
const INT BL_TASKGROUPID_TRAFFIC        =   0x00070000;
const INT BL_TASKGROUPID_VOICE          =   0x00080000;
const INT BL_TASKGROUPID_COLLECTION     =   0x00090000;
const INT BL_TASKGROUPID_HWY            =   0x000A0000;

#endif /* >>CXX_BL_THREADPOOLTASK_H<< */
/* EOF */