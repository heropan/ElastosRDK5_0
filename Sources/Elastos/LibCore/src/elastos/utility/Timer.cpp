
#include "cmdef.h"
#include "Timer.h"
#include "CSystem.h"
#include <elastos/Autolock.h>
#include <elastos/StringBuilder.h>

using Elastos::Core::ISystem;
using Elastos::Core::Autolock;
using Elastos::Core::StringBuilder;
using Elastos::Core::ISynchronize;

namespace Elastos {
namespace Utility {

Timer::TimerImpl::TimerHeap::TimerHeap()
    : DEFAULT_HEAP_SIZE(256)
    , mTimers(ArrayOf<ITimerTask*>::Alloc(DEFAULT_HEAP_SIZE))
    , mSize(0)
    , mDeletedCancelledNumber(0)
{}

Timer::TimerImpl::TimerHeap::~TimerHeap()
{}

AutoPtr<ITimerTask> Timer::TimerImpl::TimerHeap::Minimum()
{
    return (*mTimers)[0];
}

Boolean Timer::TimerImpl::TimerHeap::IsEmpty()
{
    return mSize == 0;
}

void Timer::TimerImpl::TimerHeap::Insert(
    /* [in] */ ITimerTask* task)
{
    if (mTimers->GetLength() == mSize) {
        AutoPtr< ArrayOf<ITimerTask*> > appendedTimers = ArrayOf<ITimerTask*>::Alloc(mSize * 2);
        appendedTimers->Copy(mTimers);
        mTimers = appendedTimers;
    }
    mTimers->Set(mSize++, task);
    UpHeap();
}

void Timer::TimerImpl::TimerHeap::Delete(
    /* [in] */ Int32 pos)
{
    if (pos >= 0 && pos < mSize) {
        mTimers->Set(pos, (*mTimers)[--mSize]);
        mTimers->Set(mSize, NULL);
        DownHeap(pos);
    }
}

void Timer::TimerImpl::TimerHeap::UpHeap()
{
    Int32 current = mSize - 1;
    Int32 parent = (current - 1) / 2;

    Int64 curWhen, parWhen;
    while ((*mTimers)[current]->GetWhen(&curWhen),
            (*mTimers)[parent]->GetWhen(&parWhen),
            curWhen < parWhen) {
        // swap the two
        AutoPtr<ITimerTask> tmp = (*mTimers)[current];
        mTimers->Set(current, (*mTimers)[parent]);
        mTimers->Set(parent, tmp);

        // update pos and current
        current = parent;
        parent = (current - 1) / 2;
    }
}

void Timer::TimerImpl::TimerHeap::DownHeap(
    /* [in] */ Int32 pos)
{
    Int32 current = pos;
    Int32 child = 2 * current + 1;

    Int64 when1, when2;
    while (child < mSize && mSize > 0) {
        // compare the children if they exist
        if (child + 1 < mSize
            && ((*mTimers)[child + 1]->GetWhen(&when1),
                (*mTimers)[child]->GetWhen(&when2),
                when1 < when2)) {
            child++;
        }

        // compare selected child with parent
        if ((*mTimers)[current]->GetWhen(&when1),
            (*mTimers)[child]->GetWhen(&when2),
            when1 < when2) {
            break;
        }

        // swap the two
        AutoPtr<ITimerTask> tmp = (*mTimers)[current];
        mTimers->Set(current, (*mTimers)[child]);
        mTimers->Set(child, tmp);

        // update pos and current
        current = child;
        child = 2 * current + 1;
    }
}

void Timer::TimerImpl::TimerHeap::Reset()
{
    mTimers = ArrayOf<ITimerTask*>::Alloc(DEFAULT_HEAP_SIZE);;
    mSize = 0;
}

void Timer::TimerImpl::TimerHeap::AdjustMinimum()
{
    DownHeap(0);
}

void Timer::TimerImpl::TimerHeap::DeleteIfCancelled()
{
    for (Int32 i = 0; i < mSize; i++) {
        Boolean cancelled;
        if ((*mTimers)[i]->IsCancelled(&cancelled), cancelled) {
            mDeletedCancelledNumber++;
            Delete(i);
            // re-try this point
            i--;
        }
    }
}

ECode Timer::TimerImpl::TimerHeap::GetTask(
    /* [in] */ ITimerTask* task)
{
    for (Int32 i = 0; i < mTimers->GetLength(); i++) {
        if ((*mTimers)[i] == task) {
            return i;
        }
    }
    return -1;
}


Timer::TimerImpl::TimerImpl(
    /* [in] */ const String& name,
    /* [in] */ Boolean isDaemon)
    : mCancelled(FALSE)
    , mFinished(FALSE)
    , mTasks(new TimerHeap())
{
    Thread::Init();
    Thread::SetName(name);
    Thread::SetDaemon(isDaemon);
    Thread::Start();
}

Timer::TimerImpl::~TimerImpl()
{}

PInterface Timer::TimerImpl::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IThread) {
        return (IThread*)this;
    }
    else if (riid == EIID_ISynchronize) {
        return (ISynchronize*)this;
    }

