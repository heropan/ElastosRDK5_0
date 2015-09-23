
#include "appwidget/CAppWidgetHost.h"
#include "appwidget/CAppWidgetHostHelper.h"
#include "appwidget/CAppWidgetHostCallbacks.h"
#include "appwidget/CAppWidgetProviderInfo.h"
#include "appwidget/CAppWidgetHostView.h"
#include "app/CActivityThreadHelper.h"
#include "utility/CTypedValue.h"
#include "utility/CTypedValueHelper.h"
#include "os/Process.h"
#include "os/Binder.h"
#include "os/CUserHandle.h"
#include "os/UserHandle.h"
#include "os/ServiceManager.h"


using Elastos::Droid::App::IContextImpl;
using Elastos::Droid::App::IActivityThreadHelper;
using Elastos::Droid::App::CActivityThreadHelper;
using Elastos::Droid::App::IActivityThread;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::CUserHandle;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::Utility::ITypedValueHelper;
using Elastos::Droid::Utility::CTypedValueHelper;

namespace Elastos {
namespace Droid {
namespace AppWidget {

const Int32 CAppWidgetHost::HANDLE_UPDATE;
const Int32 CAppWidgetHost::HANDLE_PROVIDER_CHANGED;
const Int32 CAppWidgetHost::HANDLE_PROVIDERS_CHANGED;
const Int32 CAppWidgetHost::HANDLE_VIEW_DATA_CHANGED;
Mutex CAppWidgetHost::sServiceLock;
AutoPtr<IIAppWidgetService> CAppWidgetHost::sService;

ECode CAppWidgetHost::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    switch (what) {
        case CAppWidgetHost::HANDLE_UPDATE: {
            mHost->UpdateAppWidgetView(arg1, IRemoteViews::Probe(obj));
            break;
        }
        case CAppWidgetHost::HANDLE_PROVIDER_CHANGED: {
            mHost->OnProviderChanged(arg1, IAppWidgetProviderInfo::Probe(obj));
            break;
        }
        case CAppWidgetHost::HANDLE_PROVIDERS_CHANGED: {
            mHost->OnProvidersChanged();
            break;
        }
        case CAppWidgetHost::HANDLE_VIEW_DATA_CHANGED: {
            mHost->ViewDataChanged(arg1, arg2);
            break;
        }
    }
    return NOERROR;
}

CAppWidgetHost::CAppWidgetHost()
    : mHostId(0)
{
    CAppWidgetHostCallbacks::New(this, (IIAppWidgetHost**)&mCallbacks);
}

ECode CAppWidgetHost::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ Int32 hostId)
{
    assert(ctx != NULL);
    AutoPtr<ILooper> looper;
    ctx->GetMainLooper((ILooper**)&looper);
    return Init(ctx, hostId, NULL, looper);
}

ECode CAppWidgetHost::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ Int32 hostId,
    /* [in] */ IRemoteViewsOnClickHandler* handler,
    /* [in] */ ILooper* looper)
{
    return Init(ctx, hostId, handler, looper);
}

ECode CAppWidgetHost::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 hostId,
    /* [in] */ IRemoteViewsOnClickHandler* handler,
    /* [in] */ ILooper* looper)
{
    mContext = context;
    mHostId = hostId;
    mOnClickHandler = handler;
    mHandler = new MyHandler(looper, this);
    assert(context != NULL);
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    assert(res != NULL);
    res->GetDisplayMetrics((IDisplayMetrics**)&mDisplayMetrics);
    Process::MyUserHandle((IUserHandle**)&mUser);
    BindService();
    return NOERROR;
}

void CAppWidgetHost::SetUserId(
    /* [in] */ Int32 userId)
{
    CUserHandle::New(userId, (IUserHandle**)&mUser);
}

void CAppWidgetHost::BindService()
{
    AutoLock lock(sServiceLock);
    if (sService == NULL) {
        sService = IIAppWidgetService::Probe(ServiceManager::GetService(IContext::APPWIDGET_SERVICE));
    }
}

ECode CAppWidgetHost::StartListening()
{
    return StartListeningAsUser(UserHandle::GetMyUserId());
}

