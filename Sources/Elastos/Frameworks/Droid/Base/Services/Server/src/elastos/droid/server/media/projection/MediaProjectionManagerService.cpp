
#include "elastos/droid/server/media/projection/MediaProjectionManagerService.h"
#include "elastos/droid/server/media/projection/CMediaProjectionManager.h"
#include "elastos/droid/server/Watchdog.h"
#include "elastos/droid/os/Looper.h"
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Os::Looper;
using Elastos::Droid::Server::Watchdog;
using Elastos::Droid::Server::EIID_IWatchdogMonitor;
using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Media {
namespace Projection {

//==============================================================================
//                  MediaProjectionManagerService::AddCallbackDeathRecipient
//==============================================================================

ECode MediaProjectionManagerService::AddCallbackDeathRecipient::ProxyDied()
{
    AutoLock lock(mHost->mLock);
    RemoveCallback(callback);
    return NOERROR;
}


//==============================================================================
//                  MediaProjectionManagerService::MediaRouterCallback
//==============================================================================

CAR_INTERFACE_IMPL_2(MediaProjectionManagerService::MediaRouterCallback, Object, IMediaRouterCallback, IMediaRouterSimpleCallback)

ECode MediaProjectionManagerService::MediaRouterCallback::OnRouteSelected(
    /* [in] */ IMediaRouter* router,
    /* [in] */ Int32 type,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    AutoLock lock(mHost->mLock);
    if ((type & IMediaRouter::ROUTE_TYPE_REMOTE_DISPLAY) != 0) {
        mHost->mMediaRouteInfo = info;
        if (mHost->mProjectionGrant != NULL) {
            mHost->mProjectionGrant->Stop();
        }
    }
    return NOERROR;
}

ECode MediaProjectionManagerService::MediaRouterCallback::OnRouteUnselected(
    /* [in] */ IMediaRouter* router,
    /* [in] */ Int32 type,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    if (mHost->mMediaRouteInfo.Get() == info) {
        mHost->mMediaRouteInfo = NULL;
    }
    return NOERROR;
}

ECode MediaProjectionManagerService::MediaRouterCallback::OnRouteAdded(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    return NOERROR;
}

ECode MediaProjectionManagerService::MediaRouterCallback::OnRouteRemoved(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    return NOERROR;
}

ECode MediaProjectionManagerService::MediaRouterCallback::OnRouteChanged(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    return NOERROR;
}

ECode MediaProjectionManagerService::MediaRouterCallback::OnRouteGrouped(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IMediaRouterRouteInfo* info,
    /* [in] */ IMediaRouterRouteGroup* group,
    /* [in] */ Int32 index)
{
    return NOERROR;
}

ECode MediaProjectionManagerService::MediaRouterCallback::OnRouteUngrouped(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IMediaRouterRouteInfo* info,
    /* [in] */ IMediaRouterRouteGroup* group)
{
    return NOERROR;
}

ECode MediaProjectionManagerService::MediaRouterCallback::OnRouteVolumeChanged(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    return NOERROR;
}

ECode MediaProjectionManagerService::MediaRouterCallback::OnRoutePresentationDisplayChanged(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    return NOERROR;
}


//==============================================================================
//                  MediaProjectionManagerService::CallbackDelegate
//==============================================================================

MediaProjectionManagerService::CallbackDelegate::CallbackDelegate()
{
    AutoPtr<ILooper> l = Looper::GetMainLooper();
    CHandler::New(l, NULL, TRUE /*async*/, (IHandler**)&mHandler);
}

void MediaProjectionManagerService::CallbackDelegate::Add(
    /* [in] */ IIMediaProjectionCallback* callback)
{
    synchronized (mLock) {
        AutoPtr<IBinder> b = IBinder::Probe(callback);
        mClientCallbacks[b] = callback;
    }
}

void MediaProjectionManagerService::CallbackDelegate::Add(
    /* [in] */ IIMediaProjectionWatcherCallback* callback)
{
    synchronized (mLock) {
        AutoPtr<IBinder> b = IBinder::Probe(callback);
        mWatcherCallbacks[b] = callback;
    }
}

void MediaProjectionManagerService::CallbackDelegate::Remove(
    /* [in] */ IIMediaProjectionCallback* callback)
{
    synchronized (mLock) {
        AutoPtr<IBinder> b = IBinder::Probe(callback);
        mClientCallbacks.Erase(b);
    }
}

void MediaProjectionManagerService::CallbackDelegate::Remove(
    /* [in] */ IIMediaProjectionWatcherCallback* callback)
{
    synchronized (mLock) {
        AutoPtr<IBinder> b = IBinder::Probe(callback);
        mWatcherCallbacks.Erase(b);
    }
}

void MediaProjectionManagerService::CallbackDelegate::DispatchStart(
    /* [in] */ CMediaProjection* projection)
{
    if (projection == NULL) {
        Slogger::E(TAG, "Tried to dispatch start notification for a null media projection. Ignoring!");
        return;
    }
    synchronized (mLock) {
        HashMap<AutoPtr<IBinder>, AutoPtr<IIMediaProjectionWatcherCallback> >::Iterator it = mWatcherCallbacks.Begin();
        for (; it != mWatcherCallbacks.End(); ++it) {
            AutoPtr<IIMediaProjectionWatcherCallback> callback = it->mSecond;
            AutoPtr<IMediaRouterRouteInfo> info = projection->GetProjectionInfo();
            AutoPtr<IRunnable> startWatcher = (IRunnable*)new WatcherStartCallback(info, callback);
            Boolean result;
            mHandler->Post(startWatcher, &result);
        }
    }
}

void MediaProjectionManagerService::CallbackDelegate::DispatchStop(
    /* [in] */ CMediaProjection* projection)
{
    if (projection == NULL) {
        Slogger::E(TAG, "Tried to dispatch stop notification for a null media projection. Ignoring!");
        return;
    }
    synchronized (mLock) {
        HashMap<AutoPtr<IBinder>, AutoPtr<IIMediaProjectionCallback> >::Iterator it = mClientCallbacks.Begin();
        for (; it != mClientCallbacks.End(); ++it) {
            AutoPtr<IIMediaProjectionCallback> callback = it->mSecond;
            AutoPtr<IRunnable> stopClient = (IRunnable*)new ClientStopCallback(callback);
            Boolean result;
            mHandler->Post(stopClient, &result);
        }

        HashMap<AutoPtr<IBinder>, AutoPtr<IIMediaProjectionWatcherCallback> >::Iterator it = mWatcherCallbacks.Begin();
        for (; it != mWatcherCallbacks.End(); ++it) {
            AutoPtr<IIMediaProjectionWatcherCallback> callback = it->mSecond;
            AutoPtr<IMediaRouterRouteInfo> info = projection->GetProjectionInfo();
            AutoPtr<IRunnable> stopWatcher = (IRunnable*)new WatcherStopCallback(info, callback);
            Boolean result;
            mHandler->Post(stopWatcher, &result);
        }
    }
}


//==============================================================================
//                  MediaProjectionManagerService::WatcherStartCallback
//==============================================================================

ECode MediaProjectionManagerService::WatcherStartCallback::Run()
{
    // try {
    if (FAILED(mCallback->OnStart(mInfo))) {
        Slogger::W(TAG, "Failed to notify media projection has stopped");
    }
    return NOERROR;
    // } catch (RemoteException e) {
    //     Slog.w(TAG, "Failed to notify media projection has stopped", e);
    // }
}


//==============================================================================
//                  MediaProjectionManagerService::WatcherStopCallback
//==============================================================================

ECode MediaProjectionManagerService::WatcherStopCallback::Run()
{
    // try {
    if (FAILED(mCallback->OnStop(mInfo))) {
        Slogger::W(TAG, "Failed to notify media projection has stopped");
    }
    return NOERROR;
    // } catch (RemoteException e) {
    //     Slog.w(TAG, "Failed to notify media projection has stopped", e);
    // }
}


//==============================================================================
//                  MediaProjectionManagerService::ClientStopCallback
//==============================================================================

ECode MediaProjectionManagerService::ClientStopCallback::Run()
{
    // try {
    if (FAILED(mCallback->OnStop())) {
        Slogger::W(TAG, "Failed to notify media projection has stopped");
    }
    return NOERROR;
    // } catch (RemoteException e) {
    //     Slog.w(TAG, "Failed to notify media projection has stopped", e);
    // }
}


//==============================================================================
//                  MediaProjectionManagerService
//==============================================================================

const String MediaProjectionManagerService::TAG("MediaProjectionManagerService");

CAR_INTERFACE_IMPL(MediaProjectionManagerService, SystemService, IWatchdogMonitor)

MediaProjectionManagerService::MediaProjectionManagerService(
    /* [in] */ IContext* context)
    : SystemService(context)
{
    mContext = context;
    mCallbackDelegate = new CallbackDelegate();
    AutoPtr<IInterface> service;
    mContext->GetSystemService(IContext::APP_OPS_SERVICE, (IInterface**)&service);
    mAppOps = IAppOpsManager::Probe(service);
    service = NULL;
    mContext->GetSystemService(IContext::MEDIA_ROUTER_SERVICE, (IInterface**)&service);
    mMediaRouter = IMediaRouter::Probe(service);
    mMediaRouterCallback = new MediaRouterCallback(this);
    Watchdog::GetInstance()->AddMonitor((IWatchdogMonitor*)this->Probe(EIID_IWatchdogMonitor));
}

ECode MediaProjectionManagerService::OnStart()
{
    AutoPtr<IBinder> b;
    CMediaProjectionManager::New((Handle64)this, (IBinder**)&b);
    PublishBinderService(IContext::MEDIA_PROJECTION_SERVICE, b, FALSE /*allowIsolated*/);
    return mMediaRouter->AddCallback(IMediaRouter::ROUTE_TYPE_REMOTE_DISPLAY, mMediaRouterCallback,
            IMediaRouter::CALLBACK_FLAG_PASSIVE_DISCOVERY);
}

ECode MediaProjectionManagerService::OnSwitchUser(
    /* [in] */ Int32 userId)
{
    return mMediaRouter->RebindAsUser(userId);
}

ECode MediaProjectionManagerService::Monitor()
{
    synchronized (mLock) { /* check for deadlock */ }
    return NOERROR;
}

void MediaProjectionManagerService::StartProjectionLocked(
    /* [in] */ CMediaProjection* projection)
{
    if (mProjectionGrant != NULL) {
        mProjectionGrant->Stop();
    }
    if (mMediaRouteInfo != NULL) {
        AutoPtr<IMediaRouterRouteInfo> info;
        mMediaRouter->GetDefaultRoute((IMediaRouterRouteInfo**)&info);
        info->Select();
    }
    mProjectionToken = IBinder::Probe(projection);
    mProjectionGrant = projection;
    DispatchStart(projection);
    return NOERROR;
}

void MediaProjectionManagerService::StopProjectionLocked(
    /* [in] */ CMediaProjection* projection)
{
    mProjectionToken = NULL;
    mProjectionGrant = NULL;
    DispatchStop(projection);
    return NOERROR;
}

void MediaProjectionManagerService::AddCallback(
    /* [in] */ IIMediaProjectionWatcherCallback* callback)
{
    AutoPtr<IProxyDeathRecipient> deathRecipient = new AddCallbackDeathRecipient(this);
    synchronized (mLock) {
        mCallbackDelegate->Add(callback);
        LinkDeathRecipientLocked(callback, deathRecipient);
    }
}

void MediaProjectionManagerService::RemoveCallback(
    /* [in] */ IIMediaProjectionWatcherCallback* callback)
{
    synchronized (mLock) {
        UnlinkDeathRecipientLocked(callback);
        mCallbackDelegate->Remove(callback);
    }
}

void MediaProjectionManagerService::LinkDeathRecipientLocked(
    /* [in] */ IIMediaProjectionWatcherCallback* callback,
    /* [in] */ IProxyDeathRecipient* deathRecipient)
{
    // try {
    AutoPtr<IBinder> token = IBinder::Probe(callback);
    AutoPtr<IProxy> proxy = (IProxy*)token->Probe(EIID_IProxy);
    if (proxy != NULL) {
        if (FAILED(proxy->LinkToDeath(deathRecipient, 0))) {
            Slogger::E(TAG, "Unable to link to death for media projection monitoring callback");
        }
    }
    mDeathEaters[token] = deathRecipient;
    // } catch (RemoteException e) {
    //     Slog.e(TAG, "Unable to link to death for media projection monitoring callback", e);
    // }
}

void MediaProjectionManagerService::UnlinkDeathRecipientLocked(
    /* [in] */ IIMediaProjectionWatcherCallback* callback)
{
    AutoPtr<IBinder> token = IBinder::Probe(callback);
    AutoPtr<IProxyDeathRecipient> deathRecipient;
    HashMap<AutoPtr<IBinder>, AutoPtr<IProxyDeathRecipient> >::Iterator it = mDeathEaters.Find(token);
    if (it != mDeathEaters.End()) {
        deathRecipient = it->mSecond;
        mDeathEaters.Erase(it);
    }
    if (deathRecipient != NULL) {
        AutoPtr<IProxy> proxy = (IProxy*)token->Probe(EIID_IProxy);
        if (proxy != NULL) proxy->UnlinkToDeath(deathRecipient, 0);
    }
}

void MediaProjectionManagerService::DispatchStart(
    /* [in] */ CMediaProjection* projection)
{
    mCallbackDelegate->DispatchStart(projection);
}

void MediaProjectionManagerService::DispatchStop(
    /* [in] */ CMediaProjection* projection)
{
    mCallbackDelegate->DispatchStop(projection);
}

Boolean MediaProjectionManagerService::IsValidMediaProjection(
    /* [in] */ IBinder* token)
{
    synchronized (mLock) {
        if (mProjectionToken != NULL) {
            Boolean equals;
            IObject::Probe(mProjectionToken)->Equals(token, &equals);
            return equals;
        }
    }
    return FALSE;
}

AutoPtr<IMediaProjectionInfo> MediaProjectionManagerService::GetActiveProjectionInfo()
{
    synchronized (mLock) {
        if (mProjectionGrant == NULL) {
            return NULL;
        }
        AutoPtr<IMediaProjectionInfo> info = mProjectionGrant->GetProjectionInfo();
    }
    return info;
}

String MediaProjectionManagerService::TypeToString(
    /* [in] */ Int32 type)
{
    switch (type) {
        case IMediaProjectionManager::TYPE_SCREEN_CAPTURE:
            return String("TYPE_SCREEN_CAPTURE");
        case IMediaProjectionManager::TYPE_MIRRORING:
            return String("TYPE_MIRRORING");
        case IMediaProjectionManager::TYPE_PRESENTATION:
            return String("TYPE_PRESENTATION");
    }
    return StringUtils::ToString(type);
}

} // namespace Projection
} // namespace Media
} // namespace Server
} // namespace Droid
} // namespace Elastos