    return NULL;
}

UInt32 Timer::TimerImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Timer::TimerImpl::Release()
{
    return ElRefBase::Release();
}

ECode Timer::TimerImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pObject == (IInterface*)(IThread*)this) {
        *pIID = EIID_IThread;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode Timer::TimerImpl::CheckAccess()
{
    return Thread::CheckAccess();
}

ECode Timer::TimerImpl::CountStackFrames(
    /* [out] */ Int32* number)
{
    return Thread::CountStackFrames(number);
}

ECode Timer::TimerImpl::Destroy()
{
    return Thread::Destroy();
}

ECode Timer::TimerImpl::GetId(
    /* [out] */ Int64* id)
{
    return Thread::GetId(id);
}

ECode Timer::TimerImpl::GetName(
    /* [out] */ String* name)
{
    return Thread::GetName(name);
}

ECode Timer::TimerImpl::GetPriority(
    /* [out] */ Int32* priority)
{
    return Thread::GetPriority(priority);
}

ECode Timer::TimerImpl::GetState(
    /* [out] */ ThreadState* state)
{
    return Thread::GetState(state);
}

ECode Timer::TimerImpl::GetThreadGroup(
    /* [out] */ IThreadGroup** group)
{
    return Thread::GetThreadGroup(group);
}

ECode Timer::TimerImpl::Interrupt()
{
    return Thread::Interrupt();
}

ECode Timer::TimerImpl::IsAlive(
    /* [out] */ Boolean* isAlive)
{
    return Thread::IsAlive(isAlive);
}

ECode Timer::TimerImpl::IsDaemon(
    /* [out] */ Boolean* isDaemon)
{
    return Thread::IsDaemon(isDaemon);
}

ECode Timer::TimerImpl::IsInterrupted(
    /* [out] */ Boolean* isInterrupted)
{
    return Thread::IsInterrupted(isInterrupted);
}

ECode Timer::TimerImpl::Join()
{
    return Thread::Join();
}

ECode Timer::TimerImpl::JoinEx(
    /* [in] */ Int64 millis)
{
    return Thread::JoinEx(millis);
}

ECode Timer::TimerImpl::JoinEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::JoinEx2(millis, nanos);
}

ECode Timer::TimerImpl::Resume()
{
    return Thread::Resume();
}

ECode Timer::TimerImpl::SetDaemon(
    /* [in] */ Boolean isDaemon)
{
    return Thread::SetDaemon(isDaemon);
}

ECode Timer::TimerImpl::SetName(
    /* [in] */ const String& threadName)
{
    return Thread::SetName(threadName);
}

ECode Timer::TimerImpl::SetPriority(
    /* [in] */ Int32 priority)
{
    return Thread::SetPriority(priority);
}

ECode Timer::TimerImpl::Start()
{
    return Thread::Start();
}

ECode Timer::TimerImpl::Stop()
{
    return Thread::Stop();
}

ECode Timer::TimerImpl::Suspend()
{
    return Thread::Suspend();
}

ECode Timer::TimerImpl::Detach()
{
    return Thread::Detach();
}

ECode Timer::TimerImpl::Unpark()
{
    return Thread::Unpark();
}

ECode Timer::TimerImpl::ParkFor(
    /* [in] */ Int64 nanos)
{
    return Thread::ParkFor(nanos);
}

ECode Timer::TimerImpl::ParkUntil(
    /* [in] */ Int64 time)
{
    return Thread::ParkUntil(time);
}

ECode Timer::TimerImpl::Lock()
{
    return Thread::Lock();
}

ECode Timer::TimerImpl::Unlock()
{
    return Thread::Unlock();
}

ECode Timer::TimerImpl::Notify()
{
    return Thread::Notify();
}

ECode Timer::TimerImpl::NotifyAll()
{
    return Thread::NotifyAll();
}

ECode Timer::TimerImpl::Wait()
{
    return Thread::Wait();
}

ECode Timer::TimerImpl::WaitEx(
    /* [in] */ Int64 millis)
{
    return Thread::Wait(millis);
}

ECode Timer::TimerImpl::WaitEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::Wait(millis, nanos);
}

Mutex* Timer::TimerImpl::GetSelfLock()
{
    return &mLock;
}