ECode CAppWidgetHost::StartListeningAsUser(
    /* [in] */ Int32 userId)
{
    AutoPtr< ArrayOf<Int32> > updatedIds;
    AutoPtr<IObjectContainer> updatedViews;

    // try {
    if (mPackageName.IsNull()) {
        mContext->GetPackageName(&mPackageName);
    }
    ECode ec = sService->StartListeningAsUser(mCallbacks, mPackageName, mHostId,
            (IObjectContainer**)&updatedViews, userId, (ArrayOf<Int32>**)&updatedIds);
    if (FAILED(ec)) return E_RUNTIME_EXCEPTION;
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }

    AutoPtr<IObjectEnumerator> ator;
    updatedViews->GetObjectEnumerator((IObjectEnumerator**)&ator);
    const Int32 N = updatedIds->GetLength();
    Boolean has = FALSE;
    for (Int32 i = 0; i < N; i++) {
        ator->MoveNext(&has);
        AutoPtr<IRemoteViews> obj;
        if (has) {
            ator->Current((IInterface**)&obj);
            AutoPtr<IUserHandle> handle;
            CUserHandle::New(userId, (IUserHandle**)&handle);
            obj->SetUser(handle);
        }
        UpdateAppWidgetView((*updatedIds)[i], obj);
    }
    return NOERROR;
}

ECode CAppWidgetHost::StopListening()
{
    // try {
    ECode ec = sService->StopListeningAsUser(mHostId, UserHandle::GetMyUserId());
    if (FAILED(ec)) return E_RUNTIME_EXCEPTION;
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
    return NOERROR;
}

ECode CAppWidgetHost::StopListeningAsUser(
    /* [in] */ Int32 userId)
{
    // try {
    if (FAILED(sService->StopListeningAsUser(mHostId, userId))) {
        return E_RUNTIME_EXCEPTION;
    }
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
    // Also clear the views
    ClearViews();
    return NOERROR;
}

ECode CAppWidgetHost::AllocateAppWidgetId(
    /* [out] */ Int32* appWidgetId)
{
    // try {
    if (mPackageName.IsNull()) {
        mContext->GetPackageName(&mPackageName);
    }
    ECode ec = sService->AllocateAppWidgetId(mPackageName, mHostId, appWidgetId);
    if (FAILED(ec)) return E_RUNTIME_EXCEPTION;
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
    return NOERROR;
}

