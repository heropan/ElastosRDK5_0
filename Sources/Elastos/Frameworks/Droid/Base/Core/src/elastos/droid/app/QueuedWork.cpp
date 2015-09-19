#include "QueuedWork.h"

namespace Elastos{
namespace Droid{
namespace App{

/**
 * Returns a single-thread Executor shared by the entire process,
 * creating it if necessary.
 */
ECode QueuedWork::SingleThreadExecutor(
    /* [out] */ IExecutorService *singleThreadExecutor)
{
    // synchronized (QueuedWork.class) {
    //     if (sSingleThreadExecutor == null) {
    //         // TODO: can we give this single thread a thread name?
    //         sSingleThreadExecutor = Executors.newSingleThreadExecutor();
    //     }
    //     return sSingleThreadExecutor;
    // }
    return E_NOT_IMPLEMENTED;
}

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
ECode QueuedWork::Add(
    /* [in] */ IRunnable *finisher)
{
    //sPendingWorkFinishers.add(finisher);
    return E_NOT_IMPLEMENTED;
}

ECode QueuedWork::Remove(
    /* [in] */ IRunnable *finisher)
{
    //sPendingWorkFinishers.remove(finisher);
    return E_NOT_IMPLEMENTED;
}

/**
 * Finishes or waits for async operations to complete.
 * (e.g. SharedPreferences$Editor#startCommit writes)
 *
 * Is called from the Activity base class's onPause(), after
 * BroadcastReceiver's onReceive, after Service command handling,
 * etc.  (so async work is never lost)
 */
ECode QueuedWork::WaitToFinish()
{
    // Runnable toFinish;
    // while ((toFinish = sPendingWorkFinishers.poll()) != null) {
    //     toFinish.run();
    // }
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns true if there is pending work to be done.  Note that the
 * result is out of data as soon as you receive it, so be careful how you
 * use it.
 */
ECode QueuedWork::HasPendingWork(
    /* [out] */ Boolean result)
{
    //return !sPendingWorkFinishers.isEmpty();
    return E_NOT_IMPLEMENTED;
}

}
}
}
