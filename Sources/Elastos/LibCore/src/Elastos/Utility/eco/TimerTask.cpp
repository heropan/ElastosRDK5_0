
#include <elastos/TimerTask.h>

using Elastos::Core::EIID_IRunnable;

namespace Elastos {
namespace Utility {

CAR_INTERFACE_IMPL_2(TimerTask, ITimerTask, IRunnable);

TimerTask::TimerTask()
    : mCancelled(FALSE)
    , mWhen(0)
    , mPeriod(0)
    , mFixedRate(FALSE)
    , mScheduledTime(0)
{}

ECode TimerTask::GetWhen(
    /* [out] */ Int64* when)
{
    VALIDATE_NOT_NULL(when)
    Mutex::Autolock lock(mLock);
    *when = mWhen;
    return NOERROR;
}

ECode TimerTask::SetScheduledTime(
    /* [in] */ Int64 time)
{
    Mutex::Autolock lock(mLock);
    mScheduledTime = time;
    return NOERROR;
}

ECode TimerTask::IsScheduled(
    /* [out] */ Boolean* scheduled)
{
    VALIDATE_NOT_NULL(scheduled)
    Mutex::Autolock lock(mLock);
    *scheduled = mWhen > 0 || mScheduledTime > 0;
    return NOERROR;
}

ECode TimerTask::Cancel(
    /* [out] */ Boolean* cancelled)
{
    VALIDATE_NOT_NULL(cancelled)
    Mutex::Autolock lock(mLock);
    Boolean willRun = !mCancelled && mWhen > 0;
    mCancelled = TRUE;
    *cancelled = willRun;
    return NOERROR;
}

ECode TimerTask::ScheduledExecutionTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time)
    Mutex::Autolock lock(mLock);
    *time = mScheduledTime;
    return NOERROR;
}

ECode TimerTask::SetWhen(
    /* [in] */ Int64 when)
{
    Mutex::Autolock lock(mLock);
    mWhen = when;
    return NOERROR;
}

ECode TimerTask::IsCancelled(
    /* [out] */ Boolean* cancelled)
{
    VALIDATE_NOT_NULL(cancelled)
    Mutex::Autolock lock(mLock);
    *cancelled = mCancelled;
    return NOERROR;
}

ECode TimerTask::GetPeriod(
    /* [out] */ Int64* period)
{
    VALIDATE_NOT_NULL(period)
    Mutex::Autolock lock(mLock);
    *period = mPeriod;
    return NOERROR;
}

ECode TimerTask::SetPeriod(
    /* [in] */ Int64 period)
{
    Mutex::Autolock lock(mLock);
    mPeriod = period;
    return NOERROR;
}

ECode TimerTask::IsFixedRate(
    /* [out] */ Boolean* fixed)
{
    VALIDATE_NOT_NULL(fixed);
    Mutex::Autolock lock(mLock);
    *fixed = mFixedRate;
    return NOERROR;
}

ECode TimerTask::SetFixedRate(
    /* [in] */ Boolean fixed)
{
    Mutex::Autolock lock(mLock);
    mFixedRate = fixed;
    return NOERROR;
}

ECode TimerTask::Lock()
{
    mLock.Lock();
    return NOERROR;
}

ECode TimerTask::Unlock()
{
    mLock.Unlock();
    return NOERROR;
}

} // namespace Utility
} // namespace Elastos
