
#ifndef __ELASTOS_CORE_AUTOLOCK_H__
#define __ELASTOS_CORE_AUTOLOCK_H__

#include <elastos/core/Object.h>

using Elastos::Core::EIID_ISynchronize;

namespace Elastos {
namespace Core {

class Autolock
{
public:
    Autolock(
        /* [in] */ ISynchronize * obj)
        : mLocked(TRUE)
    {
        assert(obj != NULL);
        mSyncObj = obj;
        ASSERT_SUCCEEDED(mSyncObj->Lock());
    }

    Autolock(
        /* [in] */ IInterface * obj)
        : mLocked(TRUE)
    {
        assert(obj != NULL);
        mSyncObj = (ISynchronize *)obj->Probe(EIID_ISynchronize);
        assert(mSyncObj != NULL);
        ASSERT_SUCCEEDED(mSyncObj->Lock());
    }

    Autolock(
        /* [in] */ IObject * obj)
        : mLocked(TRUE)
    {
        assert(obj != NULL);
        mSyncObj = (ISynchronize *)obj->Probe(EIID_ISynchronize);
        assert(mSyncObj != NULL);
        ASSERT_SUCCEEDED(mSyncObj->Lock());
    }

    Autolock(
        /* [in] */ Object * obj)
        : mLocked(TRUE)
    {
        mSyncObj = (ISynchronize *)obj->Probe(EIID_ISynchronize);
        assert(mSyncObj != NULL);
        ASSERT_SUCCEEDED(mSyncObj->Lock());
    }

    Autolock(
        /* [in] */ Object & obj)
        : mLocked(TRUE)
    {
        mSyncObj = (ISynchronize *)obj.Probe(EIID_ISynchronize);
        assert(mSyncObj != NULL);
        ASSERT_SUCCEEDED(mSyncObj->Lock());
    }

    ~Autolock()
    {
        assert(mSyncObj != NULL);
        ASSERT_SUCCEEDED(mSyncObj->Unlock());
    }

    // report the state of locking when used as a boolean
    inline operator Boolean () const
    {
        return mLocked;
    }

    // unlock
    inline void SetUnlock()
    {
        mLocked = FALSE;
    }

private:
    Autolock(const Autolock&);

private:
    AutoPtr<ISynchronize> mSyncObj;
    Boolean mLocked;
};

} // namespace Core
} // namespace Elastos

#ifndef synchronized
#define synchronized(obj)  for(Elastos::Core::Autolock obj##_lock(obj); obj##_lock; obj##_lock.SetUnlock())
#endif

using Elastos::Core::Autolock;

#endif //__ELASTOS_CORE_AUTOLOCK_H__
