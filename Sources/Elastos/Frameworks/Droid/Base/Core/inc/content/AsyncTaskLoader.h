
#ifndef __ELASTOS_DROID_CONTENT_ASYNCTASKLOADER_H__
#define __ELASTOS_DROID_CONTENT_ASYNCTASKLOADER_H__

#include <ext/frameworkext.h>
#include <elastos/core/StringBuilder.h>
#include "Loader.h"
#include "os/AsyncTask.h"

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::IO;
using namespace Elastos::Droid::Database;
using namespace Elastos::Droid::Os;
using namespace Elastos::Droid::Utility;

namespace Elastos {
namespace Droid {
namespace Content {

class AsyncTaskLoader
    : public ElRefBase
    , public IObject
    , public IAsyncTaskLoader
    , public Loader
{
private:
    class LoadTask
        : public AsyncTask
        , public IRunnable
    {
    friend class AsyncTaskLoader;

    public:
        LoadTask(
            /* [in] */ AsyncTaskLoader* context);

        ~LoadTask();

        CAR_INTERFACE_DECL()

        /* Runs on the UI thread, when the waiting task is posted to a handler.
         * This method is only executed when task execution was deferred (waiting was true). */
        CARAPI Run();

        /* Used for testing purposes to wait for the task to complete. */
        CARAPI WaitForLoader();

    protected:
        /* Runs on a worker thread */
        CARAPI DoInBackground(
            /* [in] */ ArrayOf<IInterface*>* params,
            /* [out] */ IInterface** result);

        /* Runs on the UI thread */
        CARAPI OnPostExecute(
            /* [in] */ IInterface* data);

        /* Runs on the UI thread */
        CARAPI OnCancelled(
            /* [in] */ IInterface* data);

    private:
        AutoPtr<ICountDownLatch> mDone;
        // Set to true to indicate that the task has been posted to a handler for
        // execution at a later time.  Used to throttle updates.
        Boolean waiting;
        AutoPtr<AsyncTaskLoader> mContext;

    };

public:
    AsyncTaskLoader();

    AsyncTaskLoader(
        /* [in] */ IContext* context);

    virtual ~AsyncTaskLoader();

    CAR_INTERFACE_DECL()

    CARAPI Aggregate(
        /* [in] */ AggrType aggrType,
        /* [in] */ PInterface pObject);

    CARAPI GetDomain(
        /* [out] */ PInterface *ppObject);

    CARAPI GetClassID(
        /* [out] */ ClassID *pCLSID);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean * result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

    CARAPI ToString(
        /* [out] */ String* info);

    /**
     * Set amount to throttle updates by.  This is the minimum time from
     * when the last {@link #loadInBackground()} call has completed until
     * a new load is scheduled.
     *
     * @param delayMS Amount of delay, in milliseconds.
     */
    CARAPI SetUpdateThrottle(
        /* [in] */ Int64 delayMS);

    /**
     * Called if the task was canceled before it was completed.  Gives the class a chance
     * to clean up post-cancellation and to properly dispose of the result.
     *
     * @param data The value that was returned by {@link #loadInBackground}, or null
     * if the task threw {@link OperationCanceledException}.
     */
    CARAPI OnCanceled(
        /* [in] */ IInterface* data);

    /**
     * Called on a worker thread to perform the actual load and to return
     * the result of the load operation.
     *
     * Implementations should not deliver the result directly, but should return them
     * from this method, which will eventually end up calling {@link #deliverResult} on
     * the UI thread.  If implementations need to process the results on the UI thread
     * they may override {@link #deliverResult} and do so there.
     *
     * To support cancellation, this method should periodically check the value of
     * {@link #isLoadInBackgroundCanceled} and terminate when it returns true.
     * Subclasses may also override {@link #cancelLoadInBackground} to interrupt the load
     * directly instead of polling {@link #isLoadInBackgroundCanceled}.
     *
     * When the load is canceled, this method may either return normally or throw
     * {@link OperationCanceledException}.  In either case, the {@link Loader} will
     * call {@link #onCanceled} to perform post-cancellation cleanup and to dispose of the
     * result object, if any.
     *
     * @return The result of the load operation.
     *
     * @throws OperationCanceledException if the load is canceled during execution.
     *
     * @see #isLoadInBackgroundCanceled
     * @see #cancelLoadInBackground
     * @see #onCanceled
     */
    virtual CARAPI LoadInBackground(
        /* [out] */ IInterface** result) = 0;

    /**
     * Called on the main thread to abort a load in progress.
     *
     * Override this method to abort the current invocation of {@link #loadInBackground}
     * that is running in the background on a worker thread.
     *
     * This method should do nothing if {@link #loadInBackground} has not started
     * running or if it has already finished.
     *
     * @see #loadInBackground
     */
    CARAPI CancelLoadInBackground();

    /**
     * Returns true if the current invocation of {@link #loadInBackground} is being canceled.
     *
     * @return True if the current invocation of {@link #loadInBackground} is being canceled.
     *
     * @see #loadInBackground
     */
    CARAPI IsLoadInBackgroundCanceled(
        /* [out] */ Boolean* isCanceled);

    /**
     * Locks the current thread until the loader completes the current load
     * operation. Returns immediately if there is no load operation running.
     * Should not be called from the UI thread: calling it from the UI
     * thread would cause a deadlock.
     * <p>
     * Use for testing only.  <b>Never</b> call this from a UI thread.
     *
     * @hide
     */
    CARAPI WaitForLoader();

    CARAPI Dump(
        /* [in] */ const String& prefix,
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* writer,
        /* [in] */ ArrayOf<String>* args);

protected:
    CARAPI OnForceLoad();

    CARAPI OnCancelLoad(
        /* [out] */ Boolean* isCanceled);

    /**
     * Calls {@link #loadInBackground()}.
     *
     * This method is reserved for use by the loader framework.
     * Subclasses should override {@link #loadInBackground} instead of this method.
     *
     * @return The result of the load operation.
     *
     * @throws OperationCanceledException if the load is canceled during execution.
     *
     * @see #loadInBackground
     */
    CARAPI OnLoadInBackground(
        /* [out] */ IInterface** result);

private:
    CARAPI ExecutePendingTask();

    CARAPI DispatchOnCancelled(
        /* [in] */ LoadTask* task,
        /* [in] */ IInterface* data);

    CARAPI DispatchOnLoadComplete(
        /* [in] */ LoadTask* task,
        /* [in] */ IInterface* data);

private:
    static const CString TAG;
    static const Boolean DEBUG;

private:
    /* volatile */ AutoPtr<LoadTask> mTask;
    /* volatile */ AutoPtr<LoadTask> mCancellingTask;

    Int64 mUpdateThrottle;
    Int64 mLastLoadCompleteTime;
    AutoPtr<IHandler> mHandler;

};

}
}
}

#endif //__ELASTOS_DROID_CONTENT_ASYNCTASKLOADER_H__
