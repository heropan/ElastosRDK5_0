
#include "elastos/droid/os/CStrictModeThreadPolicyBuilder.h"
#include "elastos/droid/os/CStrictMode.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CStrictModeThreadPolicyBuilder::constructor()
{
    mMask = 0;
    return NOERROR;
}

ECode CStrictModeThreadPolicyBuilder::constructor(
    /* [in] */ IStrictModeThreadPolicy* policy)
{
    CStrictMode::ThreadPolicy* tp = (CStrictMode::ThreadPolicy*)policy;
    mMask = tp->mMask;
    return NOERROR;
}

ECode CStrictModeThreadPolicyBuilder::DetectAll()
{
    return Enable(CStrictMode::ALL_THREAD_DETECT_BITS);
}

ECode CStrictModeThreadPolicyBuilder::PermitAll()
{
    return Disable(CStrictMode::ALL_THREAD_DETECT_BITS);
}

ECode CStrictModeThreadPolicyBuilder::DetectNetwork()
{
    return Enable(IStrictMode::DETECT_NETWORK);
}

ECode CStrictModeThreadPolicyBuilder::PermitNetwork()
{
    return Disable(IStrictMode::DETECT_NETWORK);
}

ECode CStrictModeThreadPolicyBuilder::DetectDiskReads()
{
    return Enable(IStrictMode::DETECT_DISK_READ);
}

ECode CStrictModeThreadPolicyBuilder::PermitDiskReads()
{
    return Disable(IStrictMode::DETECT_DISK_READ);
}

ECode CStrictModeThreadPolicyBuilder::DetectCustomSlowCalls()
{
    return Enable(IStrictMode::DETECT_CUSTOM);
}

ECode CStrictModeThreadPolicyBuilder::PermitCustomSlowCalls()
{
    return Disable(IStrictMode::DETECT_CUSTOM);
}

ECode CStrictModeThreadPolicyBuilder::DetectDiskWrites()
{
    return Enable(IStrictMode::DETECT_DISK_WRITE);
}

ECode CStrictModeThreadPolicyBuilder::PermitDiskWrites()
{
    return Disable(IStrictMode::DETECT_DISK_WRITE);
}

ECode CStrictModeThreadPolicyBuilder::PenaltyDialog()
{
    return Enable(IStrictMode::PENALTY_DIALOG);
}

ECode CStrictModeThreadPolicyBuilder::PenaltyDeath()
{
    return Enable(IStrictMode::PENALTY_DEATH);
}

ECode CStrictModeThreadPolicyBuilder::PenaltyDeathOnNetwork()
{
    return Enable(IStrictMode::PENALTY_DEATH_ON_NETWORK);
}

ECode CStrictModeThreadPolicyBuilder::PenaltyFlashScreen()
{
    return Enable(IStrictMode::PENALTY_FLASH);
}

ECode CStrictModeThreadPolicyBuilder::PenaltyLog()
{
    return Enable(IStrictMode::PENALTY_LOG);
}

ECode CStrictModeThreadPolicyBuilder::PenaltyDropBox()
{
    return Enable(IStrictMode::PENALTY_DROPBOX);
}

ECode CStrictModeThreadPolicyBuilder::Build(
    /* [out] */ IStrictModeThreadPolicy** policy)
{
    // If there are detection bits set but no violation bits
    // set, enable simple logging.
    VALIDATE_NOT_NULL(policy);
    if (mMask != 0 &&
        (mMask & (IStrictMode::PENALTY_DEATH | IStrictMode::PENALTY_LOG |
                  IStrictMode::PENALTY_DROPBOX | IStrictMode::PENALTY_DIALOG)) == 0) {
        PenaltyLog();
    }
    AutoPtr<IStrictModeThreadPolicy> tp = new CStrictMode::ThreadPolicy(mMask);
    *policy = tp;
    REFCOUNT_ADD(*policy);
    return NOERROR;
}

ECode CStrictModeThreadPolicyBuilder::Enable(
    /* [in] */ Int32 bit)
{
    mMask |= bit;
    return NOERROR;
}

ECode CStrictModeThreadPolicyBuilder::Disable(
    /* [in] */ Int32 bit)
{
    mMask &= ~bit;
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos