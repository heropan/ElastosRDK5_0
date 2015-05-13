
#include "os/CSystemVibrator.h"
#include <elastos/Logger.h>
#include "os/ServiceManager.h"
#include "os/CBinder.h"

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::CBinder;

namespace Elastos {
namespace Droid {
namespace Os {

const String CSystemVibrator::TAG("Vibrator");

CSystemVibrator::CSystemVibrator()
{
    CBinder::New((IBinder**)&mToken);
}

ECode CSystemVibrator::constructor() {
    mService = IVibratorService::Probe(ServiceManager::GetService(String("vibrator")).Get());
    return NOERROR;
}

ECode CSystemVibrator::HasVibrator(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mService == NULL) {
        Logger::W(TAG, "Failed to vibrate; no vibrator service.");
        *result = FALSE;
        return NOERROR;
    }

    ECode ec = mService->HasVibrator(result);
    if (FAILED(ec)) {
        *result = FALSE;
    }
    return ec;
}

ECode CSystemVibrator::Vibrate(
    /* [in] */ Int64 milliseconds)
{
    if (mService == NULL) {
        Logger::W(TAG, "Failed to vibrate; no vibrator service.");
        return E_NULL_POINTER_EXCEPTION;
    }

    if (FAILED(mService->Vibrate(milliseconds, mToken))) {
         // Log.w(TAG, "Failed to vibrate.", e);
    }

    return NOERROR;
}

ECode CSystemVibrator::VibrateEx(
    /* [in] */ const ArrayOf<Int64>& pattern,
    /* [in] */ Int32 repeat)
{
    if (mService == NULL) {
        Logger::W(TAG, "Failed to vibrate; no vibrator service.");
        return E_NULL_POINTER_EXCEPTION;
    }
    // catch this here because the server will do nothing.  pattern may
    // not be null, let that be checked, because the server will drop it
    // anyway
    if (repeat < pattern.GetLength()) {
        if(FAILED(mService->VibratePattern(const_cast<ArrayOf<Int64>* >(&pattern), repeat, mToken))) {
            // Log.w(TAG, "Failed to vibrate.", e);
        }
        return NOERROR;
    }
    else {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
}

ECode CSystemVibrator::Cancel()
{
    if (mService == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if(FAILED(mService->CancelVibrate(mToken))) {
        // Log.w(TAG, "Failed to cancel vibration.", e);
    }
    return NOERROR;
}


} // namespace Os
} // namespace Droid
} // namespace Elastos