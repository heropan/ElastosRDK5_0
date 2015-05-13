
#include "content/AsyncTaskLoader.h"

namespace Elastos {
namespace Droid {
namespace Content {

const CString AsyncTaskLoader::TAG = "AsyncTaskLoader";
const Boolean AsyncTaskLoader::DEBUG = FALSE;

AsyncTaskLoader::LoadTask::LoadTask(
    /* [in] */ AsyncTaskLoader* context)
    : mDone(NULL)
    , waiting(FALSE)
    , mContext(context)
{
//***    CCountDownLatch::New((ICountDownLatch**)&mDone);
}

AsyncTaskLoader::LoadTask::~LoadTask()
{}

CAR_INTERFACE_IMPL_2(AsyncTaskLoader::LoadTask, IAsyncTask, IRunnable)

ECode AsyncTaskLoader::LoadTask::Run()
{
    waiting = FALSE;
    return mContext->ExecutePendingTask();
}

ECode AsyncTaskLoader::LoadTask::WaitForLoader()
{
    return mDone->Await();
}

ECode AsyncTaskLoader::LoadTask::DoInBackground(
    /* [in] */ ArrayOf<IInterface*>* params,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result)
    if (AsyncTaskLoader::DEBUG) {
//        Slog.v(TAG, this + " >>> doInBackground");
    }
//    try {
    FAIL_RETURN(mContext->OnLoadInBackground(result));
    if (AsyncTaskLoader::DEBUG) {
//        Slog.v(TAG, this + "  <<< doInBackground");
    }
    return NOERROR;
//    } catch (OperationCanceledException ex) {
//        if (!isCancelled()) {
            // onLoadInBackground threw a canceled exception spuriously.
            // This is problematic because it means that the LoaderManager did not
            // cancel the Loader itself and still expects to receive a result.
            // Additionally, the Loader's own state will not have been updated to
            // reflect the fact that the task was being canceled.
            // So we treat this case as an unhandled exception.
//            throw ex;
//        }
//        if (AsyncTaskLoader::DEBUG) Slog.v(TAG, this + "  <<< doInBackground (was canceled)");
//        return null;
//    }
}

ECode AsyncTaskLoader::LoadTask::OnPostExecute(
    /* [in] */ IInterface* data)
{
    if (AsyncTaskLoader::DEBUG) {
//        Slog.v(TAG, this + " onPostExecute");
    }
    ECode ec = mContext->DispatchOnLoadComplete(this, data);
    FAIL_RETURN(mDone->CountDown());
    return ec;
}

ECode AsyncTaskLoader::LoadTask::OnCancelled(
    /* [in] */ IInterface* data)
{
    if (AsyncTaskLoader::DEBUG) {
//        Slog.v(TAG, this + " onCancelled");
    }
    ECode ec = mContext->DispatchOnCancelled(this, data);
    FAIL_RETURN(mDone->CountDown());
    return ec;
}

AsyncTaskLoader::AsyncTaskLoader()
    : mTask(NULL)
    , mCancellingTask(NULL)
    , mUpdateThrottle(0)
    , mLastLoadCompleteTime(-10000)
    , mHandler(NULL)
{}

AsyncTaskLoader::AsyncTaskLoader(
    /* [in] */ IContext* context)
    : mTask(NULL)
    , mCancellingTask(NULL)
    , mUpdateThrottle(0)
    , mLastLoadCompleteTime(-10000)
    , mHandler(NULL)
{
    Loader::Init(context);
}

AsyncTaskLoader::~AsyncTaskLoader()
{}

CAR_INTERFACE_IMPL(AsyncTaskLoader, IAsyncTaskLoader)

ECode AsyncTaskLoader::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode AsyncTaskLoader::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode AsyncTaskLoader::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AsyncTaskLoader::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    IAsyncTaskLoader * o = IAsyncTaskLoader::Probe(other);
    if (o != NULL) {
        *result = (o == THIS_PROBE(IAsyncTaskLoader));
    }
    return NOERROR;
}

ECode AsyncTaskLoader::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (Int32)THIS_PROBE(IAsyncTaskLoader);
    return NOERROR;
}

