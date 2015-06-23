#include "os/CUpdateLock.h"
#include <elastos/utility/logging/Logger.h>
#include "os/ServiceManager.h"
#include "os/CBinder.h"

using Elastos::Droid::Os::CBinder;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Os {

const Boolean CUpdateLock::DEBUG = FALSE;
const String CUpdateLock::TAG("UpdateLock");

AutoPtr<IIUpdateLock> CUpdateLock::sService;

void CUpdateLock::CheckService()
{
    if (sService == NULL) {
        sService = IIUpdateLock::Probe(ServiceManager::GetService(IContext::UPDATE_LOCK_SERVICE).Get());
    }
}

CUpdateLock::CUpdateLock()
    : mCount(0)
    , mRefCounted(TRUE)
    , mHeld(FALSE)
{
}

ECode CUpdateLock::constructor(
    /* [in] */ const String& tag)
{
    mTag = tag;
    CBinder::New((IBinder**)&mToken);
    return NOERROR;
}

ECode CUpdateLock::SetReferenceCounted(
    /* [in] */ Boolean isRefCounted)
{
    if (DEBUG) {
        // Logger::V(TAG, "setting refcounted=" + isRefCounted + " : " + this);
    }
    mRefCounted = isRefCounted;
    return NOERROR;
}

ECode CUpdateLock::IsHeld(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    {
        Mutex::Autolock lock(mLock);
        *result = mHeld;
    }
    return NOERROR;
}

ECode CUpdateLock::AcquireLock()
{
    if (DEBUG) {
        // Log.v(TAG, "acquire() : " + this, new RuntimeException("here"));
    }
    CheckService();
    {
        Mutex::Autolock lock(mLock);
        AcquireLocked();
    }
    return NOERROR;
}

ECode CUpdateLock::AcquireLocked()
{
    if (!mRefCounted || mCount++ == 0) {
        if (sService != NULL) {
            ECode ec = sService->AcquireUpdateLock(mToken, mTag);
            if (FAILED(ec)) {
                Logger::E(TAG, "Unable to contact service to acquire");
                return E_REMOTE_EXCEPTION;
            }
        }
        mHeld = TRUE;
    }
    return NOERROR;
}

ECode CUpdateLock::ReleaseLock()
{
    if (DEBUG) {
        // Log.v(TAG, "release() : " + this, new RuntimeException("here"));
    }
    CheckService();
    {
        Mutex::Autolock lock(mLock);
        ReleaseLocked();
    }
    return NOERROR;
}

ECode CUpdateLock::ReleaseLocked()
{
    if (!mRefCounted || --mCount == 0) {
        if (sService != NULL) {
            ECode ec = sService->ReleaseUpdateLock(mToken);
            if (FAILED(ec)) {
                Logger::E(TAG, "Unable to contact service to release");
                return E_REMOTE_EXCEPTION;
            }
        }
        mHeld = FALSE;
    }
    if (mCount < 0) {
        Logger::E(TAG, "UpdateLock under-locked");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

CUpdateLock::~CUpdateLock()
{
    Mutex::Autolock lock(mLock);
    // if mHeld is true, sService must be non-null
    if (mHeld) {
        // Log.wtf(TAG, "UpdateLock finalized while still held");
        ECode ec = sService->ReleaseUpdateLock(mToken);
        if (FAILED(ec)) {
            Logger::E(TAG, "Unable to contact service to release");
            // return E_REMOTE_EXCEPTION;
        }
    }
}

} // namespace Os
} // namespace Droid
} // namespace Elastos