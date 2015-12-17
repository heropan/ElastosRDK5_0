#include "elastos/droid/widget/CRemoteViewsAdapterServiceConnection.h"
#include "elastos/droid/widget/RemoteViewsAdapter.h"
#include "elastos/droid/appwidget/CAppWidgetManager.h"
#include "elastos/droid/os/CUserHandle.h"
#include "elastos/droid/os/Process.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Os::CUserHandle;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::AppWidget::CAppWidgetManager;
using Elastos::Droid::AppWidget::IAppWidgetManager;
namespace Elastos {
namespace Droid {
namespace Widget {
/*----------------------------------MyRunnable----------------------------------*/
CRemoteViewsAdapterServiceConnection::MyRunnable::MyRunnable(
    /* [in] */ IRemoteViewsAdapter* adapter) : mAdapter(adapter)
{}

ECode CRemoteViewsAdapterServiceConnection::MyRunnable::Run()
{
    RemoteViewsAdapter* adpImpl = (RemoteViewsAdapter*)mAdapter.Get();
    if (adpImpl->mNotifyDataSetChangedAfterOnServiceConnected) {
        // Handle queued notifyDataSetChanged() if necessary
        adpImpl->OnNotifyDataSetChanged();
    }
    else {
        AutoPtr<IIRemoteViewsFactory> factory;
        adpImpl->mServiceConnection->GetRemoteViewsFactory((IIRemoteViewsFactory**)&factory);
        // try {
            Boolean isCreated = FALSE;
            if (!(factory->IsCreated(&isCreated), isCreated)) {
                // We only call onDataSetChanged() if this is the factory was just
                // create in response to this bind
                FAIL_RETURN(factory->OnDataSetChanged());
            }
        // } catch (RemoteException e) {
        //     Log.e(TAG, "Error notifying factory of data set changed in " +
        //                 "onServiceConnected(): " + e.getMessage());

        //     // Return early to prevent anything further from being notified
        //     // (effectively nothing has changed)
        //     return;
        // } catch (RuntimeException e) {
        //     Log.e(TAG, "Error notifying factory of data set changed in " +
        //             "onServiceConnected(): " + e.getMessage());
        // }
        // Request meta data so that we have up to date data when calling back to
        // the remote adapter callback
        adpImpl->UpdateTemporaryMetaData();
        // Notify the host that we've connected
        AutoPtr<IRunnable> r = new MyRunnable(mAdapter);
        Boolean rst;
        adpImpl->mMainQueue->Post(r, &rst);
    }
    return NOERROR;
}

/*----------------------------------MyRunnableEx----------------------------------*/
CRemoteViewsAdapterServiceConnection::MyRunnableEx::MyRunnable(
    /* [in] */ IRemoteViewsAdapter* adapter) : mAdapter(adapter)
{}

ECode CRemoteViewsAdapterServiceConnection::MyRunnableEx::Run()
{
    RemoteViewsAdapter* adapter = reinterpret_cast<RemoteViewsAdapter*>(mAdapter->Probe(EIID_RemoteViewsAdapter));
    {
        AutoLock lock(adapter->mCacheLock);
        adapter->mCache->CommitTemporaryMetaData();
    }
    AutoPtr<IRemoteAdapterConnectionCallback> callback = adapter->mCallback;
    if (callback) {
        Boolean rst;
        callback->OnRemoteAdapterConnected(&rst);
    }
    return NOERROR;
}

/*----------------------------------MyRunnableEx2----------------------------------*/
CRemoteViewsAdapterServiceConnection::MyRunnableEx2::MyRunnable(
    /* [in] */ IRemoteViewsAdapter* adapter) : mAdapter(adapter)
{}

ECode CRemoteViewsAdapterServiceConnection::MyRunnableEx2::Run()
{
    RemoteViewsAdapter* adapter = reinterpret_cast<RemoteViewsAdapter*>(mAdapter->Probe(EIID_RemoteViewsAdapter));

    adapter->mMainQueue->RemoveMessages(RemoteViewsAdapter::sUnbindServiceMessageType);
    AutoPtr<IRemoteAdapterConnectionCallback> callback = adapter->mCallback;
    if (callback) {
        Boolean rst;
        callback->OnRemoteAdapterConnected(&rst);
    }
    return NOERROR;
}


/*----------------------------------CRemoteViewsAdapterServiceConnection----------------------------------*/
CRemoteViewsAdapterServiceConnection::CRemoteViewsAdapterServiceConnection()
    : mIsConnected(FALSE)
    , mIsConnecting(FALSE)
    , mAdapter(NULL)
{}

ECode CRemoteViewsAdapterServiceConnection::constructor(
    /* [in] */ IRemoteViewsAdapter* adapter)
{
    mAdapter = adapter;
    return NOERROR;
}

ECode CRemoteViewsAdapterServiceConnection::Bind(
    /* [in] */ IContext* ctx,
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IIntent* intent)
{
    AutoLock lock(mLock);
    if (!mIsConnecting)
    {
        AutoPtr<IRemoteViewsAdapter> adapter;
        AutoPtr<IAppWidgetManager> mgr;
        CAppWidgetManager::GetInstance(ctx, (IAppWidgetManager**)&mgr);
        ECode pe;
        if ((adapter = mAdapter) != NULL) {
            String opPkgName;
            ctx->GetOpPackageName(&opPkgName);
            pe = mgr->BindRemoteViewsService(opPkgName, appWidgetId, intent, this);
        }
        else {
            SLOGGERE("RemoteViewsAdapterServiceConnection", "bind: adapter was null");
        }
        if (FAILED(pe)) {
            SLOGGERE("RemoteViewsAdapterServiceConnection", "Bind() : Error system server dead?")
            mIsConnecting = FALSE;
            mIsConnected = FALSE;
            return pe;
        }
        mIsConnecting = TRUE;
    }
    return NOERROR;
}

ECode CRemoteViewsAdapterServiceConnection::Unbind(
    /* [in] */ IContext* ctx,
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IIntent* intent)
{
    AutoLock lock(mLock);
    // try {
        AutoPtr<IRemoteViewsAdapter> adapter;
        AutoPtr<IAppWidgetManager> mgr;
        CAppWidgetManager::GetInstance(ctx, (IAppWidgetManager**)&mgr);
        ECode pe;
        if ((adapter = mAdapter) != NULL) {
            String opPkgName;
            ctx->GetOpPackageName(&opPkgName);
            pe = mgr->UnbindRemoteViewsService(opPkgName, appWidgetId, intent);
        } else {
            SLOGGERE("RemoteViewsAdapterServiceConnection", "unbind: adapter was null");
        }
        if (FAILED(pe)) {
            SLOGGERE("RemoteViewsAdapterServiceConnection", "Unbind() : Error system server dead?")
            mIsConnecting = FALSE;
            mIsConnected = FALSE;
            return pe;
        }
        mIsConnecting = FALSE;
        return NOERROR;
    // } catch (Exception e) {
    //     Log.e("RemoteViewsAdapterServiceConnection", "unbind(): " + e.getMessage());
    //     mIsConnecting = false;
    //     mIsConnected = false;
    // }
}

ECode CRemoteViewsAdapterServiceConnection::GetRemoteViewsFactory(
    /* [out] */ IIRemoteViewsFactory** factory)
{
    AutoLock lock(mLock);
    *factory = mRemoteViewsFactory;
    REFCOUNT_ADD(*factory);
    return NOERROR;
}

ECode CRemoteViewsAdapterServiceConnection::IsConnected(
    /* [out] */ Boolean* isConnected)
{
    AutoLock lock(mLock);
    *isConnected = mIsConnected;
    return NOERROR;
}

ECode CRemoteViewsAdapterServiceConnection::OnServiceConnected(
    /* [in] */ IBinder* service)
{
    AutoLock lock(mLock);
    mRemoteViewsFactory = IIRemoteViewsFactory::Probe(service);
    // Remove any deferred unbind messages
    // final RemoteViewsAdapter adapter = mAdapter.get();
    // if (adapter == null) return;

    // Queue up work that we need to do for the callback to run
    RemoteViewsAdapter* adapter = (RemoteViewsAdapter*)mAdapter.Get();
    if(adapter == NULL) return NOERROR;
    AutoPtr<IRunnable> r = new MyRunnable(mAdapter);
    Boolean rst;
    adapter->mWorkerQueue->Post(r, &rst);

    // Enqueue unbind message
    adapter->EnqueueDeferredUnbindServiceMessage();
    mIsConnected = TRUE;
    mIsConnecting = FALSE;
    return NOERROR;
}

ECode CRemoteViewsAdapterServiceConnection::OnServiceDisconnected()
{
    AutoLock lock(mLock);
    mIsConnected = FALSE;
    mIsConnecting = FALSE;
    mRemoteViewsFactory = NULL;

    // Clear the main/worker queues
    RemoteViewsAdapter* adapter = (RemoteViewsAdapter*)mAdapter.Get();
    if (adapter == NULL) return NOERROR;
    AutoPtr<IRunnable> r = new MyRunnable(mAdapter);
    Boolean rst;
    adapter->mMainQueue->Post(r, &rst);
    return NOERROR;
}

ECode CRemoteViewsAdapterServiceConnection::ToString(
    /* [out] */ String* str)
{
    *str = "CRemoteViewsAdapterServiceConnection";
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
