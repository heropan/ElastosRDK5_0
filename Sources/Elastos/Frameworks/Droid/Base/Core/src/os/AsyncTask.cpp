
#include "os/AsyncTask.h"
#ifdef DROID_CORE
#include "os/CLooperHelper.h"
#endif
#include <os/Process.h>
#include <elastos/StringUtils.h>

using Elastos::Core::EIID_IRunnable;
using Elastos::Core::StringUtils;
using Elastos::Core::Threading::CThread;
using Elastos::Utility::Concurrent::CLinkedBlockingQueue;
using Elastos::Utility::Concurrent::EIID_IThreadFactory;
using Elastos::Utility::Concurrent::EIID_IExecutor;
using Elastos::Utility::Concurrent::EIID_ICallable;
using Elastos::Utility::Concurrent::EIID_IRunnableFuture;
using Elastos::Utility::Concurrent::IThreadPoolExecutor;
using Elastos::Utility::Concurrent::CThreadPoolExecutor;
using Elastos::Utility::Concurrent::ITimeUnitHelper;
using Elastos::Utility::Concurrent::CTimeUnitHelper;
using Elastos::Utility::Concurrent::Atomic::CAtomicInteger32;
using Elastos::Utility::Concurrent::Atomic::CAtomicBoolean;
using Elastos::Droid::Os::ILooperHelper;
using Elastos::Droid::Os::CLooperHelper;

namespace Elastos {
namespace Droid {
namespace Os {

static AutoPtr<IBlockingQueue> InitPoolWorkQueue()
{
    AutoPtr<IBlockingQueue> bq;
    ASSERT_SUCCEEDED(CLinkedBlockingQueue::New(10, (IBlockingQueue**)&bq));
    return bq;
}

const String AsyncTask::TAG("AsyncTask");
const Int32 AsyncTask::CORE_POOL_SIZE;
const Int32 AsyncTask::MAXIMUM_POOL_SIZE;
const Int32 AsyncTask::KEEP_ALIVE;
const AutoPtr<IThreadFactory> AsyncTask::sThreadFactory = new AsyncTask::MyThreadFactory();
const AutoPtr<IBlockingQueue> AsyncTask::sPoolWorkQueue = InitPoolWorkQueue();
const AutoPtr<IExecutor> AsyncTask::THREAD_POOL_EXECUTOR = InitThreadPoolExecutor();

const AutoPtr<IExecutor> AsyncTask::SERIAL_EXECUTOR = new AsyncTask::SerialExecutor();
const Int32 AsyncTask::MESSAGE_POST_RESULT;
const Int32 AsyncTask::MESSAGE_POST_PROGRESS;
AutoPtr<IHandler> AsyncTask::sHandler;
/* volatile */ AutoPtr<IExecutor> AsyncTask::sDefaultExecutor = SERIAL_EXECUTOR;

CAR_INTERFACE_IMPL(AsyncTask::AsyncTaskResult, IInterface)

//====================================================================
// AsyncTask::InternalHandler
//====================================================================
ECode AsyncTask::InternalHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);
    AsyncTask::AsyncTaskResult* result = (AsyncTask::AsyncTaskResult*)obj.Get();

    switch(what) {
        case AsyncTask::MESSAGE_POST_RESULT:
            // There is only one result
            result->mTask->Finish((*result->mData)[0]);
            break;
        case AsyncTask::MESSAGE_POST_PROGRESS:
            result->mTask->OnProgressUpdate(result->mData);
            break;
    }

    return NOERROR;
}

//====================================================================
// AsyncTask::MyThreadFactory
//====================================================================
AsyncTask::MyThreadFactory::MyThreadFactory()
{
    CAtomicInteger32::New(1, (IAtomicInteger32**)&mCount);
}

CAR_INTERFACE_IMPL_LIGHT(AsyncTask::MyThreadFactory, IThreadFactory);

ECode AsyncTask::MyThreadFactory::NewThread(
    /* [in] */ IRunnable* r,
    /* [out] */ IThread** t)
{
    Int32 value;
    mCount->GetAndIncrement(&value);
    return CThread::New(r, String("AsyncTask #") + StringUtils::Int32ToString(value), t);
}


//====================================================================
// AsyncTask::SerialExecutor::MyRunnable
//====================================================================
CAR_INTERFACE_IMPL_LIGHT(AsyncTask::SerialExecutor::MyRunnable, IRunnable)

ECode AsyncTask::SerialExecutor::MyRunnable::Run()
{
    // try {
    mR->Run();
    // } finally {
    mOwner->ScheduleNext();
    // }
    return NOERROR;
}


//====================================================================
// AsyncTask::SerialExecutor
//====================================================================
CAR_INTERFACE_IMPL_LIGHT(AsyncTask::SerialExecutor, IExecutor)

ECode AsyncTask::SerialExecutor::Execute(
    /* [in] */ IRunnable* r)
{
    AutoPtr<IRunnable> runnable = new MyRunnable(this, r);
    mTasks.PushBack(runnable);
    if (mActive == NULL) {
        ScheduleNext();
    }
    return NOERROR;
}

ECode AsyncTask::SerialExecutor::ScheduleNext()
{
    if (mTasks.GetSize() > 0) {
        mActive = mTasks.GetFront();
        mTasks.PopFront();
    }
    else
        mActive = NULL;

    if (mActive != NULL) {
        THREAD_POOL_EXECUTOR->Execute(mActive);
    }
    return NOERROR;
}


//====================================================================
// AsyncTask::WorkerRunnable
//====================================================================
CAR_INTERFACE_IMPL_LIGHT(AsyncTask::WorkerRunnable, ICallable)

ECode AsyncTask::WorkerRunnable::Call(
    /* [out] */ IInterface** result)
{
    mOwner->mTaskInvoked->Set(TRUE);

    Process::SetThreadPriority(IProcess::THREAD_PRIORITY_BACKGROUND);
    //noinspection unchecked
    AutoPtr<IInterface> r = mOwner->DoInBackground(mParams);
    AutoPtr<IInterface> _r = mOwner->PostResult(r);
    *result = _r;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}


//====================================================================
// AsyncTask::MyFutureTask
//====================================================================
CAR_INTERFACE_IMPL_LIGHT_2(AsyncTask::MyFutureTask, IRunnableFuture, IRunnable)

void AsyncTask::MyFutureTask::Done()
{
    // try {
    AutoPtr<IInterface> object;
    Get((IInterface**)&object);
    mOwner->PostResultIfNotInvoked(object);
    // } catch (InterruptedException e) {
    //     android.util.Log.w(TAG, e);
    // } catch (ExecutionException e) {
    //     throw new RuntimeException("An error occured while executing doInBackground()",
    //             e.getCause());
    // } catch (CancellationException e) {
    //     postResultIfNotInvoked(null);
    // }
}

ECode AsyncTask::MyFutureTask::Run()
{
    return FutureTask::Run();
}

ECode AsyncTask::MyFutureTask::Cancel(
    /* [in] */ Boolean mayInterruptIfRunning,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::Cancel(mayInterruptIfRunning, result);
}

ECode AsyncTask::MyFutureTask::IsCancelled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::IsCancelled(result);
}

ECode AsyncTask::MyFutureTask::IsDone(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::IsDone(result);
}

ECode AsyncTask::MyFutureTask::Get(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::Get(result);
}

ECode AsyncTask::MyFutureTask::GetEx(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::GetEx(timeout, unit, result);
}


//====================================================================
// AsyncTask
//====================================================================
AsyncTask::AsyncTask()
    : mStatus(PENDING)
{
    CAtomicBoolean::New((IAtomicBoolean**)&mCancelled);
    CAtomicBoolean::New((IAtomicBoolean**)&mTaskInvoked);
    mWorker = new WorkerRunnable(this);
    mFuture = new MyFutureTask(mWorker, this);
}

ECode AsyncTask::Init()
{
    AutoPtr<ILooper> looper;
    return GetHandler()->GetLooper((ILooper**)&looper);
}

AutoPtr<IHandler> AsyncTask::GetHandler()
{
    if (sHandler == NULL) {
        AutoPtr<ILooperHelper> helper;
        CLooperHelper::AcquireSingleton((ILooperHelper**)&helper);
        helper->Prepare();
        AutoPtr<ILooper> myLooper;
        helper->MyLooper((ILooper**)&myLooper);
        assert(myLooper != NULL && "Failed to get myLooper!");

        sHandler = new InternalHandler(myLooper); // TODO luo.zhaohui
    }

    return sHandler;
}

