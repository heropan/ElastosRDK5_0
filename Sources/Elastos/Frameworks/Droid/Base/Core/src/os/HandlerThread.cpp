
#include "os/HandlerThread.h"
#include "os/Process.h"
#include "os/Looper.h"

namespace Elastos {
namespace Droid {
namespace Os {

HandlerThread::HandlerThread(
    /* [in] */ const String& name)
{
    Thread::Init(name);
    mPriority = IProcess::THREAD_PRIORITY_DEFAULT;
}

HandlerThread::HandlerThread(
    /* [in] */ const String& name,
    /* [in] */ Int32 priority)
{
    Thread::Init(name);
    mPriority = priority;
}

void HandlerThread::OnLooperPrepared()
{}

ECode HandlerThread::Run()
{
    mTid = Process::MyTid();
    Looper::Prepare();
    {
        Lock();

        mLooper = Looper::MyLooper();
        NotifyAll();

        Unlock();
    }
    Process::SetThreadPriority(mPriority);
    OnLooperPrepared();
    Looper::Loop();
    mTid = -1;
    return NOERROR;
}

ECode HandlerThread::GetLooper(
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
        Lock();

        while ((IsAlive(&isAlive), isAlive) && mLooper == NULL) {
            // try {
            Wait();
            // } catch (InterruptedException e) {
            // }
        }

        Unlock();
    }
    *looper = mLooper;
    INTERFACE_ADDREF(*looper);
    return NOERROR;
}

ECode HandlerThread::Quit(
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

ECode HandlerThread::GetThreadId(
    /* [out] */ Int32* tid)
{
    VALIDATE_NOT_NULL(tid);

    *tid = mTid;
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
