
#include "ext/frameworkdef.h"
#include "os/CHandlerThread.h"
#include "os/Process.h"
#include "os/Looper.h"

namespace Elastos {
namespace Droid {
namespace Os {

CAR_OBJECT_IMPL(CHandlerThread)

CHandlerThread::CHandlerThread()
    : mPriority(0)
    , mTid(0)
{
}

ECode CHandlerThread::constructor(
    /* [in] */ const String& name)
{
    Thread::constructor(name);
    mPriority = IProcess::THREAD_PRIORITY_DEFAULT;
    return NOERROR;
}

/**
 * Constructs a HandlerThread.
 * @param name
 * @param priority The priority to run the thread at. The value supplied must be from
 * {@link android.os.Process} and not from java.lang.Thread.
 */
ECode CHandlerThread::constructor(
    /* [in] */ const String& name,
    /* [in] */ Int32 priority)
{
    Thread::constructor(name);
    mPriority = priority;
    return NOERROR;
}

void CHandlerThread::OnLooperPrepared()
{}

ECode CHandlerThread::Run()
{
    mTid = Process::MyTid();
    Looper::Prepare();
    {
        AutoLock(this)

        mLooper = Looper::MyLooper();
        NotifyAll();
    }
    Process::SetThreadPriority(mPriority);
    OnLooperPrepared();
    Looper::Loop();
    mTid = -1;
    return NOERROR;
}

ECode CHandlerThread::GetLooper(
    /* [out] */ ILooper** looper)
{
    VALIDATE_NOT_NULL(looper);

    Boolean isAlive;
    if (IsAlive(&isAlive), !isAlive) {
        *looper = NULL;
        return NOERROR;
    }

    // If the thread has been started, wait until the looper has been created.
    {
        AutoLock(this)

        while ((IsAlive(&isAlive), isAlive) && mLooper == NULL) {
            // try {
            Wait();
            // } catch (InterruptedException e) {
            // }
        }
    }
    *looper = mLooper;
    REFCOUNT_ADD(*looper);
    return NOERROR;
}

ECode CHandlerThread::Quit(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ILooper> looper;
    FAIL_RETURN(GetLooper((ILooper**)&looper));
    if (looper != NULL) {
        looper->Quit();
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CHandlerThread::QuitSafely(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ILooper> looper;
    FAIL_RETURN(GetLooper((ILooper**)&looper));
    if (looper != NULL) {
        looper->QuitSafely();
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CHandlerThread::GetThreadId(
    /* [out] */ Int32* tid)
{
    VALIDATE_NOT_NULL(tid);

    *tid = mTid;
    return NOERROR;
}


} // namespace Os
} // namespace Droid
} // namespace Elastos
