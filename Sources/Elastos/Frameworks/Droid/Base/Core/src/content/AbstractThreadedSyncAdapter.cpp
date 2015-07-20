
#include "content/AbstractThreadedSyncAdapter.h"
//***#include "content/CSyncContext.h"
//***#include "content/CSyncResult.h"
#include "os/CBundle.h"
#include <elaatomics.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::ITrace;

namespace Elastos {
namespace Droid {
namespace Content {

#define FAIL_WithGoto(ec)           \
            do {                    \
                if (FAILED(ec)) {   \
                    goto EXIT;      \
                }                   \
            } while(0);

AbstractThreadedSyncAdapter::ISyncAdapterImpl::ISyncAdapterImpl(
    /* [in] */ AbstractThreadedSyncAdapter* context)
    : mAdapterContext(context)
{}

AbstractThreadedSyncAdapter::ISyncAdapterImpl::~ISyncAdapterImpl()
{}

CAR_INTERFACE_IMPL(AbstractThreadedSyncAdapter::ISyncAdapterImpl, IISyncAdapter)

ECode AbstractThreadedSyncAdapter::ISyncAdapterImpl::StartSync(
    /* [in] */ IISyncContext* syncContext,
    /* [in] */ const String& authority,
    /* [in] */ IAccount* account,
    /* [in] */ IBundle* extras)
{
    AutoPtr<ISyncContext> syncContextClient;
//***    FAIL_RETURN(CSyncContext::New(syncContext, (ISyncContext**)&syncContextClient))
    Boolean alreadyInProgress = FALSE;
    // synchronize to make sure that mSyncThreads doesn't change between when we
    // check it and when we use it
    AutoPtr<IAccount> threadsKey = mAdapterContext->ToSyncKey(account);
    {
        AutoLock lock(mAdapterContext->mSyncThreadLock);
        HashMap<AutoPtr<IAccount>, AutoPtr<SyncThread> >::Iterator it = mAdapterContext->mSyncThreads.Find(threadsKey);

        if (it == mAdapterContext->mSyncThreads.End()) {
            Boolean ret = FALSE;
            if (mAdapterContext->mAutoInitialize && NULL != extras
                    && (extras->GetBoolean(IContentResolver::SYNC_EXTRAS_INITIALIZE, FALSE, &ret), ret)) {
                AutoPtr<IContentResolverHelper> resolverHelper;
                Int32 result = 0;
//***                FAIL_RETURN(ContentResolverHelper::AcquireSingleton((IContentResolverHelper**)&resolverHelper))
                FAIL_RETURN(resolverHelper->GetIsSyncable(account, authority, &result))
                if (result < 0) {
                    FAIL_RETURN(resolverHelper->SetIsSyncable(account, authority, 1))
                }
                AutoPtr<ISyncResult> syncResult;
//***                FAIL_RETURN(CSyncResult::New((ISyncResult**)&syncResult))
                FAIL_RETURN(syncContextClient->OnFinished(syncResult))
                return NOERROR;
            }

            AutoPtr<SyncThread> syncThread = new AbstractThreadedSyncAdapter::SyncThread(
                    String("SyncAdapterThread-") + StringUtils::Int32ToString(atomic_inc(&mAdapterContext->mNumSyncStarts)),
                    syncContextClient, authority, account, extras, mAdapterContext);
            mAdapterContext->mSyncThreads[threadsKey] = syncThread;
            FAIL_RETURN(syncThread->Start())
            alreadyInProgress = FALSE;
        }
        else {
            alreadyInProgress = TRUE;
        }
    }

    // do this outside since we don't want to call back into the syncContext while
    // holding the synchronization lock
    if (alreadyInProgress) {
        AutoPtr<ISyncResultHelper> resultHelper;
        AutoPtr<ISyncResult> newSyncResult;
//***        FAIL_RETURN(CSyncResultHelper::AcquireSingleton((ISyncResultHelper**)&resultHelper))
        FAIL_RETURN(resultHelper->GetALREADY_IN_PROGRESS((ISyncResult**)&newSyncResult))
        FAIL_RETURN(syncContextClient->OnFinished(newSyncResult))
    }
    return NOERROR;
}

ECode AbstractThreadedSyncAdapter::ISyncAdapterImpl::CancelSync(
    /* [in] */ IISyncContext* syncContext)
{
    // synchronize to make sure that mSyncThreads doesn't change between when we
    // check it and when we use it
    AutoPtr<SyncThread> info = NULL;
    {
        AutoLock lock(mAdapterContext->mSyncThreadLock);
        HashMap<AutoPtr<IAccount>, AutoPtr<SyncThread> >::Iterator it = mAdapterContext->mSyncThreads.Begin();
        AutoPtr<SyncThread> current;
        for (; it != mAdapterContext->mSyncThreads.End(); it++) {
            current = it->mSecond;
            if (_CObject_Compare(current->mSyncContext, syncContext)) {
                info = current;
                break;
            }
        }
    }

    if (NULL != info) {
        if (mAdapterContext->mAllowParallelSyncs) {
            FAIL_RETURN(mAdapterContext->OnSyncCanceled(info))
        }
        else {
            FAIL_RETURN(mAdapterContext->OnSyncCanceled())
        }
    }

    return NOERROR;
}

ECode AbstractThreadedSyncAdapter::ISyncAdapterImpl::Initialize(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority)
{
    AutoPtr<IBundle> extras;
    FAIL_RETURN(CBundle::New((IBundle**)&extras))
    FAIL_RETURN(extras->PutBoolean(IContentResolver::SYNC_EXTRAS_INITIALIZE, TRUE))
    FAIL_RETURN(StartSync(NULL, authority, account, extras))
    return NOERROR;
}

ECode AbstractThreadedSyncAdapter::ISyncAdapterImpl::ToString(
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}

AbstractThreadedSyncAdapter::SyncThread::SyncThread(
    /* [in] */ const String& name,
    /* [in] */ ISyncContext* syncContext,
    /* [in] */ const String& authority,
    /* [in] */ IAccount* account,
    /* [in] */ IBundle* extras,
    /* [in] */ AbstractThreadedSyncAdapter* context)
    : mSyncContext(NULL)
    , mAccount(NULL)
    , mExtras(NULL)
    , mThreadsKey(NULL)
    , mAdapterContext(context)
{
    ASSERT_SUCCEEDED(Thread::constructor(name))
    mSyncContext = syncContext;
    mAuthority = authority;
    mAccount = account;
    mExtras = extras;
    mThreadsKey = mAdapterContext->ToSyncKey(account);
}

AbstractThreadedSyncAdapter::SyncThread::~SyncThread()
{}

ECode AbstractThreadedSyncAdapter::SyncThread::Run()
{
//    Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);

    // Trace this sync instance.  Note, conceptually this should be in
    // SyncStorageEngine.insertStartSyncEvent(), but the trace functions require unique
    // threads in order to track overlapping operations, so we'll do it here for now.
    AutoPtr<ITrace> trace;
//***    FAIL_RETURN(CTrace::AcquireSingleton((ITrace**)&trace))
    FAIL_RETURN(trace->TraceBegin(ITrace::TRACE_TAG_SYNC_MANAGER, mAuthority))

    AutoPtr<ISyncResult> syncResult;
//***    FAIL_RETURN(CSyncResult::New((ISyncResult**)&syncResult))
    AutoPtr<IContentProviderClient> provider = NULL;
    ECode e = NOERROR;

    {
        Boolean isCanceled = FALSE;
        AutoPtr<IContentResolver> resolver;
        e = IsCanceled(&isCanceled);
        FAIL_WithGoto(e)
        if (isCanceled) {
            return NOERROR;
        }

        e = mAdapterContext->mContext->GetContentResolver((IContentResolver**)&resolver);
        FAIL_WithGoto(e)
        e = resolver->AcquireContentProviderClient(mAuthority, (IContentProviderClient**)&provider);
        FAIL_WithGoto(e)
        if (NULL != provider) {
            e = mAdapterContext->OnPerformSync(mAccount, mExtras,
                    mAuthority, provider, syncResult);
            FAIL_WithGoto(e)
        }
        else {
            e = syncResult->SetDatabaseError(TRUE);
            FAIL_WithGoto(e)
        }
    }
EXIT:
    FAIL_RETURN(trace->TraceEnd(ITrace::TRACE_TAG_SYNC_MANAGER))

    if (NULL != provider) {
        Boolean ret = FALSE;
        FAIL_RETURN(provider->ReleaseProvider(&ret))
    }
    Boolean isCanceled = FALSE;
    FAIL_RETURN(IsCanceled(&isCanceled))
    if (!isCanceled) {
        FAIL_RETURN(mSyncContext->OnFinished(syncResult))
    }
    // synchronize so that the assignment will be seen by other threads
    // that also synchronize accesses to mSyncThreads
    {
        AutoLock lock(mAdapterContext->mSyncThreadLock);
        mAdapterContext->mSyncThreads.Erase(mThreadsKey);
    }
    FAIL_RETURN(e)
    return NOERROR;
}

ECode AbstractThreadedSyncAdapter::SyncThread::IsCanceled(
    /* [out] */ Boolean* isCanceled)
{
    VALIDATE_NOT_NULL(isCanceled)
    return Thread::GetCurrentThread()->IsInterrupted(isCanceled);
}

AbstractThreadedSyncAdapter::AbstractThreadedSyncAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Boolean autoInitialize)
    : mContext(NULL)
    , mNumSyncStarts(0)
    , mISyncAdapterImpl(NULL)
    , mAutoInitialize(FALSE)
    , mAllowParallelSyncs(FALSE)
{
    ASSERT_SUCCEEDED(Init(context, autoInitialize, FALSE))
}

AbstractThreadedSyncAdapter::AbstractThreadedSyncAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Boolean autoInitialize,
    /* [in] */ Boolean allowParallelSyncs)
    : mContext(NULL)
    , mNumSyncStarts(0)
    , mSyncThreads(NULL)
    , mAutoInitialize(FALSE)
    , mAllowParallelSyncs(FALSE)
{
    ASSERT_SUCCEEDED(Init(context, autoInitialize, allowParallelSyncs))
}

AbstractThreadedSyncAdapter::~AbstractThreadedSyncAdapter()
{
    mSyncThreads.Clear();
}

CAR_INTERFACE_IMPL(AbstractThreadedSyncAdapter, IAbstractThreadedSyncAdapter)

ECode AbstractThreadedSyncAdapter::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context)
    *context = mContext;
    REFCOUNT_ADD(*context);
    return NOERROR;
}