ECode CAppWidgetHost::AllocateAppWidgetIdForSystem(
    /* [in] */ Int32 hostId,
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    FAIL_RETURN(CheckCallerIsSystem());
    // try {
    if (sService == NULL) {
        BindService();
    }
    AutoPtr<IActivityThreadHelper> helper;
    CActivityThreadHelper::AcquireSingleton((IActivityThreadHelper**)&helper);
    AutoPtr<IActivityThread> currentActivityThread;
    helper->GetCurrentActivityThread((IActivityThread**)&currentActivityThread);
    AutoPtr<IContextImpl> tmp;
    currentActivityThread->GetSystemContext((IContextImpl**)&tmp);
    AutoPtr<IContext> systemContext = IContext::Probe(tmp);
    String packageName;
    systemContext->GetPackageName(&packageName);
    ECode ec = sService->AllocateAppWidgetId(packageName, hostId, id);
    if (FAILED(ec)) return E_RUNTIME_EXCEPTION;
    return ec;
    // } catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetHost::GetAppWidgetIds(
    /* [out] */ ArrayOf<Int32>** ids)
{
    VALIDATE_NOT_NULL(ids)
    // try {
    if (sService == NULL) {
        BindService();
    }
    return sService->GetAppWidgetIdsForHost(mHostId, ids);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetHost::CheckCallerIsSystem()
{
    Int32 uid = Process::MyUid();
    if (CUserHandle::GetAppId(uid) == IProcess::SYSTEM_UID || uid == 0) {
        return NOERROR;
    }
    return E_SECURITY_EXCEPTION;
}

Boolean CAppWidgetHost::IsLocalBinder()
{
    return Process::MyPid() == Binder::GetCallingPid();
}

ECode CAppWidgetHost::DeleteAppWidgetId(
    /* [in] */ Int32 appWidgetId)
{
    AutoLock lock(mViewsLock);
    mViews.Erase(appWidgetId);
    // try {
    ECode ec = sService->DeleteAppWidgetId(appWidgetId);
    if (FAILED(ec)) return E_RUNTIME_EXCEPTION;
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
    return NOERROR;
}

ECode CAppWidgetHost::DeleteAppWidgetIdForSystem(
    /* [in] */ Int32 appWidgetId)
{
    FAIL_RETURN(CheckCallerIsSystem());
    // try {
    if (sService == NULL) {
        BindService();
    }
    ECode ec = sService->DeleteAppWidgetId(appWidgetId);
    if (FAILED(ec)) return E_RUNTIME_EXCEPTION;
    // } catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
    return NOERROR;
}

ECode CAppWidgetHost::DeleteHost()
{
    // try {
    ECode ec = sService->DeleteHost(mHostId);
    if (FAILED(ec)) return E_RUNTIME_EXCEPTION;
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
    return NOERROR;
}

ECode CAppWidgetHost::DeleteAllHosts()
{
    // try {
    ECode ec = sService->DeleteAllHosts();
    if (FAILED(ec)) return E_RUNTIME_EXCEPTION;
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
    return NOERROR;
}

ECode CAppWidgetHost::CreateView(
    /* [in] */ IContext* context,
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IAppWidgetProviderInfo* appWidget,
    /* [out] */ IAppWidgetHostView** hostView)
{
    VALIDATE_NOT_NULL(hostView);

    AutoPtr<IAppWidgetHostView> view = OnCreateView(context, appWidgetId, appWidget);
    Int32 id;
    mUser->GetIdentifier(&id);
    view->SetUserId(id);
    view->SetOnClickHandler(mOnClickHandler);
    view->SetAppWidget(appWidgetId, appWidget);
    {
        AutoLock lock(mViewsLock);
        mViews[appWidgetId] = view;
    }
    AutoPtr<IParcelable> views;
    // try {
    ECode ec = sService->GetAppWidgetViews(appWidgetId, (IParcelable**)&views);
    if (FAILED(ec)) return E_RUNTIME_EXCEPTION;
    if (IRemoteViews::Probe(views) != NULL) {
        AutoPtr<IRemoteViews> rv = IRemoteViews::Probe(views);
        rv->SetUser(mUser);
    }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
    view->UpdateAppWidget(IRemoteViews::Probe(views));

    *hostView = view;
    REFCOUNT_ADD(*hostView);
    return NOERROR;
}

AutoPtr<IAppWidgetHostView> CAppWidgetHost::OnCreateView(
    /* [in] */ IContext* context,
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IAppWidgetProviderInfo* appWidget)
{
    AutoPtr<IAppWidgetHostView> view;
    CAppWidgetHostView::New(context, mOnClickHandler, (IAppWidgetHostView**)&view);
    return view;
}

void CAppWidgetHost::OnProviderChanged(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IAppWidgetProviderInfo* app)
{
    AutoPtr<IAppWidgetHostView> v;

    Int32 minWidth, minHeight, minResizeWidth, minResizeHeight;
    app->GetMinWidth(&minWidth);
    app->GetMinHeight(&minHeight);
    app->GetMinResizeWidth(&minResizeWidth);
    app->GetMinResizeHeight(&minResizeHeight);
    // Convert complex to dp -- we are getting the AppWidgetProviderInfo from the
    // AppWidgetService, which doesn't have our context, hence we need to do the
    // conversion here.
    minWidth = CTypedValue::ComplexToDimensionPixelSize(minWidth, mDisplayMetrics);
    app->SetMinWidth(minWidth);
    minHeight = CTypedValue::ComplexToDimensionPixelSize(minHeight, mDisplayMetrics);
    app->SetMinHeight(minHeight);
    minResizeWidth = CTypedValue::ComplexToDimensionPixelSize(minResizeWidth, mDisplayMetrics);
    app->SetMinResizeWidth(minResizeWidth);
    minResizeHeight = CTypedValue::ComplexToDimensionPixelSize(minResizeHeight, mDisplayMetrics);
    app->SetMinResizeHeight(minResizeHeight);

    {
        AutoLock lock(mViewsLock);
        HashMap<Int32, AutoPtr<IAppWidgetHostView> >::Iterator it = mViews.Find(appWidgetId);
        if (it != mViews.End()) {
            v = it->mSecond;
        }
    }
    if (v != NULL) {
        v->ResetAppWidget(app);
    }
}

void CAppWidgetHost::OnProvidersChanged()
{
    // Do nothing
}

ECode CAppWidgetHost::UpdateAppWidgetView(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IRemoteViews* views)
{
    AutoPtr<IAppWidgetHostView> v;
    {
        AutoLock lock(mViewsLock);
        HashMap<Int32, AutoPtr<IAppWidgetHostView> >::Iterator it = mViews.Find(appWidgetId);
        if (it != mViews.End()) {
            v = it->mSecond;
        }
    }
    if (v != NULL) {
        v->UpdateAppWidget(views);
    }
    return NOERROR;
}

ECode CAppWidgetHost::ViewDataChanged(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ Int32 viewId)
{
    AutoPtr<IAppWidgetHostView> v;
    {
        AutoLock lock(mViewsLock);
        HashMap<Int32, AutoPtr<IAppWidgetHostView> >::Iterator it = mViews.Find(appWidgetId);
        if (it != mViews.End()) {
            v = it->mSecond;
        }
    }
    if (v != NULL) {
        v->ViewDataChanged(viewId);
    }
    return NOERROR;
}

void CAppWidgetHost::ClearViews()
{
    mViews.Clear();
}

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos
