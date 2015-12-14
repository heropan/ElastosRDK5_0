
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Net.h"
#include "elastos/droid/net/http/ConnectionThread.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

const Int32 ConnectionThread::WAIT_TIMEOUT = 5000;
const Int32 ConnectionThread::WAIT_TICK = 1000;

ConnectionThread::ConnectionThread()
    : mCurrentThreadTime(0)
    , mTotalThreadTime(0)
    , mWaiting(FALSE)
    , mRunning(TRUE)
    , mId(0)
{}

ECode ConnectionThread::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [in] */ IRequestQueueConnectionManager* connectionManager,
    /* [in] */ IRequestFeeder* requestFeeder)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    Thread::constructor();
    mContext = context;
    SetName(String("http") + StringUtils::Int32ToString(id));
    mId = id;
    mConnectionManager = connectionManager;
    mRequestFeeder = requestFeeder;
#endif
}

ECode ConnectionThread::RequestStop()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    synchronized(this) {

        mRunning = FALSE;
        // return mRequestFeeder->Notify();
    }
    return NOERROR;
#endif
}

ECode ConnectionThread::Run()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
    //         synchronized(mLock) {

        //         // if (HttpLog.LOGV) HttpLog.v("ConnectionThread: Waiting for work");
        //         mWaiting = TRUE;
        //         mRequestFeeder->Wait();
        //         mWaiting = false;

        //         if (mCurrentThreadTime != 0) {
        //             mCurrentThreadTime = SystemClock::CurrentThreadTimeMillis();
        //         }
                }
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
#endif
}

ECode ConnectionThread::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(str);

    String con;
    // if(mConnection != NULL) mConnection->ToString(*con);
    String active = mWaiting ? String("w") : String("a");
    *str = String("cid ") + StringUtils::Int32ToString(mId) + String(" ") + active + String(" ")  + con;
    return NOERROR;
#endif
}


} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
