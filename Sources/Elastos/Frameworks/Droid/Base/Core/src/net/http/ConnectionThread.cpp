
#include "ConnectionThread.h"
#include "ext/frameworkext.h"
// #include "os/SystemClock.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

const Int32 ConnectionThread::WAIT_TIMEOUT = 5000;
const Int32 ConnectionThread::WAIT_TICK = 1000;

ConnectionThread::ConnectionThread(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [in] */ RequestQueue::ConnectionManager* connectionManager,
    /* [in] */ RequestFeeder* requestFeeder)
    : mCurrentThreadTime(0)
    , mTotalThreadTime(0)
    , mWaiting(FALSE)
    , mRunning(TRUE)
    , mContext(context)
    , mId(id)
    , mConnection(NULL)
    , mConnectionManager(connectionManager)
    , mRequestFeeder(requestFeeder)
{
    Thread::constructor();
    SetName(String("http") + StringUtils::Int32ToString(id));
}

ECode ConnectionThread::RequestStop()
{
    Object::Autolock lock(this);

    mRunning = FALSE;
    // return mRequestFeeder->Notify();
    return NOERROR;
}

ECode ConnectionThread::Run()
{
    // TODO:
    // AutoPtr<IProcessHelper> process;
    // CProcessHelper::AcquireSingleton((IProcessHelper**)&process);
    // process->SetThreadPriority(
    //         IProcess::THREAD_PRIORITY_DEFAULT +
    //         IProcess::THREAD_PRIORITY_LESS_FAVORABLE);

    // these are used to get performance data. When it is not in the timing,
    // mCurrentThreadTime is 0. When it starts timing, mCurrentThreadTime is
    // first set to -1, it will be set to the current thread time when the
    // next request starts.
    mCurrentThreadTime = 0;
    mTotalThreadTime = 0;

    // while (mRunning) {
    //     if (mCurrentThreadTime == -1) {
    //         mCurrentThreadTime = SystemClock::CurrentThreadTimeMillis();
    //     }

    //     Request* request;

    //     /* Get a request to process */
    //     mRequestFeeder->GetRequest(&request);

    //     /* wait for work */
    //     if (request == NULL) {
    //         AutoLock lock(mLock);

    //         // if (HttpLog.LOGV) HttpLog.v("ConnectionThread: Waiting for work");
    //         mWaiting = TRUE;
    //         mRequestFeeder->Wait();
    //         mWaiting = false;

    //         if (mCurrentThreadTime != 0) {
    //             mCurrentThreadTime = SystemClock::CurrentThreadTimeMillis();
    //         }
    //     } else {
    //         // if (HttpLog.LOGV) HttpLog.v("ConnectionThread: new request " +
    //         //                             request.mHost + " " + request );

    //         mConnectionManager->GetConnection(mContext, request->mHost, (IRequestFeeder**)&mConnection);
    //         mConnection->ProcessRequests(request);
    //         if (mConnection->GetCanPersist()) {
    //             if (!mConnectionManager->RecycleConnection(mConnection)) {
    //                 mConnection->CloseConnection();
    //             }
    //         } else {
    //             mConnection->CloseConnection();
    //         }
    //         mConnection = NULL;

    //         if (mCurrentThreadTime > 0) {
    //             Int64 start = mCurrentThreadTime;
    //             mCurrentThreadTime = SystemClock:CurrentThreadTimeMillis();
    //             mTotalThreadTime += mCurrentThreadTime - start;
    //         }
    //     }

    // }
    return NOERROR;
}

ECode ConnectionThread::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    String con;
    // if(mConnection != NULL) mConnection->ToString(*con);
    String active = mWaiting ? String("w") : String("a");
    *str = String("cid ") + StringUtils::Int32ToString(mId) + String(" ") + active + String(" ")  + con;
    return NOERROR;
}

ConnectionThread::ConnectionThread()
    : mCurrentThreadTime(0)
    , mTotalThreadTime(0)
    , mWaiting(FALSE)
    , mRunning(TRUE)
    , mId(0)
{
    Thread::constructor();
}

ConnectionThread::ConnectionThread(
    /* [in] */ IRunnable* runnable)
    : mCurrentThreadTime(0)
    , mTotalThreadTime(0)
    , mWaiting(FALSE)
    , mRunning(TRUE)
    , mId(0)
{
    Thread::constructor(runnable);
}

ConnectionThread::ConnectionThread(
    /* [in] */ IRunnable* runnable,
    /* [in] */ const String& threadName)
    : mCurrentThreadTime(0)
    , mTotalThreadTime(0)
    , mWaiting(FALSE)
    , mRunning(TRUE)
    , mId(0)
{
    Thread::constructor(runnable, threadName);
}

ConnectionThread::ConnectionThread(
    /* [in] */ const String& threadName)
    : mCurrentThreadTime(0)
    , mTotalThreadTime(0)
    , mWaiting(FALSE)
    , mRunning(TRUE)
    , mId(0)
{
    Thread::constructor(threadName);
}

ConnectionThread::ConnectionThread(
    /* [in] */ IThreadGroup* group,
    /* [in] */ IRunnable* runnable)
    : mCurrentThreadTime(0)
    , mTotalThreadTime(0)
    , mWaiting(FALSE)
    , mRunning(TRUE)
    , mId(0)
{
    Thread::constructor(group, runnable);
}

ConnectionThread::ConnectionThread(
    /* [in] */ IThreadGroup* group,
    /* [in] */ IRunnable* runnable,
    /* [in] */ const String& threadName)
    : mCurrentThreadTime(0)
    , mTotalThreadTime(0)
    , mWaiting(FALSE)
    , mRunning(TRUE)
    , mId(0)
{
    Thread::constructor(group, runnable, threadName);
}

ConnectionThread::ConnectionThread(
    /* [in] */ IThreadGroup* group,
    /* [in] */ const String& threadName)
    : mCurrentThreadTime(0)
    , mTotalThreadTime(0)
    , mWaiting(FALSE)
    , mRunning(TRUE)
    , mId(0)
{
    Thread::constructor(group, threadName);
}

ConnectionThread::ConnectionThread(
    /* [in] */ IThreadGroup* group,
    /* [in] */ IRunnable* runnable,
    /* [in] */ const String& threadName,
    /* [in] */ Int64 stackSize)
    : mCurrentThreadTime(0)
    , mTotalThreadTime(0)
    , mWaiting(FALSE)
    , mRunning(TRUE)
    , mId(0)
{
    Thread::constructor(group, runnable, threadName, stackSize);
}

}
}
}
}
