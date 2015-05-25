
#include "CReentrantLock.h"
#include "elastos/Thread.h"

using Elastos::Core::Thread;

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Locks {

ECode CReentrantLock::constructor()
{
    return ReentrantLock::Init();
}

ECode CReentrantLock::constructor(
    /* [in] */ Boolean fair)
{
    return ReentrantLock::Init(fair);
}

ECode CReentrantLock::Lock()
{
    return ReentrantLock::Lock();
}

ECode CReentrantLock::LockInterruptibly()
{
    return ReentrantLock::LockInterruptibly();
}


ECode CReentrantLock::TryLock(
    /* [out] */ Boolean* result)
{
    return ReentrantLock::TryLock(result);
}

ECode CReentrantLock::TryLockEx(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ Boolean* result)
{
    return ReentrantLock::TryLockEx(timeout, unit, result);
}

ECode CReentrantLock::UnLock()
{
    return ReentrantLock::UnLock();
}

ECode CReentrantLock::NewCondition(
    /* [out] */ ICondition** condition)
{
    return ReentrantLock::NewCondition(condition);
}

ECode CReentrantLock::GetHoldCount(
    /* [out] */ Int32* count)
{
    return ReentrantLock::GetHoldCount(count);
}

ECode CReentrantLock::IsHeldByCurrentThread(
    /* [out] */ Boolean* result)
{
    return ReentrantLock::IsHeldByCurrentThread(result);
}

ECode CReentrantLock::IsLocked(
    /* [out] */ Boolean* result)
{
    return ReentrantLock::IsLocked(result);
}

ECode CReentrantLock::IsFair(
    /* [out] */ Boolean* result)
{
    return ReentrantLock::IsFair(result);
}

ECode CReentrantLock::HasQueuedThreads(
    /* [out] */ Boolean* result)
{
    return ReentrantLock::HasQueuedThreads(result);
}

ECode CReentrantLock::HasQueuedThread(
    /* [in] */ IThread* thread,
    /* [out] */ Boolean* result)
{
    return ReentrantLock::HasQueuedThread(thread, result);
}

ECode CReentrantLock::GetQueueLength(
    /* [out] */ Int32* result)
{
    return ReentrantLock::GetQueueLength(result);
}

ECode CReentrantLock::HasWaiters(
    /* [in] */ ICondition* condition,
    /* [out] */ Boolean* result)
{
    return ReentrantLock::HasWaiters(condition, result);
}

ECode CReentrantLock::GetWaitQueueLength(
    /* [in] */ ICondition* condition,
    /* [out] */ Int32* result)
{
    return ReentrantLock::GetWaitQueueLength(condition, result);
}

ECode CReentrantLock::ToString(
    /* [out] */ String* info)
{
    return ReentrantLock::ToString(info);
}

} // namespace Locks
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