ECode Timer::TimerImpl::Run()
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    while (TRUE) {
        AutoPtr<ITimerTask> task;
        {
            Autolock lock(this);
            // need to check cancelled inside the synchronized block
            if (mCancelled) {
                return NOERROR;
            }
            if (mTasks->IsEmpty()) {
                if (mFinished) {
                    return NOERROR;
                }
                // no tasks scheduled -- sleep until any task appear
                // try {
                Wait();
                // } catch (InterruptedException ignored) {
                // }
                continue;
            }

            Int64 currentTime;
            system->GetCurrentTimeMillis(&currentTime);

            task = mTasks->Minimum();
            Int64 timeToSleep;

            task->Lock();
            {
                Boolean cancelled;
                if (task->IsCancelled(&cancelled), cancelled) {
                    mTasks->Delete(0);
                    task->Unlock();
                    continue;
                }

                // check the time to sleep for the first task scheduled
                Int64 when;
                task->GetWhen(&when);
                timeToSleep = when - currentTime;
            }
            task->Unlock();

            if (timeToSleep > 0) {
                // sleep!
                // try {
                WaitEx(timeToSleep);
                // } catch (InterruptedException ignored) {
                // }
                continue;
            }

            // no sleep is necessary before launching the task

            task->Lock();
            {
                Int32 pos = 0;
                Int64 when1, when2;
                mTasks->Minimum()->GetWhen(&when1);
                task->GetWhen(&when2);
                if (when1 != when2) {
                    pos = mTasks->GetTask(task);
                }
                Boolean cancelled;
                task->IsCancelled(&cancelled);
                if (cancelled) {
                    mTasks->Delete(mTasks->GetTask(task));
                    task->Unlock();
                    continue;
                }

                // set time to schedule
                task->SetScheduledTime(when2);

                // remove task from queue
                mTasks->Delete(pos);

                // set when the next task should be launched
                Int64 period;
                if (task->GetPeriod(&period), period >= 0) {
                    // this is a repeating task,
                    Boolean fixed;
                    if (task->IsFixedRate(&fixed), fixed) {
                        // task is scheduled at fixed rate
                        task->SetWhen(when2 + period);
                    }
                    else {
                        // task is scheduled at fixed delay
                        Int64 millis;
                        system->GetCurrentTimeMillis(&millis);
                        task->SetWhen(millis + period);
                    }

                    // insert this task into queue
                    InsertTask(task);
                }
                else {
                    task->SetWhen(0);
                }
            }
            task->Unlock();
        }

        Boolean taskCompletedNormally = FALSE;
        if (SUCCEEDED(task->Run())) {
            taskCompletedNormally = TRUE;
        }
        if (!taskCompletedNormally) {
            Autolock lock(this);
            mCancelled = TRUE;
        }
    }
    return NOERROR;
}

void Timer::TimerImpl::InsertTask(
    /* [in] */ ITimerTask* newTask)
{
    // callers are synchronized
    mTasks->Insert(newTask);
    Notify();
}

void Timer::TimerImpl::Cancel()
{
    Autolock lock(this);
    mCancelled = TRUE;
    mTasks->Reset();
    Notify();
}

Int32 Timer::TimerImpl::Purge()
{
    if (mTasks->IsEmpty()) {
        return 0;
    }
    // callers are synchronized
    mTasks->mDeletedCancelledNumber = 0;
    mTasks->DeleteIfCancelled();
    return mTasks->mDeletedCancelledNumber;
}

ECode Timer::TimerImpl::GetUncaughtExceptionHandler(
    /* [out] */ IThreadUncaughtExceptionHandler** handler)
{
    return Thread::GetUncaughtExceptionHandler(handler);
}

ECode Timer::TimerImpl::GetContextClassLoader(
    /* [out] */ IClassLoader** outload)
{
    return Thread::GetContextClassLoader(outload);
}

ECode Timer::TimerImpl::SetContextClassLoader(
    /* [in] */ IClassLoader* cl)
{
    return Thread::SetContextClassLoader(cl);
}

ECode Timer::TimerImpl::PushInterruptAction(
    /* [in] */ IRunnable* interruptAction)
{
    return Thread::PushInterruptAction(interruptAction);
}

ECode Timer::TimerImpl::PopInterruptAction(
    /* [in] */ IRunnable* interruptAction)
{
    return Thread::PopInterruptAction(interruptAction);
}

ECode Timer::TimerImpl::SetUncaughtExceptionHandler(
    /* [in] */ IThreadUncaughtExceptionHandler* handler)
{
    return Thread::SetUncaughtExceptionHandler(handler);
}

Timer::FinalizerHelper::FinalizerHelper(
    /* [in] */ TimerImpl* impl)
    : mImpl(impl)
{}

Timer::FinalizerHelper::~FinalizerHelper()
{
    Autolock lock(mImpl);
    mImpl->mFinished = TRUE;
    mImpl->Notify();
}


