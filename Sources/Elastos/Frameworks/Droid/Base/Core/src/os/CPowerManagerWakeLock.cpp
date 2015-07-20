
#include "os/CPowerManagerWakeLock.h"
#include "os/CPowerManager.h"
#include "os/CBinder.h"
#include "os/Handler.h"
#include "os/CWorkSource.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Slogger;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Os::CWorkSource;

namespace Elastos {
namespace Droid {
namespace Os {

CPowerManagerWakeLock::ReleaseRunnable::ReleaseRunnable(
    /* [in] */ CPowerManagerWakeLock* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(CPowerManagerWakeLock::ReleaseRunnable, IRunnable);

ECode CPowerManagerWakeLock::ReleaseRunnable::Run()
{
    return mHost->ReleaseLock();
}

CPowerManagerWakeLock::CPowerManagerWakeLock()
    : mFlags(0)
    , mCount(0)
    , mRefCounted(TRUE)
    , mHeld(FALSE)
{
    mReleaser = (IRunnable*)new ReleaseRunnable(this);
}

CPowerManagerWakeLock::~CPowerManagerWakeLock()
{
    AutoLock lock(mTokenLock);
    if (mHeld) {
        // Log.wtf(TAG, "WakeLock finalized while still held: " + mTag);
        // try {
        mHost->mService->ReleaseWakeLock(mToken, 0);
        // } catch (RemoteException e) {
        // }
    }
}

ECode CPowerManagerWakeLock::SetReferenceCounted(
    /* [in] */ Boolean value)
{
    AutoLock lock(mTokenLock);
    mRefCounted = value;
    return NOERROR;
}

ECode CPowerManagerWakeLock::AcquireLock()
{
    AutoLock lock(mTokenLock);
    AcquireLocked();
    return NOERROR;
}

ECode CPowerManagerWakeLock::AcquireLock(
    /* [in] */ Int64 timeout)
{
    AutoLock lock(mTokenLock);
    AcquireLocked();
    Boolean result;
    return mHost->mHandler->PostDelayed(mReleaser, timeout, &result);
}

void CPowerManagerWakeLock::AcquireLocked()
{
    if (!mRefCounted || mCount++ == 0) {
        // Do this even if the wake lock is already thought to be held (mHeld == true)
        // because non-reference counted wake locks are not always properly released.
        // For example, the keyguard's wake lock might be forcibly released by the
        // power manager without the keyguard knowing.  A subsequent call to acquire
        // should immediately acquire the wake lock once again despite never having
        // been explicitly released by the keyguard.
        mHost->mHandler->RemoveCallbacks(mReleaser);
        // try {
        mHost->mService->AcquireWakeLock(mToken, mFlags, mTag, mWorkSource);
        // } catch (RemoteException e) {
        // }
        mHeld = TRUE;
    }
}

ECode CPowerManagerWakeLock::ReleaseLock()
{
    return ReleaseLock(0);
}

ECode CPowerManagerWakeLock::ReleaseLock(
    /* [in] */ Int32 flags)
{
    AutoLock lock(mTokenLock);
    if (!mRefCounted || --mCount == 0) {
        mHost->mHandler->RemoveCallbacks(mReleaser);
        if (mHeld) {
            // try {
            mHost->mService->ReleaseWakeLock(mToken, flags);
            // } catch (RemoteException e) {
            // }
            mHeld = FALSE;
        }
    }
    if (mCount < 0) {
        Slogger::E(String("PowerManager"), "WakeLock under-locked %s", (const char*)mTag);
        return E_RUNTIME_EXCEPTION;
        // throw new RuntimeException("WakeLock under-locked " + mTag);
    }
    return NOERROR;
}

ECode CPowerManagerWakeLock::IsHeld(
    /* [out] */ Boolean* isHeld)
{
    VALIDATE_NOT_NULL(isHeld);
    AutoLock lock(mTokenLock);
    *isHeld = mHeld;
    return NOERROR;
}

ECode CPowerManagerWakeLock::SetWorkSource(
    /* [in] */ IWorkSource* ws)
{
    AutoLock lock(mTokenLock);
    Int32 size;
    if (ws != NULL && (ws->GetSize(&size), size == 0)) {
        ws = NULL;
    }

    Boolean changed;
    if (ws == NULL) {
        changed = mWorkSource != NULL;
        mWorkSource = NULL;
    }
    else if (mWorkSource == NULL) {
        changed = TRUE;
        CWorkSource::New(ws, (IWorkSource**)&mWorkSource);
    }
    else {
        mWorkSource->Diff(ws, &changed);
        if (changed) {
            mWorkSource->Set(ws);
        }
    }

    if (changed && mHeld) {
        // try {
        mHost->mService->UpdateWakeLockWorkSource(mToken, mWorkSource);
        // } catch (RemoteException e) {
        // }
    }
    return NOERROR;
}

ECode CPowerManagerWakeLock::ToString(
    /* [out] */ String* s)
{
    VALIDATE_NOT_NULL(s);
    AutoLock lock(mTokenLock);
    *s = String("WakeLock{") /*Integer.toHexString(System.identityHashCode(this))*/
            + String(" held=") + StringUtils::BooleanToString(mHeld) + String(", refCount=")
            + StringUtils::Int32ToString(mCount) + String("}");
    return NOERROR;
}

ECode CPowerManagerWakeLock::constructor(
    /* [in] */ Int32 flags,
    /* [in] */ const String& tag,
    /* [in] */ Handle32 host)
{
    mFlags = flags;
    mTag = tag;
    FAIL_RETURN(CBinder::New((IBinder**)&mToken));
    mHost = (CPowerManager*)host;
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
