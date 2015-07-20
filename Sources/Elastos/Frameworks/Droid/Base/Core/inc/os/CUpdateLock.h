#ifndef __CUPDATELOCK_H__
#define __CUPDATELOCK_H__

#include "_CUpdateLock.h"
#include "ext/frameworkdef.h"

using Elastos::Droid::Os::IIUpdateLock;
namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CUpdateLock)
{
public:
    CUpdateLock();

    ~CUpdateLock();
    /**
     * Construct an UpdateLock instance.
     * @param tag An arbitrary string used to identify this lock instance in dump output.
     */
    CARAPI constructor(
        /* [in] */ const String& tag);

    /**
     * Change the refcount behavior of this update lock.
     */
    CARAPI SetReferenceCounted(
        /* [in] */ Boolean isRefCounted);

    /**
     * Is this lock currently held?
     */
    CARAPI IsHeld(
        /* [out] */ Boolean* result);

    /**
     * Acquire an update lock.
     */
    CARAPI AcquireLock();

    /**
     * Release this update lock.
     */
    CARAPI ReleaseLock();

private:
    static CARAPI_(void) CheckService();

    CARAPI AcquireLocked();

    CARAPI ReleaseLocked();

protected:
    AutoPtr<IBinder> mToken;
    Int32 mCount;
    Boolean mRefCounted;
    Boolean mHeld;
    String mTag;

private:
    static const Boolean DEBUG;
    static const String TAG;
    static AutoPtr<IIUpdateLock> sService;
    Object mLock;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CUPDATELOCK_H__