/** @hide */
void AsyncTask::SetDefaultExecutor(
    /* [in] */ IExecutor* exec)
{
    sDefaultExecutor = exec;
}

void AsyncTask::PostResultIfNotInvoked(
    /* [in] */ IInterface* result)
{
    Boolean wasTaskInvoked;
    mTaskInvoked->Get(&wasTaskInvoked);
    if (!wasTaskInvoked) {
        PostResult(result);
    }
}

AutoPtr<IInterface> AsyncTask::PostResult(
    /* [in] */ IInterface* result)
{
    AutoPtr< ArrayOf<IInterface*> > data = ArrayOf<IInterface*>::Alloc(1);
    data->Set(0, result);
    AutoPtr<AsyncTaskResult> atResult = new AsyncTaskResult(this, data);

    AutoPtr<IMessage> msg;
    GetHandler()->ObtainMessageEx(MESSAGE_POST_RESULT, atResult, (IMessage**)&msg);
    msg->SendToTarget();

    return result;
}

AsyncTask::Status AsyncTask::GetStatus()
{
    return mStatus;
}

Boolean AsyncTask::IsCancelled()
{
    Boolean value;
    mCancelled->Get(&value);
    return value;
}

Boolean AsyncTask::Cancel(
    /* [in] */ Boolean mayInterruptIfRunning)
{
    mCancelled->Set(TRUE);
    Boolean result;
    mFuture->Cancel(mayInterruptIfRunning, &result);
    return result;
}

ECode AsyncTask::Get(
    /* [out] */ IInterface** result)
{
    return mFuture->Get(result);
}

ECode AsyncTask::Get(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IInterface** result)
{
    return mFuture->GetEx(timeout, unit, result);
}

ECode AsyncTask::Execute(
    /* [in] */ ArrayOf<IInterface*>* params)
{
    return ExecuteOnExecutor(sDefaultExecutor, params);
}

ECode AsyncTask::ExecuteOnExecutor(
    /* [in] */ IExecutor* exec,
    /* [in] */ ArrayOf<IInterface*>* params)
{
    if (mStatus != PENDING) {
        switch (mStatus) {
            case RUNNING:
                // throw new IllegalStateException("Cannot execute task:"
                //         + " the task is already running.");
                return E_ILLEGAL_STATE_EXCEPTION;
            case FINISHED:
                // throw new IllegalStateException("Cannot execute task:"
                //         + " the task has already been executed "
                //         + "(a task can be executed only once)");
                return E_ILLEGAL_STATE_EXCEPTION;
        }
    }

    mStatus = RUNNING;

    OnPreExecute();

    mWorker->mParams = params;
    exec->Execute(IRunnable::Probe(mFuture));

    return NOERROR;
}

ECode AsyncTask::Execute(
    /* [in] */ IRunnable* runnable)
{
    return sDefaultExecutor->Execute(runnable);
}

void AsyncTask::PublishProgress(
    /* [in] */ ArrayOf<IInterface*>* values)
{
    if (!IsCancelled()) {

        AutoPtr<AsyncTaskResult> atResult = new AsyncTaskResult(this, values);

        AutoPtr<IMessage> msg;
        GetHandler()->ObtainMessageEx(MESSAGE_POST_PROGRESS, atResult, (IMessage**)&msg);
        msg->SendToTarget();
    }
}

void AsyncTask::Finish(
    /* [in] */ IInterface* result)
{
    if (IsCancelled()) {
        OnCancelled(result);
    }
    else {
        OnPostExecute(result);
    }
    mStatus = FINISHED;
}

AutoPtr<IExecutor> AsyncTask::InitThreadPoolExecutor()
{
    assert(sPoolWorkQueue != NULL);
    assert(sThreadFactory != NULL);
    AutoPtr<ITimeUnitHelper> tuHelper;
    CTimeUnitHelper::AcquireSingleton((ITimeUnitHelper**)&tuHelper);
    AutoPtr<ITimeUnit> seconds;
    tuHelper->GetSECONDS((ITimeUnit**)&seconds);
    AutoPtr<IThreadPoolExecutor> executor;
    CThreadPoolExecutor::New(CORE_POOL_SIZE, MAXIMUM_POOL_SIZE, KEEP_ALIVE,
        seconds, sPoolWorkQueue, sThreadFactory, (IThreadPoolExecutor**)&executor);
    return IExecutor::Probe(executor);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