ECode AbstractThreadedSyncAdapter::GetSyncAdapterBinder(
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder)
    *binder = IISyncAdapter::Probe((IISyncAdapter*) mISyncAdapterImpl);
    REFCOUNT_ADD(*binder);
    return NOERROR;
}

ECode AbstractThreadedSyncAdapter::OnSyncCanceled()
{
    AutoPtr<SyncThread> syncThread;
    {
        AutoLock lock(mSyncThreadLock);
        HashMap<AutoPtr<IAccount>, AutoPtr<SyncThread> >::Iterator it = mSyncThreads.Find((AutoPtr<IAccount>) NULL);
        syncThread = it->mSecond;
    }
    if (NULL != syncThread) {
        FAIL_RETURN(syncThread->Interrupt())
    }
    return NOERROR;
}

ECode AbstractThreadedSyncAdapter::OnSyncCanceled(
    /* [in] */ IThread* thread)
{
    if (NULL != thread) {
        FAIL_RETURN(thread->Interrupt())
    }
    return NOERROR;
}

AutoPtr<IAccount> AbstractThreadedSyncAdapter::ToSyncKey(
    /* [in] */ IAccount* account)
{
    if (mAllowParallelSyncs) {
        return account;
    }
    else {
        return NULL;
    }
}

ECode AbstractThreadedSyncAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ Boolean autoInitialize,
    /* [in] */ Boolean allowParallelSyncs)
{
    mContext = context;
    mISyncAdapterImpl = new ISyncAdapterImpl(this);
    mNumSyncStarts = 0;
    mAutoInitialize = autoInitialize;
    mAllowParallelSyncs = allowParallelSyncs;
    return NOERROR;
}

}
}
}
