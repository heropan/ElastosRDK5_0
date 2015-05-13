#ifndef __QUEUEDWORK_H__
#define __QUEUEDWORK_H__

#include <Elastos.Core.h>

using Elastos::Utility::Concurrent::IExecutorService;
using Elastos::Core::IRunnable;

namespace Elastos {
namespace Droid {
namespace App {

class QueuedWork
{
public:
    /**
     * Returns a single-thread Executor shared by the entire process,
     * creating it if necessary.
     */
    static CARAPI SingleThreadExecutor(
        /* [out] */ IExecutorService* singleThreadExecutor);

        /**
     * Add a runnable to finish (or wait for) a deferred operation
     * started in this context earlier.  Typically finished by e.g.
     * an Activity#onPause.  Used by SharedPreferences$Editor#startCommit().
     *
     * Note that this doesn't actually start it running.  This is just
     * a scratch set for callers doing async work to keep updated with
     * what's in-flight.  In the common case, caller code
     * (e.g. SharedPreferences) will pretty quickly call remove()
     * after an add().  The only time these Runnables are run is from
     * waitToFinish(), below.
     */
    static CARAPI Add(
        /* [in] */ IRunnable* finisher);

    static CARAPI Remove(
        /* [in] */ IRunnable* finisher);

    /**
     * Finishes or waits for async operations to complete.
     * (e.g. SharedPreferences$Editor#startCommit writes)
     *
     * Is called from the Activity base class's onPause(), after
     * BroadcastReceiver's onReceive, after Service command handling,
     * etc.  (so async work is never lost)
     */
    static CARAPI WaitToFinish();

    /**
     * Returns true if there is pending work to be done.  Note that the
     * result is out of data as soon as you receive it, so be careful how you
     * use it.
     */
    static CARAPI HasPendingWork(
        /* [out] */ Boolean result);

public:
    // The set of Runnables that will finish or wait on any async
    // activities started by the application.
    //static ConcurrentLinkedQueue<Runnable> sPendingWorkFinishers;

    static AutoPtr<IExecutorService> sSingleThreadExecutor; // lazy, guarded by class
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __QUEUEDWORK_H__
