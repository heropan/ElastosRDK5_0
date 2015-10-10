
#ifndef __ELASTOS_DROID_NET_HTTP_CONNECTIONTHREAD_H__
#define __ELASTOS_DROID_NET_HTTP_CONNECTIONTHREAD_H__

#include "elastos/droid/ext/frameworkext.h"
#include "RequestFeeder.h"
#include "RequestQueue.h"

using namespace Elastos::Core;

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

class ConnectionThread
    : public ThreadBase
{
public:
    ConnectionThread(
        /* [in] */ IContext* context,
        /* [in] */ Int32 id,
        /* [in] */ RequestQueue::ConnectionManager* connectionManager,
        /* [in] */ RequestFeeder* requestFeeder);

    ConnectionThread();

    ConnectionThread(
        /* [in] */ IRunnable* runnable);

    ConnectionThread(
        /* [in] */ IRunnable* runnable,
        /* [in] */ const String& threadName);

    ConnectionThread(
        /* [in] */ const String& threadName);

    ConnectionThread(
        /* [in] */ IThreadGroup* group,
        /* [in] */ IRunnable* runnable);

    ConnectionThread(
        /* [in] */ IThreadGroup* group,
        /* [in] */ IRunnable* runnable,
        /* [in] */ const String& threadName);

    ConnectionThread(
        /* [in] */ IThreadGroup* group,
        /* [in] */ const String& threadName);

    ConnectionThread(
        /* [in] */ IThreadGroup* group,
        /* [in] */ IRunnable* runnable,
        /* [in] */ const String& threadName,
        /* [in] */ Int64 stackSize);


    CARAPI RequestStop();

    /**
     * Loop until app shutdown. Runs connections in priority
     * order.
     */
    virtual CARAPI Run();

    CARAPI ToString(
        /* [out] */ String* str);

public:
    static const Int32 WAIT_TIMEOUT;

    static const Int32 WAIT_TICK;

    // Performance probe
    Int64 mCurrentThreadTime;

    Int64 mTotalThreadTime;

    Connection* mConnection;

private:
    Boolean mWaiting;

    /*volatile*/ Boolean mRunning;

    AutoPtr<IContext> mContext;

    RequestQueue::ConnectionManager* mConnectionManager;

    RequestFeeder* mRequestFeeder;

    Int32 mId;
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_CONNECTIONTHREAD_H__