ECode AsyncTaskLoader::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    StringBuilder sb("AsyncTaskLoader:(");
    sb += (Int32)THIS_PROBE(IAsyncTaskLoader);
    sb += ")";
    sb.ToString(info);
    return NOERROR;
}

ECode AsyncTaskLoader::SetUpdateThrottle(
    /* [in] */ Int64 delayMS)
{
    mUpdateThrottle = delayMS;
    if (delayMS != 0) {
//        mHandler = new Handler();
    }
    return NOERROR;
}

ECode AsyncTaskLoader::OnCanceled(
    /* [in] */ IInterface* data)
{
    return NOERROR;
}

ECode AsyncTaskLoader::CancelLoadInBackground()
{
    return NOERROR;
}

ECode AsyncTaskLoader::IsLoadInBackgroundCanceled(
    /* [out] */ Boolean* isCanceled)
{
    VALIDATE_NOT_NULL(isCanceled)
    *isCanceled = mCancellingTask != NULL;
    return NOERROR;
}

ECode AsyncTaskLoader::WaitForLoader()
{
    AutoPtr<AsyncTaskLoader::LoadTask> task = mTask;
    if (NULL != task) {
        FAIL_RETURN(task->WaitForLoader());
    }

    return NOERROR;
}

ECode AsyncTaskLoader::Dump(
    /* [in] */ const String& prefix,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ ArrayOf<String>* args)
{
    FAIL_RETURN(Loader::Dump(prefix, fd, writer, args));

    if (NULL != mTask) {
        writer->PrintString(prefix);
        writer->PrintString(String("mTask="));
        writer->PrintObject(mTask);
        writer->PrintString(String(" waiting="));
        writer->PrintBoolean(mTask->waiting);
    }
    if (NULL != mCancellingTask) {
        writer->PrintString(prefix);
        writer->PrintString(String("mCancellingTask="));
        writer->PrintObject(mCancellingTask);
        writer->PrintString(String(" waiting="));
        writer->PrintBoolean(mCancellingTask->waiting);
    }
    if (mUpdateThrottle != 0) {
        writer->PrintString(prefix);
        writer->PrintString(String("mUpdateThrottle="));
//        TimeUtils.formatDuration(mUpdateThrottle, writer);
        writer->PrintString(String(" mLastLoadCompleteTime="));
//        TimeUtils.formatDuration(mLastLoadCompleteTime,
//        SystemClock.uptimeMillis(), writer);
        writer->Println();
    }

    return NOERROR;
}

ECode AsyncTaskLoader::OnForceLoad()
{
    FAIL_RETURN(Loader::OnForceLoad());
    Boolean ret = FALSE;
    FAIL_RETURN(Loader::CancelLoad(&ret));
    mTask = new LoadTask(this);
    if (DEBUG) {
//        Slog.v(TAG, "Preparing load: mTask=" + mTask);
    }
    FAIL_RETURN(ExecutePendingTask());
    return NOERROR;
}