Int64 Timer::sTimerId = 0;
Mutex Timer::sTimerIdLock;

Int64 Timer::NextId()
{
    Mutex::Autolock lock(&sTimerIdLock);
    return sTimerId++;
}

Timer::Timer()
{}

Timer::Timer(
    /* [in] */ const String& name,
    /* [in] */ Boolean isDaemon)
{
    ASSERT_SUCCEEDED(Init(name, isDaemon));
}

Timer::Timer(
    /* [in] */ const String& name)
{
    ASSERT_SUCCEEDED(Init(name, FALSE));
}

Timer::Timer(
    /* [in] */Boolean isDaemon)
{
    ASSERT_SUCCEEDED(Init(isDaemon));
}

Timer::~Timer()
{}

ECode Timer::Init(
    /* [in] */ const String& name,
    /* [in] */ Boolean isDaemon)
{
    if (name.IsNull()){
        // throw new NullPointerException("name is null");
        return E_NULL_POINTER_EXCEPTION;
    }
    mImpl = new TimerImpl(name, isDaemon);
    mFinalizer = new FinalizerHelper(mImpl);
    return NOERROR;
}

ECode Timer::Init(
    /* [in] */Boolean isDaemon)
{
    StringBuilder sb;
    sb += "Timer-";
    sb += Timer::NextId();
    return Init(sb.ToString(), isDaemon);
}

ECode Timer::Init()
{
    return Init(FALSE);
}

ECode Timer::Cancel()
{
    mImpl->Cancel();
    return NOERROR;
}

ECode Timer::Purge(
    /* [out] */ Int32* number)
{
    Autolock lock(mImpl);
    *number = mImpl->Purge();
    return NOERROR;
}

ECode Timer::Schedule(
    /* [in] */ ITimerTask* task,
    /* [in] */ IDate* when)
{
    Int64 time;
    when->GetTime(&time);
    if (time < 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 delay;
    system->GetCurrentTimeMillis(&delay);
    delay = time - delay;
    return ScheduleImpl(task, delay < 0 ? 0 : delay, -1, FALSE);
}

ECode Timer::Schedule(
    /* [in] */ ITimerTask* task,
    /* [in] */ Int64 delay)
{
    if (delay < 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return ScheduleImpl(task, delay, -1, FALSE);
}

ECode Timer::Schedule(
    /* [in] */ ITimerTask* task,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period)
{
    if (delay < 0 || period <= 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return ScheduleImpl(task, delay, period, FALSE);
}

ECode Timer::Schedule(
    /* [in] */ ITimerTask* task,
    /* [in] */ IDate* when,
    /* [in] */ Int64 period)
{
    Int64 time;
    when->GetTime(&time);
    if (period <= 0 || time < 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 delay;
    system->GetCurrentTimeMillis(&delay);
    delay = time - delay;
    return ScheduleImpl(task, delay < 0 ? 0 : delay, period, FALSE);
}

ECode Timer::ScheduleAtFixedRate(
    /* [in] */ ITimerTask* task,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period)
{
    if (delay < 0 || period <= 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return ScheduleImpl(task, delay, period, TRUE);
}

ECode Timer::ScheduleAtFixedRate(
    /* [in] */ ITimerTask* task,
    /* [in] */ IDate* when,
    /* [in] */ Int64 period)
{
    Int64 time;
    when->GetTime(&time);
    if (period <= 0 || time < 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    Int64 delay;
    system->GetCurrentTimeMillis(&delay);
    delay = time - delay;
    return ScheduleImpl(task, delay, period, TRUE);
}

ECode Timer::ScheduleImpl(
    /* [in] */ ITimerTask* task,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period,
    /* [in] */ Boolean fixed)
{
    Autolock lock(mImpl);

    if (mImpl->mCancelled) {
        // throw new IllegalStateException("Timer was canceled");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    Int64 when;
    system->GetCurrentTimeMillis(&when);
    when = delay + when;

    if (when < 0) {
        // throw new IllegalArgumentException("Illegal delay to start the TimerTask: " + when);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    task->Lock();
    {
        Boolean scheduled;
        if (task->IsScheduled(&scheduled), scheduled) {
            // throw new IllegalStateException("TimerTask is scheduled already");
            task->Unlock();
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        Boolean cancelled;
        if (task->IsCancelled(&cancelled), cancelled) {
            // throw new IllegalStateException("TimerTask is canceled");
            task->Unlock();
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        task->SetWhen(when);
        task->SetPeriod(period);
        task->SetFixedRate(fixed);
    }
    task->Unlock();

    // insert the newTask into queue
    mImpl->InsertTask(task);
    return NOERROR;
}

} // namespace Utility
} // namespace Elastos