ECode AsyncTaskLoader::OnCancelLoad(
    /* [out] */ Boolean* isCanceled)
{
    VALIDATE_NOT_NULL(isCanceled)
    if (DEBUG) {
//        Slog.v(TAG, "onCancelLoad: mTask=" + mTask);
    }

    if (NULL != mTask) {
        if (NULL != mCancellingTask) {
            // There was a pending task already waiting for a previous
            // one being canceled; just drop it.
            if (DEBUG) {
//                Slog.v(TAG, "cancelLoad: still waiting for cancelled task; dropping next");
            }
            if (mTask->waiting) {
                mTask->waiting = FALSE;
//                mHandler.removeCallbacks(mTask);
            }
            mTask = NULL;
            *isCanceled = FALSE;
            return NOERROR;
        }
        else if (mTask->waiting) {
            // There is a task, but it is waiting for the time it should
            // execute.  We can just toss it.
            if (DEBUG) {
//                Slog.v(TAG, "cancelLoad: task is waiting, dropping it");
            }
            mTask->waiting = FALSE;
//            mHandler.removeCallbacks(mTask);
            mTask = NULL;
            *isCanceled = FALSE;
            return NOERROR;
        }
        else {
//***            FAIL_RETURN(mTask->Cancel(FALSE, isCanceled));
            if (DEBUG) {
//                Slog.v(TAG, "cancelLoad: cancelled=" + cancelled);
            }
            if (*isCanceled) {
                mCancellingTask = mTask;
                FAIL_RETURN(CancelLoadInBackground());
            }
            mTask = NULL;
            return NOERROR;
        }
    }

    *isCanceled = FALSE;
    return NOERROR;
}

ECode AsyncTaskLoader::OnLoadInBackground(
    /* [out] */ IInterface** result)
{
    return LoadInBackground(result);
}

ECode AsyncTaskLoader::ExecutePendingTask()
{
    if (NULL == mCancellingTask && NULL != mTask) {
        if (mTask->waiting) {
            mTask->waiting = FALSE;
//            mHandler.removeCallbacks(mTask);
        }
        if (mUpdateThrottle > 0) {
            Int64 now = 0;
//            now = SystemClock.uptimeMillis();
            if (now < (mLastLoadCompleteTime+mUpdateThrottle)) {
                // Not yet time to do another load.
                if (DEBUG) {
//                    Slog.v(TAG, "Waiting until " + (mLastLoadCompleteTime+mUpdateThrottle)
//                        + " to execute: " + mTask);
                }
                mTask->waiting = TRUE;
//                mHandler.postAtTime(mTask, mLastLoadCompleteTime+mUpdateThrottle);
                return NOERROR;
            }
        }
        if (DEBUG) {
//            Slog.v(TAG, "Executing: " + mTask);
        }
//        mTask->ExecuteOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR, NULL);
    }

    return NOERROR;
}

ECode AsyncTaskLoader::DispatchOnCancelled(
    /* [in] */ LoadTask* task,
    /* [in] */ IInterface* data)
{
    FAIL_RETURN(OnCanceled(data));
    if (_CObject_Compare(mCancellingTask, task)) {
        if (DEBUG) {
//            Slog.v(TAG, "Cancelled task is now canceled!");
        }
//        mLastLoadCompleteTime = SystemClock.uptimeMillis();
        mCancellingTask = NULL;
        if (DEBUG) {
//            Slog.v(TAG, "Delivering cancellation");
        }
        FAIL_RETURN(Loader::DeliverCancellation());
        FAIL_RETURN(ExecutePendingTask());
    }

    return NOERROR;
}

ECode AsyncTaskLoader::DispatchOnLoadComplete(
    /* [in] */ LoadTask* task,
    /* [in] */ IInterface* data)
{
    if (!_CObject_Compare(mTask, task)) {
        if (DEBUG) {
//            Slog.v(TAG, "Load complete of old task, trying to cancel");
        }
        FAIL_RETURN(DispatchOnCancelled(task, data));
    }
    else {
        Boolean ret = FALSE;
        if ((Loader::IsAbandoned(&ret), ret)) {
            // This cursor has been abandoned; just cancel the new data.
            FAIL_RETURN(OnCanceled(data));
        }
        else {
//            mLastLoadCompleteTime = SystemClock.uptimeMillis();
            mTask = NULL;
            if (DEBUG) {
//                Slog.v(TAG, "Delivering result");
            }
            FAIL_RETURN(Loader::DeliverResult(data));
        }
    }

    return NOERROR;
}

}
}
}
