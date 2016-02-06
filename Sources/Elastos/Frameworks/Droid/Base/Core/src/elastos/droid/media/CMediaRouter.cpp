#include "Elastos.Droid.Graphics.h"
#include "elastos/droid/app/CActivityThread.h"
#include "elastos/droid/content/CIntentFilter.h"
#include "elastos/droid/content/res/CResourcesHelper.h"
#include "elastos/droid/media/CAudioRoutesInfo.h"
#include "elastos/droid/media/CMediaRouter.h"
#include "elastos/droid/media/CRouteCategory.h"
#include "elastos/droid/media/CMediaRouteInfo.h"
#include "elastos/droid/media/VolumeProvider.h"
#include "elastos/droid/media/CUserRouteInfo.h"
#include "elastos/droid/media/MediaRouterClientState.h"
#include "elastos/droid/os/CHandler.h"
#include "elastos/droid/os/CServiceManager.h"
#include "elastos/droid/os/Process.h"
#include "elastos/droid/R.h"
#include "elastos/droid/text/TextUtils.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/droid/Manifest.h>
#include <elastos/droid/os/UserHandle.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/CoreUtils.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/Math.h>
#include <elastos/utility/etl/List.h>

using Elastos::Droid::App::CActivityThread;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::Res::CResourcesHelper;
using Elastos::Droid::Content::Res::IResourcesHelper;
using Elastos::Droid::Hardware::Display::EIID_IDisplayListener;
using Elastos::Droid::Manifest;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IServiceManager;
using Elastos::Droid::Os::CServiceManager;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Text::TextUtils;
using Elastos::Utility::Etl::List;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Logging::Slogger;
using Elastos::Core::CString;
using Elastos::Core::CoreUtils;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::Math;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Media {

const String CMediaRouter::TAG("MediaRouter");

AutoPtr<IMediaRouterStatic> CMediaRouter::sStatic;

const Int32 CMediaRouter::ROUTE_TYPE_ANY = IMediaRouter::ROUTE_TYPE_LIVE_AUDIO | IMediaRouter::ROUTE_TYPE_LIVE_VIDEO
            | IMediaRouter::ROUTE_TYPE_REMOTE_DISPLAY | IMediaRouter::ROUTE_TYPE_USER;

HashMap< AutoPtr<IContext>, AutoPtr<IMediaRouter> > CMediaRouter::sRouters;

Object CMediaRouter::mStaticClass;

//-----------------------------------------------
//    CMediaRouter::Static::MyAudioRoutesObserver
//-----------------------------------------------

CAR_INTERFACE_IMPL(CMediaRouter::Static::MyAudioRoutesObserver, Object, IIAudioRoutesObserver);

CMediaRouter::Static::MyAudioRoutesObserver::MyAudioRoutesObserver(
    /* [in] */ Static* owner)
    : mOwner(owner)
{}

ECode CMediaRouter::Static::MyAudioRoutesObserver::DispatchAudioRoutesChanged(
    /* [in] */ IAudioRoutesInfo* newRoutes)
{
    AutoPtr<MyRunnable> myRunnable = new MyRunnable(newRoutes, mOwner);
    Boolean tempState;
    return mOwner->mHandler->Post(myRunnable.Get(), &tempState);
}

//------------------------------------
//    CMediaRouter::Static::MyRunnable
//------------------------------------
CAR_INTERFACE_IMPL(CMediaRouter::Static::MyRunnable, Object, IRunnable)

CMediaRouter::Static::MyRunnable::MyRunnable(
    /* [in] */ IAudioRoutesInfo* newRoutes,
    /* [in] */ Static* owner)
    : mOwner(owner)
{}

ECode CMediaRouter::Static::MyRunnable::Run()
{
    if (IObject::Equals(IIMediaRouterClient::Probe(this), mClient.Get())) {
        return mOwner->UpdateAudioRoutes(mNewRoutes);
    }
    return NOERROR;
}

//------------------------------------
//    CMediaRouter::Static::MyStaticClient
//------------------------------------

CAR_INTERFACE_IMPL(CMediaRouter::Static::MyStaticClient, Object, IIMediaRouterClient);

CMediaRouter::Static::MyStaticClient::MyStaticClient(
    /* [in] */ Static* owner)
    : mOwner(owner)
{}

ECode CMediaRouter::Static::MyStaticClient::OnStateChanged()
{
    AutoPtr<MyRunnable> mr = new MyRunnable(NULL, mOwner);
    Boolean flag = FALSE;
    return mHandler->Post(IRunnable::Probe(mr), &flag);
}

//------------------------
//    CMediaRouter::Static
//------------------------

CAR_INTERFACE_IMPL_2(CMediaRouter::Static, Object, IDisplayListener, IMediaRouterStatic);

CMediaRouter::Static::Static()
    : mCanConfigureWifiDisplays(FALSE)
    , mActivelyScanningWifiDisplays(FALSE)
    , mDiscoveryRequestRouteTypes(0)
    , mCurrentUserId(-1)
{}

CMediaRouter::Static::~Static()
{}

ECode CMediaRouter::Static::constructor(
    /* [in] */ IContext* appContext,
    /* [in] */ CMediaRouter* owner)
{
    mOwner = owner;
    mAppContext = appContext;
    CAudioRoutesInfo::New((IAudioRoutesInfo**)&mCurAudioRoutesInfo);
    mAudioRoutesObserver = new MyAudioRoutesObserver(this);

    AutoPtr<IResourcesHelper> resourcesHelper;
    CResourcesHelper::AcquireSingleton((IResourcesHelper**)&resourcesHelper);
    resourcesHelper->GetSystem((IResources**)&mResources);

    AutoPtr<ILooper> looper;
    appContext->GetMainLooper((ILooper**)&looper);
    CHandler::New(looper, (IHandler**)&mHandler);

    AutoPtr<IServiceManager> serviceManager;
    CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager);
    AutoPtr<IInterface> obj;
    serviceManager->GetService(IContext::AUDIO_SERVICE, (IInterface**)&obj);
    AutoPtr<IBinder> b = IBinder::Probe(obj);
    mAudioService = IIAudioService::Probe(obj.Get());

    appContext->GetSystemService(IContext::DISPLAY_SERVICE, (IInterface**)&mDisplayService);

    obj = NULL;
    serviceManager->GetService(IContext::MEDIA_ROUTER_SERVICE, (IInterface**)&obj);
    mMediaRouterService = IIMediaRouterService::Probe(obj);

    CRouteCategory::New(R::string::default_audio_route_category_name,
        ROUTE_TYPE_LIVE_AUDIO | ROUTE_TYPE_LIVE_VIDEO, FALSE, (IMediaRouterRouteCategory**)&mSystemCategory);

    ((RouteCategory*)mSystemCategory.Get())->mIsSystem = TRUE;

    // Only the system can configure wifi displays.  The display manager
    // enforces this with a permission check.  Set a flag here so that we
    // know whether this process is actually allowed to scan and connect.
    Int32 vol;
    appContext->CheckPermission(Manifest::permission::CONFIGURE_WIFI_DISPLAY,
        Process::MyPid(), Process::MyUid(), &vol);

    mCanConfigureWifiDisplays = (vol == IPackageManager::PERMISSION_GRANTED);
    return NOERROR;
}

ECode CMediaRouter::Static::OnDisplayAdded(
    /* [in] */ Int32 displayId)
{
    UpdatePresentationDisplays(displayId);
    return NOERROR;
}

ECode CMediaRouter::Static::OnDisplayChanged(
    /* [in] */ Int32 displayId)
{
    UpdatePresentationDisplays(displayId);
    return NOERROR;
}

ECode CMediaRouter::Static::OnDisplayRemoved(
    /* [in] */ Int32 displayId)
{
    UpdatePresentationDisplays(displayId);
    return NOERROR;
}

ECode CMediaRouter::Static::GetAllPresentationDisplays(
    /* [out, callee] */ ArrayOf<IDisplay*>** result)
{
    VALIDATE_NOT_NULL(result);
    return mDisplayService->GetDisplays(IDisplayManager::DISPLAY_CATEGORY_PRESENTATION, result);
}

ECode CMediaRouter::Static::StartMonitoringRoutes(
    /* [in] */ IContext* appContext)
{
    CMediaRouteInfo::New(mSystemCategory, (IMediaRouterRouteInfo**)&mDefaultAudioVideo);

    String str = StringUtils::ToString(R::string::default_audio_route_name);

    ((RouteInfo*)mDefaultAudioVideo.Get())->SetName(StringUtils::ParseCharSequence(str).Get());
    mDefaultAudioVideo->SetSupportedTypes((IMediaRouter::ROUTE_TYPE_LIVE_AUDIO | IMediaRouter::ROUTE_TYPE_LIVE_VIDEO));
    Boolean flag = FALSE;
    mDefaultAudioVideo->UpdatePresentationDisplay(&flag);
    AddRouteStatic((RouteInfo*)mDefaultAudioVideo.Get());

    // This will select the active wifi display route if there is one.
    AutoPtr<IWifiDisplayStatus> wifiDisplayStatus;
    mDisplayService->GetWifiDisplayStatus((IWifiDisplayStatus**)&wifiDisplayStatus);
    UpdateWifiDisplayStatus(wifiDisplayStatus);

    AutoPtr<WifiDisplayStatusChangedReceiver> wifiDisplayStatusChangedReceiver
        = new WifiDisplayStatusChangedReceiver();

    AutoPtr<IIntentFilter> intentFilter1;
    CIntentFilter::New(IDisplayManager::ACTION_WIFI_DISPLAY_STATUS_CHANGED,
        (IIntentFilter**)&intentFilter1);
    AutoPtr<IIntent> intent;
    appContext->RegisterReceiver(wifiDisplayStatusChangedReceiver, intentFilter1, (IIntent**)&intent);

    AutoPtr<VolumeChangeReceiver> volumeChangeReceiver = new VolumeChangeReceiver();

    AutoPtr<IIntentFilter> intentFilter2;
    CIntentFilter::New(IAudioManager::VOLUME_CHANGED_ACTION, (IIntentFilter**)&intentFilter2);
    intent = NULL;
    appContext->RegisterReceiver(volumeChangeReceiver, intentFilter2, (IIntent**)&intent);

    mDisplayService->RegisterDisplayListener(this, mHandler);

    AutoPtr<IAudioRoutesInfo> newAudioRoutes = NULL;
//    try {
        mAudioService->StartWatchingRoutes(mAudioRoutesObserver, (IAudioRoutesInfo**)&newAudioRoutes);
//    } catch (RemoteException e) {
//    }
    if (newAudioRoutes != NULL) {
        // This will select the active BT route if there is one and the current
        // selected route is the default system route, or if there is no selected
        // route yet.
        UpdateAudioRoutes(newAudioRoutes);
    }

    // Bind to the media router service.
    RebindAsUser(UserHandle::GetMyUserId());

    // Select the default route if the above didn't sync us up
    // appropriately with relevant system state.
    if (mSelectedRoute == NULL) {
        SelectDefaultRouteStatic();
    }
    return NOERROR;
}

ECode CMediaRouter::Static::UpdateAudioRoutes(
    /* [in] */ IAudioRoutesInfo* newRoutes)
{
    Int32 tempValue1;
    newRoutes->GetMainType(&tempValue1);
    Int32 tempValue2;
    mCurAudioRoutesInfo->GetMainType(&tempValue2);
    if (tempValue1 != tempValue2) {
        mCurAudioRoutesInfo->SetMainType(tempValue1);
        Int32 name;
        if ((tempValue1 & CAudioRoutesInfo::MAIN_HEADPHONES) != 0
                || (tempValue1 & CAudioRoutesInfo::MAIN_HEADSET) != 0) {
            name = R::string::default_audio_route_name_headphones;
        }
        else if ((tempValue1 & CAudioRoutesInfo::MAIN_DOCK_SPEAKERS) != 0) {
            name = R::string::default_audio_route_name_dock_speakers;
        }
        else if ((tempValue1 & CAudioRoutesInfo::MAIN_HDMI) != 0) {
            name = R::string::default_media_route_name_hdmi;
        }
        else {
            name = R::string::default_audio_route_name;
        }
        ((RouteInfo*)(((Static*)(sStatic.Get()))->mDefaultAudioVideo).Get())->mNameResId = name;
        DispatchRouteChanged((((Static*)(sStatic.Get()))->mDefaultAudioVideo).Get());
    }

    Int32 mainType;
    mCurAudioRoutesInfo->GetMainType(&mainType);

    AutoPtr<ICharSequence> charSequence1;
    newRoutes->GetBluetoothName((ICharSequence**)&charSequence1);
    AutoPtr<ICharSequence> charSequence2;
    mCurAudioRoutesInfo->GetBluetoothName((ICharSequence**)&charSequence2);
    if (!TextUtils::Equals(charSequence1, charSequence2)) {
        mCurAudioRoutesInfo->SetBluetoothName(charSequence1);
        if (charSequence1 != NULL) {
            if (((Static*)(sStatic.Get()))->mBluetoothA2dpRoute == NULL) {
                AutoPtr<IMediaRouterUserRouteInfo> info;
                CUserRouteInfo::New(((Static*)(sStatic.Get()))->mSystemCategory, (IMediaRouterUserRouteInfo**)&info);
                AutoPtr<ICharSequence> cs;
                ((Static*)(sStatic.Get()))->mResources->GetText(R::string::bluetooth_a2dp_audio_route_name, (ICharSequence**)&cs);
                info->SetDescription(cs);
                info->SetName(charSequence1);
                IMediaRouterRouteInfo::Probe(info)->SetSupportedTypes(IMediaRouter::ROUTE_TYPE_LIVE_AUDIO);
                ((Static*)(sStatic.Get()))->mBluetoothA2dpRoute = IMediaRouterRouteInfo::Probe(info);
                AddRouteStatic((RouteInfo*)(((Static*)(sStatic.Get()))->mBluetoothA2dpRoute).Get());
            } else {
                ((Static*)(sStatic.Get()))->mBluetoothA2dpRoute->SetName(charSequence2);
                DispatchRouteChanged((RouteInfo*)(((Static*)(sStatic.Get()))->mBluetoothA2dpRoute).Get());
            }
        } else if (((Static*)(sStatic.Get()))->mBluetoothA2dpRoute != NULL) {
            RemoveRouteStatic((RouteInfo*)(((Static*)(sStatic.Get()))->mBluetoothA2dpRoute).Get());
            ((Static*)(sStatic.Get()))->mBluetoothA2dpRoute = NULL;
        }
    }

    if (mBluetoothA2dpRoute != NULL) {
        Boolean a2dpEnabled;
        IsBluetoothA2dpOn(&a2dpEnabled);
        if (mainType != CAudioRoutesInfo::MAIN_SPEAKER &&
                mSelectedRoute == mBluetoothA2dpRoute && !a2dpEnabled) {
            SelectRouteStatic(ROUTE_TYPE_LIVE_AUDIO, (RouteInfo*)mDefaultAudioVideo.Get(), FALSE);
        } else if ((mSelectedRoute == mDefaultAudioVideo || mSelectedRoute == NULL) &&
                a2dpEnabled) {
            SelectRouteStatic(ROUTE_TYPE_LIVE_AUDIO, (RouteInfo*)mBluetoothA2dpRoute.Get(), FALSE);
        }
    }
    return NOERROR;
}

ECode CMediaRouter::Static::IsBluetoothA2dpOn(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    ECode ec = NOERROR;
    Boolean flag = FALSE;
    ec = mAudioService->IsBluetoothA2dpOn(&flag);
    // } catch (RemoteException e) {
    if (SUCCEEDED(ec)) {
        *result = flag;
        return NOERROR;
    }

    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Slogger::E(TAG, "Error querying Bluetooth A2DP state");
        *result = FALSE;
        return NOERROR;
    }
    // }
}

ECode CMediaRouter::Static::UpdateDiscoveryRequest()
{
    // What are we looking for today?
    Int32 routeTypes = 0;
    Int32 passiveRouteTypes = 0;
    Boolean activeScan = FALSE;
    Boolean activeScanWifiDisplay = FALSE;

    List< AutoPtr<IMediaRouterRouteCallbackInfo> >::Iterator it;
    AutoPtr<IMediaRouterRouteCallbackInfo> cbi_;
    AutoPtr<CallbackInfo> cbi;
    for (it = mCallbacks.Begin(); it != mCallbacks.End(); ++it) {
        cbi_ = NULL;
        cbi_ = *it;
        cbi = NULL;
        cbi = (CallbackInfo*)(cbi_.Get());
        if ((cbi->mFlags & (CALLBACK_FLAG_PERFORM_ACTIVE_SCAN
            | CALLBACK_FLAG_REQUEST_DISCOVERY)) != 0) {
            // Discovery explicitly requested.
            routeTypes |= cbi->mType;
        } else if ((cbi->mFlags & CALLBACK_FLAG_PASSIVE_DISCOVERY) != 0) {
            // Discovery only passively requested.
            passiveRouteTypes |= cbi->mType;
        } else {
            // Legacy case since applications don't specify the discovery flag.
            // Unfortunately we just have to assume they always need discovery
            // whenever they have a callback registered.
            routeTypes |= cbi->mType;
        }
        if ((cbi->mFlags & CALLBACK_FLAG_PERFORM_ACTIVE_SCAN) != 0) {
            activeScan = TRUE;
        if ((cbi->mType & ROUTE_TYPE_REMOTE_DISPLAY) != 0) {
            activeScanWifiDisplay = TRUE;
        }
        }
    }
    if (routeTypes != 0 || activeScan) {
        // If someone else requests discovery then enable the passive listeners.
        // This is used by the MediaRouteButton and MediaRouteActionProvider since
        // they don't receive lifecycle callbacks from the Activity.
        routeTypes |= passiveRouteTypes;
    }

    // Update wifi display scanning.
    // TODO: All of this should be managed by the media router service.
    if (mCanConfigureWifiDisplays) {
        Boolean bmathes = FALSE;
        mSelectedRoute->MatchesTypes(ROUTE_TYPE_REMOTE_DISPLAY, &bmathes);
        if (mSelectedRoute != NULL && bmathes) {
            // Don't scan while already connected to a remote display since
            // it may interfere with the ongoing transmission.
            activeScanWifiDisplay = FALSE;
        }
        if (activeScanWifiDisplay) {
            if (!mActivelyScanningWifiDisplays) {
                mActivelyScanningWifiDisplays = TRUE;
                mDisplayService->StartWifiDisplayScan();
            }
        } else {
            if (mActivelyScanningWifiDisplays) {
                mActivelyScanningWifiDisplays = FALSE;
                mDisplayService->StopWifiDisplayScan();
            }
        }
    }

    // Tell the media router service all about it.
    if (routeTypes != mDiscoveryRequestRouteTypes
            || activeScan != mDiscoverRequestActiveScan) {
        mDiscoveryRequestRouteTypes = routeTypes;
        mDiscoverRequestActiveScan = activeScan;
        PublishClientDiscoveryRequest();
    }
    return NOERROR;
}

ECode CMediaRouter::Static::SetSelectedRoute(
    /* [in] */ IMediaRouterRouteInfo* info,
    /* [in] */ Boolean bexplicit)
{
    // Must be non-reentrant.
    mSelectedRoute = info;
    return PublishClientSelectedRoute(bexplicit);
}

ECode CMediaRouter::Static::RebindAsUser(
    /* [in] */ Int32 userId)
{
    if (mCurrentUserId != userId || userId < 0 || mClient == NULL) {
        if (mClient != NULL) {
            // try {
            ECode ec = mMediaRouterService->UnregisterClient(mClient);
            if (ec == (ECode)E_REMOTE_EXCEPTION) {
                Slogger::E(TAG, "Unable to unregister media router client.");
            }
            // } catch (RemoteException ex) {
                // Log.e(TAG, "Unable to unregister media router client.", ex);
            // }
            mClient = NULL;
        }

        mCurrentUserId = userId;

        // try {
        AutoPtr<MyStaticClient> client = new MyStaticClient(this);
        String packageName;
        mAppContext->GetPackageName(&packageName);
        ECode ec = mMediaRouterService->RegisterClientAsUser(IIMediaRouterClient::Probe(client), packageName, userId);
        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            Slogger::E(TAG, "Unable to register media router client.");
        }
        mClient = IIMediaRouterClient::Probe(client);
        // } catch (RemoteException ex) {
            // Log.e(TAG, "Unable to register media router client.", ex);
        // }

        PublishClientDiscoveryRequest();
        PublishClientSelectedRoute(FALSE);
        UpdateClientState();
    }
    return NOERROR;
}

ECode CMediaRouter::Static::PublishClientDiscoveryRequest()
{
    if (mClient != NULL) {
    // try {
    ECode ec = mMediaRouterService->SetDiscoveryRequest(mClient,
                mDiscoveryRequestRouteTypes, mDiscoverRequestActiveScan);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Slogger::E(TAG, "Unable to publish media router client discovery request.");
    }
    // } catch (RemoteException ex) {
        // Log.e(TAG, "Unable to publish media router client discovery request.", ex);
    // }
    }
    return NOERROR;
}

ECode CMediaRouter::Static::PublishClientSelectedRoute(
    /* [in] */ Boolean bexplicit)
{
    if (mClient != NULL) {
    // try {
    ECode ec = mMediaRouterService->SetSelectedRoute(mClient,
        mSelectedRoute != NULL ? ((RouteInfo*)(mSelectedRoute.Get()))->mGlobalRouteId : String(NULL), bexplicit);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Slogger::E(TAG, "Unable to publish media router client selected route.");
    }
    // } catch (RemoteException ex) {
        // Log.e(TAG, "Unable to publish media router client selected route.", ex);
    // }
    }
    return NOERROR;
}

ECode CMediaRouter::Static::UpdateClientState()
{
    // Update the client state.
    mClientState = NULL;
    ECode ec = NOERROR;
    if (mClient != NULL) {
        // try {
        ec = mMediaRouterService->GetState(mClient, (IMediaRouterClientState**)&mClientState);
        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            Slogger::E(TAG, "Unable to retrieve media router client state.");
        }
        // } catch (RemoteException ex) {
            // Log.e(TAG, "Unable to retrieve media router client state.", ex);
        // }
    }
    // ArrayList<MediaRouterClientState::RouteInfo> globalRoutes =
    AutoPtr<IArrayList> globalRoutes =
        mClientState != NULL ? ((MediaRouterClientState*)mClientState.Get())->mRoutes : NULL;
    String globallySelectedRouteId = mClientState != NULL ?
            ((MediaRouterClientState*)mClientState.Get())->mGloballySelectedRouteId : String(NULL);

    // Add or update routes.
    Int32 size;
    Int32 globalRouteCount = globalRoutes != NULL ? (globalRoutes->GetSize(&size), size) : 0;
    AutoPtr<IInterface> obj;
    AutoPtr<MediaRouterClientState::RouteInfo> globalRoute;
    AutoPtr<IMediaRouterRouteInfo> route;
    for (Int32 i = 0; i < globalRouteCount; i++) {
        obj = NULL;
        globalRoutes->Get(i, (IInterface**)&obj);
        globalRoute = NULL;
        globalRoute = (MediaRouterClientState::RouteInfo*)IMediaRouterClientStateRouteInfo::Probe(obj);
        route = NULL;
        FindGlobalRoute(globalRoute->mId, (IMediaRouterRouteInfo**)&route);
        if (route == NULL) {
            MakeGlobalRoute(IMediaRouterClientStateRouteInfo::Probe(globalRoute), (IMediaRouterRouteInfo**)&route);
            AddRouteStatic(route);
        } else {
            UpdateGlobalRoute(route, IMediaRouterClientStateRouteInfo::Probe(globalRoute));
        }
    }

    // Synchronize state with the globally selected route.
    if (!globallySelectedRouteId.IsNull()) {
        FindGlobalRoute(globallySelectedRouteId, (IMediaRouterRouteInfo**)&route);
        if (route == NULL) {
            Slogger::W(TAG, "Could not find new globally selected route: %s"
                    , globallySelectedRouteId.string());
        } else if (route != mSelectedRoute) {
            if (DEBUG) {
                Slogger::D(TAG, "Selecting new globally selected route: %p", route.Get());
            }
            SelectRouteStatic(((RouteInfo*)(route.Get()))->mSupportedTypes, route.Get(), FALSE);
        }
    } else if (mSelectedRoute != NULL && !(((RouteInfo*)mSelectedRoute.Get())->mGlobalRouteId).IsNull()) {
        if (DEBUG) {
            Logger::D(TAG, "Unselecting previous globally selected route: %p" , mSelectedRoute.Get());
        }
        SelectDefaultRouteStatic();
    }
    return NOERROR;

    // Remove defunct routes.
    OUTER:
    {
        List<AutoPtr<IMediaRouterRouteInfo> >::Iterator it = mRoutes.Begin();
        AutoPtr<IMediaRouterRouteInfo> _route_;
        AutoPtr<RouteInfo> route_;
        AutoPtr<IInterface> obj_;
        AutoPtr<IMediaRouterClientStateRouteInfo> mcs;
        AutoPtr<MediaRouterClientState::RouteInfo> globalRoute_;
        for (; it != mRoutes.End(); ++it) {
            route_ = NULL;
            _route_ = *it;
            route_ = (RouteInfo*)(_route_.Get());
            String globalRouteId = route_->mGlobalRouteId;
            if (!globalRouteId.IsNull()) {
                for (Int32 j = 0; j < globalRouteCount; ++j) {
                    obj_ = NULL;
                    globalRoutes->Get(j, (IInterface**)&obj_);
                    mcs = NULL;
                    mcs = IMediaRouterClientStateRouteInfo::Probe(obj_);
                    globalRoute_ = NULL;
                    globalRoute_ = (MediaRouterClientState::RouteInfo*)(mcs.Get());
                    if (globalRouteId.Equals(globalRoute_->mId)) {
                        continue OUTER; //found
                    }
                }
                // notfound
                RemoveRouteStatic(IMediaRouterRouteInfo::Probe(route_));
            }
        }
    }
}

void CMediaRouter::Static::UpdatePresentationDisplays(
    /* [in] */ Int32 changedDisplayId)
{
    List<AutoPtr<IMediaRouterRouteInfo> >::Iterator it;
    AutoPtr<IMediaRouterRouteInfo> route_;
    AutoPtr<RouteInfo> route;
    Boolean updated = FALSE;
    Int32 displayId;
    for (it = mRoutes.Begin(); it != mRoutes.End(); ++it) {
        route_ = NULL;
        route_ = *it;
        route_->UpdatePresentationDisplay(&updated);
        route = NULL;
        route = (RouteInfo*)(route_.Get());
        route->mPresentationDisplay->GetDisplayId(&displayId);
        if (updated || (route->mPresentationDisplay != NULL
                && displayId == changedDisplayId)) {
            DispatchRoutePresentationDisplayChanged(route_);
        }
    }
}

ECode CMediaRouter::Static::FindGlobalRoute(
    /* [in] */ const String& globalRouteId,
    /* [out] */ IMediaRouterRouteInfo** result)
{
    VALIDATE_NOT_NULL(result);

    List< AutoPtr<IMediaRouterRouteInfo> >::Iterator it = mRoutes.Begin();
    AutoPtr<IMediaRouterRouteInfo> route_;
    AutoPtr<RouteInfo> route;
    for (; it != mRoutes.End(); ++it) {
        route_ = NULL;
        route_ = *it;
        route = (RouteInfo*)(route_.Get());
        if (globalRouteId.Equals(route->mGlobalRouteId)) {
            *result = route_;
            REFCOUNT_ADD(*result);
            return NOERROR;
        }
    }

    *result = NULL;
    return NOERROR;
}

ECode CMediaRouter::Static::RequestUpdateVolume(
    /* [in] */ IMediaRouterRouteInfo* route,
    /* [in] */ Int32 direction)
{
    AutoPtr<RouteInfo> route_ = (RouteInfo*)route;
    if (!(route_->mGlobalRouteId).IsNull() && mClient != NULL) {
        // try {
        ECode ec = mMediaRouterService->RequestUpdateVolume(mClient.Get(),
                    route_->mGlobalRouteId, direction);
        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            Slogger::W(TAG, "Unable to request volume change.");
        }
        // } catch (RemoteException ex) {
            // Log.w(TAG, "Unable to request volume change.", ex);
        // }
    }
    return NOERROR;
}

ECode CMediaRouter::Static::MakeGlobalRoute(
    /* [in] */ IMediaRouterClientStateRouteInfo* globalRoute,
    /* [out] */ IMediaRouterRouteInfo** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<RouteInfo> route = new RouteInfo();
    //todo
    // route->constructor(mOwner, (((Static*)(sStatic.Get()))->mSystemCategory).Get());
    AutoPtr<MediaRouterClientState::RouteInfo> globalRoute_ = (MediaRouterClientState::RouteInfo*)globalRoute;
    route->mGlobalRouteId = globalRoute_->mId;
    AutoPtr<ICharSequence> cs =
        StringUtils::ParseCharSequence(globalRoute_->mName);
    route->SetName(cs.Get());
    cs = NULL;
    cs = StringUtils::ParseCharSequence(globalRoute_->mDescription);
    route->mDescription = cs.Get();
    route->mSupportedTypes = globalRoute_->mSupportedTypes;
    route->mEnabled = globalRoute_->mEnabled;
    Boolean flag = FALSE;
    route->SetRealStatusCode(globalRoute_->mStatusCode, &flag);
    route->mPlaybackType = globalRoute_->mPlaybackType;
    route->mPlaybackStream = globalRoute_->mPlaybackStream;
    route->mVolume = globalRoute_->mVolume;
    route->mVolumeMax = globalRoute_->mVolumeMax;
    route->mVolumeHandling = globalRoute_->mVolumeHandling;
    route->mPresentationDisplayId = globalRoute_->mPresentationDisplayId;
    route->UpdatePresentationDisplay(&flag);
    *result = IMediaRouterRouteInfo::Probe(route);
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::Static::UpdateGlobalRoute(
    /* [in] */ IMediaRouterRouteInfo* route,
    /* [in] */ IMediaRouterClientStateRouteInfo* globalRoute)
{
    Boolean changed = FALSE;
    Boolean volumeChanged = FALSE;
    Boolean presentationDisplayChanged = FALSE;

    AutoPtr<RouteInfo> route_ = (RouteInfo*)route;
    AutoPtr<MediaRouterClientState::RouteInfo> globalRoute_ = (MediaRouterClientState::RouteInfo*)globalRoute;

    AutoPtr<ICharSequence> cs =
        StringUtils::ParseCharSequence(globalRoute_->mName);
    //todo
    // if (!IObject::Equals((route_->mName).Get(), cs.Get())) {
    //     route_->mName = cs.Get();
    //     changed = TRUE;
    // }
    cs = NULL;
    cs = StringUtils::ParseCharSequence(globalRoute_->mDescription);
    // if (!IObject::Equals((route_->mDescription).Get(), cs.Get())) {
    //     route_->mDescription = cs.Get();
    //     changed = TRUE;
    // }
    Int32 oldSupportedTypes = route_->mSupportedTypes;
    if (oldSupportedTypes != globalRoute_->mSupportedTypes) {
        route_->mSupportedTypes = globalRoute_->mSupportedTypes;
        changed = TRUE;
    }
    if (route_->mEnabled != globalRoute_->mEnabled) {
        route_->mEnabled = globalRoute_->mEnabled;
        changed = TRUE;
    }
    if (route_->mRealStatusCode != globalRoute_->mStatusCode) {
        Boolean flag = FALSE;
        route_->SetRealStatusCode(globalRoute_->mStatusCode, &flag);
        changed = TRUE;
    }
    if (route_->mPlaybackType != globalRoute_->mPlaybackType) {
        route_->mPlaybackType = globalRoute_->mPlaybackType;
        changed = TRUE;
    }
    if (route_->mPlaybackStream != globalRoute_->mPlaybackStream) {
        route_->mPlaybackStream = globalRoute_->mPlaybackStream;
        changed = TRUE;
    }
    if (route_->mVolume != globalRoute_->mVolume) {
        route_->mVolume = globalRoute_->mVolume;
        changed = TRUE;
        volumeChanged = TRUE;
    }
    if (route_->mVolumeMax != globalRoute_->mVolumeMax) {
        route_->mVolumeMax = globalRoute_->mVolumeMax;
        changed = TRUE;
        volumeChanged = TRUE;
    }
    if (route_->mVolumeHandling != globalRoute_->mVolumeHandling) {
        route_->mVolumeHandling = globalRoute_->mVolumeHandling;
        changed = TRUE;
        volumeChanged = TRUE;
    }
    if (route_->mPresentationDisplayId != globalRoute_->mPresentationDisplayId) {
        route_->mPresentationDisplayId = globalRoute_->mPresentationDisplayId;
        Boolean flag = FALSE;
        route_->UpdatePresentationDisplay(&flag);
        changed = TRUE;
        presentationDisplayChanged = TRUE;
    }

    if (changed) {
        DispatchRouteChanged(route_.Get(), oldSupportedTypes);
    }
    if (volumeChanged) {
        DispatchRouteVolumeChanged(route_.Get());
    }
    if (presentationDisplayChanged) {
        DispatchRoutePresentationDisplayChanged(route_.Get());
    }
    return NOERROR;
}

//================================================================================
//                      CMediaRouter::RouteInfo
//================================================================================
CAR_INTERFACE_IMPL(CMediaRouter::RouteInfo, Object, IMediaRouterRouteInfo)

CMediaRouter::RouteInfo::RouteInfo()
    : mNameResId(0)
    , mSupportedTypes(0)
    , mPlaybackType(IMediaRouterRouteInfo::PLAYBACK_TYPE_LOCAL)
    , mVolumeMax(IRemoteControlClient::DEFAULT_PLAYBACK_VOLUME)
    , mVolume(IRemoteControlClient::DEFAULT_PLAYBACK_VOLUME)
    , mVolumeHandling(IRemoteControlClient::DEFAULT_PLAYBACK_VOLUME_HANDLING)
    , mPlaybackStream(IAudioManager::STREAM_MUSIC)
    , mPresentationDisplayId(-1)
    , mEnabled(TRUE)
    , mRealStatusCode(0)
    , mResolvedStatusCode(0)
{}

CMediaRouter::RouteInfo::~RouteInfo()
{}

ECode CMediaRouter::RouteInfo::constructor(
    /* [in] */ CMediaRouter* host,
    /* [in] */ IMediaRouterRouteCategory* category)
{
    mHost = host;
    mCategory = category;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::GetName(
    /* [out] */ ICharSequence ** result)
{
    VALIDATE_NOT_NULL(result);
    return GetName(((Static*)(sStatic.Get()))->mResources.Get(), result);
}

ECode CMediaRouter::RouteInfo::SetName(
    /* [in] */ ICharSequence* name)
{
    mName = name;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::GetName(
    /* [in] */ IContext * context,
    /* [out] */ ICharSequence ** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    return GetName(res.Get(), result);
}

ECode CMediaRouter::RouteInfo::GetName(
    /* [in] */ IResources* res,
    /* [out] */ ICharSequence** result)
{
    VALIDATE_NOT_NULL(result);
    if (mNameResId != 0) {
        res->GetText(mNameResId, (ICharSequence**)&mName);
        *result = mName.Get();
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    *result = mName.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::GetDescription(
    /* [out] */ ICharSequence ** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mDescription;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::GetStatus(
    /* [out] */ ICharSequence ** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mStatus;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::SetRealStatusCode(
    /* [in] */ Int32 statusCode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mRealStatusCode != statusCode) {
        mRealStatusCode = statusCode;
        return ResolveStatusCode(result);
    }
    *result = FALSE;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::ResolveStatusCode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 statusCode = mRealStatusCode;
    Boolean flag = FALSE;
    IsSelected(&flag);
    if (flag) {
        switch (statusCode) {
            // If the route is selected and its status appears to be between states
            // then report it as connecting even though it has not yet had a chance
            // to officially move into the CONNECTING state.  Note that routes in
            // the NONE state are assumed to not require an explicit connection
            // lifecycle whereas those that are AVAILABLE are assumed to have
            // to eventually proceed to CONNECTED.
            case STATUS_AVAILABLE:
            case STATUS_SCANNING:
                statusCode = IMediaRouterRouteInfo::STATUS_CONNECTING;
                break;
        }
    }
    if (mResolvedStatusCode == statusCode) {
        *result = FALSE;
        return NOERROR;
    }

    mResolvedStatusCode = statusCode;
    Int32 resId;
    switch (statusCode) {
        case IMediaRouterRouteInfo::STATUS_SCANNING:
            resId = R::string::media_route_status_scanning;
            break;
        case IMediaRouterRouteInfo::STATUS_CONNECTING:
            resId = R::string::media_route_status_connecting;
            break;
        case IMediaRouterRouteInfo::STATUS_AVAILABLE:
            resId = R::string::media_route_status_available;
            break;
        case IMediaRouterRouteInfo::STATUS_NOT_AVAILABLE:
            resId = R::string::media_route_status_not_available;
            break;
        case IMediaRouterRouteInfo::STATUS_IN_USE:
            resId = R::string::media_route_status_in_use;
            break;
        case IMediaRouterRouteInfo::STATUS_CONNECTED:
        case IMediaRouterRouteInfo::STATUS_NONE:
        default:
            resId = 0;
            break;
    }
    AutoPtr<ICharSequence> cs;
    ((Static*)(sStatic.Get()))->mResources->GetText(resId, (ICharSequence**)&cs);

    mStatus = resId != 0 ? cs.Get() : NULL;
    *result = TRUE;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::GetStatusCode(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result);
    *result = mResolvedStatusCode;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::GetSupportedTypes(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result);
    *result = mSupportedTypes;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::SetSupportedTypes(
    /* [in] */ Int32 type)
{
    mSupportedTypes = type;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::MatchesTypes(
    /* [in] */ Int32 types,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = (mSupportedTypes & types) != 0;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::GetGroup(
    /* [out] */ IMediaRouterRouteGroup ** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mGroup;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::GetCategory(
    /* [out] */ IMediaRouterRouteCategory ** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mCategory;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::GetIconDrawable(
    /* [out] */ IDrawable ** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mIcon;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::SetTag(
    /* [in] */ IInterface * tag)
{
    // mTag = (Object)(*(IObject::Probe(tag)));
    return RouteUpdated();
}

ECode CMediaRouter::RouteInfo::GetTag(
    /* [out] */ IInterface ** result)
{
    VALIDATE_NOT_NULL(result);
    // *result = (IInterface*)mTag;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::GetPlaybackType(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result);
    *result = mPlaybackType;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::GetPlaybackStream(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result);
    *result = mPlaybackStream;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::GetVolume(
    /* [out] */ Int32 * result)
{
    if (mPlaybackType == IMediaRouterRouteInfo::PLAYBACK_TYPE_LOCAL) {
        Int32 vol = 0;
        // try {
        ECode ec = ((Static*)(sStatic.Get()))->mAudioService->GetStreamVolume(mPlaybackStream, &vol);
        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            Slogger::E(TAG, "Error getting local stream volume");
        }
        // } catch (RemoteException e) {
            // Log.e(TAG, "Error getting local stream volume", e);
        // }
        *result = vol;
        return NOERROR;
    } else {
        *result = mVolume;
        return NOERROR;
    }
}

ECode CMediaRouter::RouteInfo::RequestSetVolume(
    /* [in] */ Int32 volume)
{
    if (mPlaybackType == IMediaRouterRouteInfo::PLAYBACK_TYPE_LOCAL) {
        // try {
        String currentPackageName = CActivityThread::GetCurrentPackageName();
        ECode ec = ((Static*)(sStatic.Get()))->mAudioService->SetStreamVolume(mPlaybackStream, volume, 0,
            currentPackageName);
        // } catch (RemoteException e) {
        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            Slogger::E(TAG, "Error setting local stream volume");
        }

        if (SUCCEEDED(ec)) {
            return ec;
        }
        // }
    } else {
        return ((Static*)(sStatic.Get()))->RequestSetVolume(this, volume);
    }
}

ECode CMediaRouter::RouteInfo::RequestUpdateVolume(
    /* [in] */ Int32 direction)
{
    if (mPlaybackType == IMediaRouterRouteInfo::PLAYBACK_TYPE_LOCAL) {
        // try {
        Int32 volume_;
        GetVolume(&volume_);
        Int32 volumeMax;
        GetVolumeMax(&volumeMax);
        Int32 volume = Elastos::Core::Math::Max(0, Elastos::Core::Math::Min(volume_ + direction, volumeMax));
        ECode ec = ((Static*)(sStatic.Get()))->mAudioService->SetStreamVolume(mPlaybackStream, volume, 0,
                    CActivityThread::GetCurrentPackageName());
        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            Slogger::E(TAG, "Error setting local stream volume");
        }
        if (SUCCEEDED(ec)) {
            return ec;
        }
        // } catch (RemoteException e) {
            // Log.e(TAG, "Error setting local stream volume", e);
        // }
    } else {
        return ((Static*)(sStatic.Get()))->RequestUpdateVolume(this, direction);
    }
}

ECode CMediaRouter::RouteInfo::GetVolumeMax(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result);

    if (mPlaybackType == IMediaRouterRouteInfo::PLAYBACK_TYPE_LOCAL) {
        // try {
        ECode ec = ((Static*)(sStatic.Get()))->mAudioService->GetStreamMaxVolume(mPlaybackStream, result);
        if (SUCCEEDED(ec)) {
            return ec;
        }
        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            Slogger::E(TAG, "Error getting local stream volume");
        }
        // } catch (RemoteException e) {
            // Log.e(TAG, "Error getting local stream volume", e);
        // }
    } else {
        *result = mVolumeMax;
        return NOERROR;
    }
}

ECode CMediaRouter::RouteInfo::GetVolumeHandling(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result);
    *result = mVolumeHandling;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::GetPresentationDisplay(
    /* [out] */ IDisplay ** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mPresentationDisplay;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::UpdatePresentationDisplay(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IDisplay> display = ChoosePresentationDisplay();
    if (mPresentationDisplay != display) {
        mPresentationDisplay = display;
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::GetDeviceAddress(
    /* [out] */ String * result)
{
    VALIDATE_NOT_NULL(result);
    *result = mDeviceAddress;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::IsEnabled(
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = mEnabled;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::IsConnecting(
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = mResolvedStatusCode == IMediaRouterRouteInfo::STATUS_CONNECTING;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::IsSelected(
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = IMediaRouterRouteInfo::Probe(this) == ((Static*)(sStatic.Get()))->mSelectedRoute;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::IsDefault(
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = IMediaRouterRouteInfo::Probe(this) == ((Static*)(sStatic.Get()))->mDefaultAudioVideo;
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::Select()
{
    SelectRouteStatic(mSupportedTypes, this, TRUE);
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::SetStatusInt(
    /* [in] */ ICharSequence* status)
{
    Boolean flag = status == mStatus.Get();
    if (!flag) {
        mStatus = status;
        if (mGroup != NULL) {
            ((RouteGroup*)mGroup.Get())->MemberStatusChanged(this, status);
        }
        return RouteUpdated();
    }
}

ECode CMediaRouter::RouteInfo::RouteUpdated()
{
    UpdateRoute(this);
    return NOERROR;
}

ECode CMediaRouter::RouteInfo::ToString(
    /* [out] */ String* result)
{
    Int32 types;
    GetSupportedTypes(&types);
    String supportedTypes = mHost->TypesToString(types);
    String name;
    AutoPtr<ICharSequence> cs;
    GetName((ICharSequence**)&cs);
    cs->ToString(&name);
    cs = NULL;
    GetDescription((ICharSequence**)&cs);
    String description;
    cs->ToString(&description);
    cs = NULL;
    String status;
    GetStatus((ICharSequence**)&cs);
    cs->ToString(&status);
    AutoPtr<IMediaRouterRouteCategory> rc;
    GetCategory((IMediaRouterRouteCategory **)&rc);
    String category;
    IObject::Probe(rc)->ToString(&category);
    String presentationDisplay;
    IObject::Probe(mPresentationDisplay)->ToString(&presentationDisplay);

    *result = /*getClass().getSimpleName() */ String("{ name=") + name +
            String(", description=") + description +
            String(", status=") + status +
            String(", category=") + category +
            String(", supportedTypes=") + supportedTypes +
            String(", presentationDisplay=") + presentationDisplay + String(" }");

    return NOERROR;
}

AutoPtr<IDisplay> CMediaRouter::RouteInfo::ChoosePresentationDisplay()
{
    if ((mSupportedTypes & IMediaRouter::ROUTE_TYPE_LIVE_VIDEO) != 0) {
        AutoPtr<ArrayOf<IDisplay*> > displays;
        ((Static*)(sStatic.Get()))->GetAllPresentationDisplays((ArrayOf<IDisplay*>**)&displays);

        // Ensure that the specified display is valid for presentations.
        // This check will normally disallow the default display unless it was
        // configured as a presentation display for some reason.
        assert(displays != NULL);
        Int32 len = displays->GetLength();

        AutoPtr<IDisplay> display;

        if (mPresentationDisplayId >= 0) {
            for (Int32 i = 0; i < len; i++) {
                Int32 displayId;
                (*displays)[i]->GetDisplayId(&displayId);
                if (displayId == mPresentationDisplayId) {
                    display = NULL;
                    display = (*displays)[i];
                    return display;
                }
            }
            return NULL;
        }

        // Find the indicated Wifi display by its address.
        if (mDeviceAddress != NULL) {
            for (Int32 i = 0; i < len; i++) {
                Int32 vol;
                (*displays)[i]->GetType(&vol);

                if (vol == IDisplay::TYPE_WIFI) {
                    String address;
                    (*displays)[i]->GetAddress(&address);
                    if (mDeviceAddress.Equals(address)) {
                        display = NULL;
                        display = (*displays)[i];
                        return display;
                    }
                }
            }
            return NULL;
        }

        // For the default route, choose the first presentation display from the list.
        // if (IMediaRouterRouteInfo::Probe(this).Equals(((Static*)(sStatic.Get()))->mDefaultAudioVideo) && len > 0) {
        //     display = NULL;
        //     display = (*displays)[0];
        //     return display;
        // }
    }
    return NULL;
}

//================================================================================
//                      CMediaRouter::UserRouteInfo::SessionVolumeProvider
//================================================================================
CMediaRouter::UserRouteInfo::SessionVolumeProvider::SessionVolumeProvider()
{}

CMediaRouter::UserRouteInfo::SessionVolumeProvider::~SessionVolumeProvider()
{}

CAR_INTERFACE_IMPL_2(CMediaRouter::UserRouteInfo::SessionVolumeProvider, Object, IVolumeProvider, IMediaRouterUserRouteInfoSessionVolumeProvider)

ECode CMediaRouter::UserRouteInfo::SessionVolumeProvider::constructor(
    /* [in] */ Int32 volumeControl,
    /* [in] */ Int32 maxVolume,
    /* [in] */ Int32 currentVolume)
{
    return VolumeProvider::constructor(volumeControl, maxVolume, currentVolume);
}

ECode CMediaRouter::UserRouteInfo::SessionVolumeProvider::OnSetVolumeTo(
    /* [in] */ Int32 volume)
{
    assert(0 && "TODO");
    // sStatic.mHandler.post(new Runnable() {
    //     @Override
    //     public void run() {
    //         if (mVcb != null) {
    //             mVcb.vcb.onVolumeSetRequest(mVcb.route, volume);
    //         }
    //     }
    // });
}

ECode CMediaRouter::UserRouteInfo::SessionVolumeProvider::OnAdjustVolume(
    /* [in] */ Int32 direction)
{
    // sStatic.mHandler.post(new Runnable() {
    //     @Override
    //     public void run() {
    //         if (mVcb != null) {
    //             mVcb.vcb.onVolumeUpdateRequest(mVcb.route, direction);
    //         }
    //     }
    // });
}

//================================================================================
//                      CMediaRouter::UserRouteInfo
//================================================================================
CMediaRouter::UserRouteInfo::UserRouteInfo()
{}

CMediaRouter::UserRouteInfo::~UserRouteInfo()
{}

CAR_INTERFACE_IMPL_2(CMediaRouter::UserRouteInfo, Object, IMediaRouterUserRouteInfo, IMediaRouterRouteInfo)

ECode CMediaRouter::UserRouteInfo::constructor(
    /* [in] */ IMediaRouterRouteCategory* category)
{
    //todo
    // RouteInfo::constructor(category);
    mSupportedTypes = IMediaRouter::ROUTE_TYPE_USER;
    mPlaybackType = IMediaRouterRouteInfo::PLAYBACK_TYPE_REMOTE;
    mVolumeHandling = IMediaRouterRouteInfo::PLAYBACK_VOLUME_FIXED;
    return NOERROR;
}

ECode CMediaRouter::UserRouteInfo::SetName(
    /* [in] */ ICharSequence * name)
{
    mName = name;
    return RouteUpdated();
}

ECode CMediaRouter::UserRouteInfo::SetName(
    /* [in] */ Int32 resId)
{
    mNameResId = resId;
    mName = NULL;
    return RouteUpdated();
}

ECode CMediaRouter::UserRouteInfo::SetDescription(
    /* [in] */ ICharSequence * description)
{
    mDescription = description;
    return RouteUpdated();
}

ECode CMediaRouter::UserRouteInfo::SetStatus(
    /* [in] */ ICharSequence * status)
{
    return RouteInfo::SetStatusInt(status);
}

ECode CMediaRouter::UserRouteInfo::SetRemoteControlClient(
    /* [in] */ IRemoteControlClient * rcc)
{
    mRcc = rcc;
    UpdatePlaybackInfoOnRcc();
    return NOERROR;
}

ECode CMediaRouter::UserRouteInfo::GetRemoteControlClient(
    /* [out] */ IRemoteControlClient ** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mRcc;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::UserRouteInfo::SetIconDrawable(
    /* [in] */ IDrawable * icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CMediaRouter::UserRouteInfo::SetIconResource(
    /* [in] */ Int32 resId)
{
    AutoPtr<IDrawable> dr;
    ((Static*)(sStatic.Get()))->mResources->GetDrawable(resId, (IDrawable**)&dr);
    return SetIconDrawable(dr.Get());
}

ECode CMediaRouter::UserRouteInfo::SetVolumeCallback(
    /* [in] */ IMediaRouterVolumeCallback * vcb)
{
    AutoPtr<VolumeCallbackInfo> v = new VolumeCallbackInfo();
    v->constructor(vcb, this);
    mVcb = v;
    return NOERROR;
}

ECode CMediaRouter::UserRouteInfo::SetPlaybackType(
    /* [in] */ Int32 type)
{
    if (mPlaybackType != type) {
        mPlaybackType = type;
        ConfigureSessionVolume();
    }
    return NOERROR;
}

ECode CMediaRouter::UserRouteInfo::SetVolumeHandling(
    /* [in] */ Int32 volumeHandling)
{
    if (mVolumeHandling != volumeHandling) {
        mVolumeHandling = volumeHandling;
        ConfigureSessionVolume();
    }
    return NOERROR;
}

ECode CMediaRouter::UserRouteInfo::SetVolume(
    /* [in] */ Int32 volume)
{
    Int32 vol;
    GetVolumeMax(&vol);
    volume = Elastos::Core::Math::Max(0, Elastos::Core::Math::Min(volume, vol));
    if (mVolume != volume) {
        mVolume = volume;
        if (mSvp != NULL) {
            mSvp->SetCurrentVolume(mVolume);
        }
        DispatchRouteVolumeChanged(this);
        if (mGroup != NULL) {
            mGroup->MemberVolumeChanged(this);
        }
    }
    return NOERROR;
}

ECode CMediaRouter::UserRouteInfo::RequestSetVolume(
    /* [in] */ Int32 volume)
{
    if (mVolumeHandling == IMediaRouterRouteInfo::PLAYBACK_VOLUME_VARIABLE) {
        if (mVcb == NULL) {
            Slogger::E(TAG, "Cannot requestSetVolume on user route - no volume callback set");
            return NOERROR;
        }
        mVcb->vcb->OnVolumeSetRequest(this, volume);
    }
    return NOERROR;
}

ECode CMediaRouter::UserRouteInfo::RequestUpdateVolume(
    /* [in] */ Int32 direction)
{
    if (mVolumeHandling == IMediaRouterRouteInfo::PLAYBACK_VOLUME_VARIABLE) {
        if (mVcb == NULL) {
            Slogger::E(TAG, "Cannot requestChangeVolume on user route - no volumec callback set");
            return NOERROR;
        }
        mVcb->mVcb->OnVolumeUpdateRequest(this, direction);
    }
    return NOERROR;
}

ECode CMediaRouter::UserRouteInfo::SetVolumeMax(
    /* [in] */ Int32 volumeMax)
{
    if (mVolumeMax != volumeMax) {
        mVolumeMax = volumeMax;
        ConfigureSessionVolume();
    }
    return NOERROR;
}

ECode CMediaRouter::UserRouteInfo::SetPlaybackStream(
    /* [in] */ Int32 stream)
{
    if (mPlaybackStream != stream) {
        mPlaybackStream = stream;
        ConfigureSessionVolume();
    }
    return NOERROR;
}

void CMediaRouter::UserRouteInfo::UpdatePlaybackInfoOnRcc()
{
    ConfigureSessionVolume();
}

void CMediaRouter::UserRouteInfo::ConfigureSessionVolume()
{
    if (mRcc == NULL) {
        if (DEBUG) {
            Slogger::D(TAG, "No Rcc to configure volume for route %s", mName.string());
        }
        return;
    }
    AutoPtr<IMediaSession> session;
    mRcc->GetMediaSession((IMediaSession**)&session);
    if (session == NULL) {
        if (DEBUG) {
            Slogger::D(TAG, "Rcc has no session to configure volume");
        }
        return;
    }
    if (mPlaybackType == IRemoteControlClient::PLAYBACK_TYPE_REMOTE) {
        Int32 volumeControl = IVolumeProvider::VOLUME_CONTROL_FIXED;
        switch (mVolumeHandling) {
            case IRemoteControlClient::PLAYBACK_VOLUME_VARIABLE:
                volumeControl = IVolumeProvider::VOLUME_CONTROL_ABSOLUTE;
                break;
            case IRemoteControlClient::PLAYBACK_VOLUME_FIXED:
            default:
                break;
        }
        // Only register a new listener if necessary
        Int32 vol;
        IVolumeProvider::Probe(mSvp)->GetVolumeControl(&vol);
        Int32 vm;
        IVolumeProvider::Probe(mSvp)-GetMaxVolume(&vm);
        if (mSvp == NULL || vol != volumeControl
                || vm != mVolumeMax) {
            AutoPtr<SessionVolumeProvider> s = new SessionVolumeProvider();
            s->constructor(volumeControl, mVolumeMax, mVolume);
            mSvp = s;
            session->SetPlaybackToRemote(IVolumeProvider::Probe(mSvp));
        }
    } else {
        // We only know how to handle local and remote, fall back to local if not remote.
        AutoPtr<IAudioAttributesBuilder> bob;
        CAudioAttributesBuilder::New((IAudioAttributesBuilder**)&bob);
        bob->SetLegacyStreamType(mPlaybackStream);
        AutoPtr<IAudioAttributes> attributes;
        bob->Build((IAudioAttributes**)&attributes);
        session->SetPlaybackToLocal(attributes.Get());
        mSvp = NULL;
    }
}

//================================================================================
//                      CMediaRouter::RouteGroup
//================================================================================
CMediaRouter::RouteGroup::RouteGroup()
    : mUpdateName(FALSE)
{}

CMediaRouter::RouteGroup::~RouteGroup()
{}

CAR_INTERFACE_IMPL_2(CMediaRouter::RouteGroup, Object, IMediaRouterRouteGroup, IMediaRouterRouteInfo)

ECode CMediaRouter::RouteGroup::constructor(
    /* [in] */ IMediaRouterRouteCategory* category)
{
    RouteInfo::constructor(category);
    mGroup = this;
    mVolumeHandling = IMediaRouterRouteInfo::PLAYBACK_VOLUME_FIXED;
    return NOERROR;
}

ECode CMediaRouter::RouteGroup::GetName(
    /* [in] */ IResources* res,
    /* [out] */ ICharSequence** result)
{
    VALIDATE_NOT_NULL(result);

    if (mUpdateName) UpdateName();
    return RouteInfo::GetName(res, result);
}

ECode CMediaRouter::RouteGroup::AddRoute(
    /* [in] */ IMediaRouterRouteInfo * route)
{
    VALIDATE_NOT_NULL(route);
    AutoPtr<IMediaRouterRouteGroup> mr;
    route->GetGroup((IMediaRouterRouteGroup**)&mr);
    if (mr != NULL) {
        Slogger::E("RouteGroup", "Route %p is already part of a group.", route);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AutoPtr<IMediaRouterRouteCategory> rc;
    route->GetCategory((IMediaRouterRouteCategory**)&rc);

    if (!rc->Equals(mCategory)) {
        Slogger::E("RouteGroup", "Route cannot be added to a group with a different category. (Route category=%p group category=%p)", rc, mCategory);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;;
    }
    Int32 at;
    mRoutes->GetSize(&at);
    mRoutes->Add(route);
    route->mGroup = this;
    mUpdateName = TRUE;
    UpdateVolume();
    RouteUpdated();
    DispatchRouteGrouped(route, this, at);
    return NOERROR;
}

ECode CMediaRouter::RouteGroup::AddRoute(
    /* [in] */ IMediaRouterRouteInfo * route,
    /* [in] */ Int32 insertAt)
{
    VALIDATE_NOT_NULL(route);
    AutoPtr<IMediaRouterRouteGroup> mr;
    route->GetGroup((IMediaRouterRouteGroup**)&mr);
    if (mr != NULL) {
        Slogger::E("RouteGroup", "Route %p is already part of a group.", route);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AutoPtr<IMediaRouterRouteCategory> rc;
    route->GetCategory((IMediaRouterRouteCategory**)&rc);

    if (!rc->Equals(mCategory)) {
        Slogger::E("RouteGroup", "Route cannot be added to a group with a different category. (Route category=%p group category=%p)", rc, mCategory);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;;
    }
    mRoutes->Add(insertAt, route);
    route->mGroup = this;
    mUpdateName = TRUE;
    UpdateVolume();
    RouteUpdated();
    DispatchRouteGrouped(route, this, insertAt);
    return NOERROR;
}

ECode CMediaRouter::RouteGroup::RemoveRoute(
    /* [in] */ IMediaRouterRouteInfo * route)
{
    VALIDATE_NOT_NULL(route);
    AutoPtr<IMediaRouterRouteGroup> mr;
    route->GetGroup((IMediaRouterRouteGroup**)&mr);
    if (!IObject::Equals(mr.Get(), IMediaRouterRouteGroup::Probe(this))) {
        Slogger::E("RouteGroup", "Route %p is not a member of this group.", route);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mRoutes->Remove(route);
    route->mGroup = NULL;
    mUpdateName = TRUE;
    UpdateVolume();
    DispatchRouteUngrouped(route, this);
    return RouteUpdated();
}

ECode CMediaRouter::RouteGroup::RemoveRoute(
    /* [in] */ Int32 index)
{
    AutoPtr<IInterface> obj;
    mRoutes->Remove(index, (IInterface**)&obj);
    AutoPtr<RouteInfo> route = (RouteInfo*)IMediaRouterRouteInfo::Probe(obj);
    route->mGroup = NULL;
    mUpdateName = TRUE;
    UpdateVolume();
    DispatchRouteUngrouped(route, this);
    return RouteUpdated();
}

ECode CMediaRouter::RouteGroup::GetRouteCount(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result);
    Int32 size;
    mRoutes->GetSize(&size);
    *result = siz;
    return NOERROR;
}

ECode CMediaRouter::RouteGroup::GetRouteAt(
    /* [in] */ Int32 index,
    /* [out] */ IMediaRouterRouteInfo ** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IInterface> obj;
    mRoutes->Get(index, (IInterface**)&obj);
    *result = IMediaRouterRouteInfo::Probe(obj);
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::RouteGroup::SetIconDrawable(
    /* [in] */ IDrawable * icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CMediaRouter::RouteGroup::SetIconResource(
    /* [in] */ Int32 resId)
{
    AutoPtr<IDrawable> drawable;
    ((Static*)(sStatic.Get()))->mResources->GetDrawable(resId, (IDrawable**)&drawable);
    return SetIconDrawable(drawable.Get());
}

ECode CMediaRouter::RouteGroup::RequestSetVolume(
    /* [in] */ Int32 volume)
{
    Int32 maxVol;
    GetVolumeMax(&maxVol);
    if (maxVol == 0) {
        return NOERROR;
    }

    Float scaledVolume = (Float) volume / maxVol;
    Int32 routeCount;
    GetRouteCount(&routeCount);
    for (Int32 i = 0; i < routeCount; i++) {
        AutoPtr<IMediaRouterRouteInfo> mr;
        GetRouteAt(i, (IMediaRouterRouteInfo**)&mr);
        AutoPtr<RouteInfo> route = (RouteInfo*)mr.Get();
        Int32 vol;
        route->GetVolumeMax(&vol);
        Int32 routeVol = (Int32) (scaledVolume * vol);
        route->RequestSetVolume(routeVol);
    }
    if (volume != mVolume) {
        mVolume = volume;
        DispatchRouteVolumeChanged(this);
    }
    return NOERROR;
}

ECode CMediaRouter::RouteGroup::RequestUpdateVolume(
    /* [in] */ Int32 direction)
{
    Int32 maxVol;
    GetVolumeMax(&maxVol);
    if (maxVol == 0) {
        return NOERROR;
    }

    Int32 routeCount;
    GetRouteCount(&routeCount);
    Int32 volume = 0;
    for (Int32 i = 0; i < routeCount; i++) {
        AutoPtr<IMediaRouterRouteInfo> r;
        GetRouteAt(i, (IMediaRouterRouteInfo**)&r);
        AutoPtr<RouteInfo> route = (RouteInfo*)r.Get();
        route->RequestUpdateVolume(direction);
        Int32 routeVol;
        route->GetVolume(&routeVol);
        if (routeVol > volume) {
            volume = routeVol;
        }
    }
    if (volume != mVolume) {
        mVolume = volume;
        DispatchRouteVolumeChanged(this);
    }

    return NOERROR;
}

void CMediaRouter::RouteGroup::MemberNameChanged(
    /* [in] */ RouteInfo* info,
    /* [in] */ ICharSequence* name)
{
    mUpdateName = TRUE;
    RouteUpdated();
}

void CMediaRouter::RouteGroup::MemberStatusChanged(
    /* [in] */ RouteInfo* info,
    /* [in] */ ICharSequence* status)
{
    SetStatusInt(status);
}

void CMediaRouter::RouteGroup::MemberVolumeChanged(
    /* [in] */ RouteInfo* info)
{
    UpdateVolume();
}

void CMediaRouter::RouteGroup::UpdateVolume()
{
    // A group always represents the highest component volume value.
    Int32 routeCount;
    GetRouteCount(&routeCount);
    Int32 volume = 0;
    for (Int32 i = 0; i < routeCount; i++) {
        AutoPtr<IMediaRouterRouteInfo> mr;
        GetRouteAt(i, (IMediaRouterRouteInfo**)&mr);
        Int32 routeVol;
        mr->GetVolume(&routeVol);
        if (routeVol > volume) {
            volume = routeVol;
        }
    }
    if (volume != mVolume) {
        mVolume = volume;
        DispatchRouteVolumeChanged(this);
    }
}

ECode CMediaRouter::RouteGroup::RouteUpdated()
{
    Int32 types = 0;
    Int32 count;
    mRoutes->GetSize(&count);
    if (count == 0) {
        // Don't keep empty groups in the router.
        return CMediaRouterHelper::RemoveRouteStatic(this);
    }

    Int32 maxVolume = 0;
    Boolean isLocal = TRUE;
    Boolean isFixedVolume = TRUE;
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IMediaRouterRouteInfo> obj;
        mRoutes->Get(i, (IMediaRouterRouteInfo**)&obj);
        AutoPtr<RouteInfo> route = (RouteInfo*)obj;
        types |= route->mSupportedTypes;
        Int32 routeMaxVolume;
        route->GetVolumeMax(&routeMaxVolume);
        if (routeMaxVolume > maxVolume) {
            maxVolume = routeMaxVolume;
        }
        Int32 vol;
        isLocal &= (route->GetPlaybackType(&vol), vol) == IMediaRouterRouteInfo::PLAYBACK_TYPE_LOCAL;
        Int32 handling;
        isFixedVolume &= (route->GetVolumeHandling(&handling), handling) == IMediaRouterRouteInfo::PLAYBACK_VOLUME_FIXED;
    }
    mPlaybackType = isLocal ? IMediaRouterRouteInfo::PLAYBACK_TYPE_LOCAL : IMediaRouterRouteInfo::PLAYBACK_TYPE_REMOTE;
    mVolumeHandling = isFixedVolume ? IMediaRouterRouteInfo::PLAYBACK_VOLUME_FIXED : IMediaRouterRouteInfo::PLAYBACK_VOLUME_VARIABLE;
    mSupportedTypes = types;
    mVolumeMax = maxVolume;
    AutoPtr<IInterface> obj;
    mRoutes->Get(0, (IInterface**)&obj);
    AutoPtr<IDrawable> dr;
    IMediaRouterRouteInfo::Probe(obj)->GetIconDrawable((IDrawable**)&dr);
    mIcon = count == 1 ? dr : NULL;
    return RouteInfo::RouteUpdated();
}

void CMediaRouter::RouteGroup::UpdateName()
{
    StringBuilder sb;
    Int32 count;
    mRoutes->GetSize(&count);
    AutoPtr<IInterface> obj;
    AutoPtr<RouteInfo> info;
    for (Int32 i = 0; i < count; i++) {
        mRoutes->Get(i, (IInterface**)&obj);
        info = NULL;
        info = (RouteInfo*)IMediaRouterRouteInfo::Probe(obj);
        // TODO: There's probably a much more correct way to localize this.
        if (i > 0) sb.Append(String(", "));
        sb.Append(info.mName);
    }
    sb.ToString(&mName);
    mUpdateName = FALSE;
}

ECode CMediaRouter::RouteGroup::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    String routeInfoStr;
    RouteInfo::ToString(&routeInfoStr);
    StringBuilder sb(routeInfoStr);
    sb.Append('[');
    Int32 count;
    mRoutes->GetSize(&count);
    AutoPtr<IInterface> obj;
    AutoPtr<IMediaRouterRouteInfo> rf;
    for (Int32 i = 0; i < count; i++) {
        if (i > 0) sb.Append(String(", "));
        rf = NULL;
        mRoutes->Get(i, (IInterface**)&obj);
        rf = NULL;
        rf = IMediaRouterRouteInfo::Probe(obj);
        sb.Append(rf.Get());
    }
    sb.Append(']');
    return sb.ToString(result);
}

//================================================================================
//                      CMediaRouter::RouteCategory
//================================================================================
CMediaRouter::RouteCategory::RouteCategory()
{}

CMediaRouter::RouteCategory::~RouteCategory()
{}

CAR_INTERFACE_IMPL(CMediaRouter::RouteCategory, Object, IMediaRouterRouteCategory)

ECode CMediaRouter::RouteCategory::constructor(
    /* [in] */ ICharSequence* name,
    /* [in] */ Int32 types,
    /* [in] */ Boolean groupable)
{
    mName = name;
    mTypes = types;
    mGroupable = groupable;
    return NOERROR;
}

ECode CMediaRouter::RouteCategory::constructor(
    /* [in] */ Int32 nameResId,
    /* [in] */ Int32 types,
    /* [in] */ Boolean groupable)
{
    mNameResId = nameResId;
    mTypes = types;
    mGroupable = groupable;
    return NOERROR;
}

ECode CMediaRouter::RouteCategory::GetName(
    /* [out] */ ICharSequence** result)
{
    VALIDATE_NOT_NULL(result);
    return GetName(((Static*)(sStatic.Get()))->mResources, result);
}

ECode CMediaRouter::RouteCategory::GetName(
    /* [in] */ IContext* context,
    /* [out] */ ICharSequence** result)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(result);
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    return GetName(res.Get(), result);
}

ECode CMediaRouter::RouteCategory::GetName(
    /* [in] */ IResources* res,
    /* [out] */ ICharSequence** result)
{
    VALIDATE_NOT_NULL(result);
    if (mNameResId != 0) {
        return res->GetText(mNameResId, result);
    }
    *result = mName.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::RouteCategory::GetRoutes(
    /* [in] */ IList* outList,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result);

    if (outList == NULL) {
        AutoPtr<IArrayList> al;
        CArrayList::New((IArrayList**)&al);
        outList = IList::Probe(al);
    } else {
        outList->Clear();
    }

    Int32 count = GetRouteCountStatic();
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IMediaRouterRouteInfo> route = GetRouteAtStatic(i);
        if (route->mCategory.Equals(IMediaRouterRouteCategory::Probe(this))) {
            outList->Add(route.Get());
        }
    }
    *result = outList;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::RouteCategory::GetSupportedTypes(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result);
    *result = mTypes;
    return NOERROR;
}

ECode CMediaRouter::RouteCategory::IsGroupable(
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = mGroupable;
    return NOERROR;
}

ECode CMediaRouter::RouteCategory::IsSystem(
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = mIsSystem;
    return NOERROR;
}

ECode CMediaRouter::RouteCategory::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = String("RouteCategory{ name=") + mName + String(" types=") + TypesToString(mTypes) +
            String(" groupable=") + mGroupable + String(" }");
    return NOERROR;
}

//================================================================================
//                      CMediaRouter::CallbackInfo
//================================================================================
CMediaRouter::CallbackInfo::CallbackInfo(
    /* [in] */ IMediaRouterCallback* cb,
    /* [in] */ Int32 type,
    /* [in] */ Int32 flags,
    /* [in] */ CMediaRouter* router)
    : mCb(cb)
    , mTypes(type)
    , mFlags(flags)
    , mRouter(router)
{}

CMediaRouter::CallbackInfo::~CallbackInfo()
{}

CAR_INTERFACE_IMPL(CMediaRouter::CallbackInfo, Object, IMediaRouterRouteCallbackInfo)

ECode CMediaRouter::CallbackInfo::FilterRouteEvent(
    /* [in] */ IMediaRouterRouteInfo* route,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<RouteInfo> r = (RouteInfo*)route;
    return FilterRouteEvent(r->mSupportedTypes, result);
}

ECode CMediaRouter::CallbackInfo::FilterRouteEvent(
    /* [in] */ Int32 supportedTypes,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (mFlags & IMediaRouter::CALLBACK_FLAG_UNFILTERED_EVENTS) != 0
            || (type & supportedTypes) != 0;
    return NOERROR;
}

//================================================================================
//                      CMediaRouter::SimpleCallback
//================================================================================
CMediaRouter::SimpleCallback::SimpleCallback()
{}

CMediaRouter::SimpleCallback::~SimpleCallback()
{}

ECode CMediaRouter::SimpleCallback::constructor()
{
    return NOERROR;
}

CAR_INTERFACE_IMPL_2(CMediaRouter::SimpleCallback, Object, IMediaRouterCallback, IMediaRouterSimpleCallback)

ECode CMediaRouter::SimpleCallback::OnRouteSelected(
    /* [in] */ IMediaRouter* router,
    /* [in] */ Int32 type,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    return NOERROR;
}

ECode CMediaRouter::SimpleCallback::OnRouteUnselected(
    /* [in] */ IMediaRouter* router,
    /* [in] */ Int32 type,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    return NOERROR;
}

ECode CMediaRouter::SimpleCallback::OnRouteAdded(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    return NOERROR;
}

ECode CMediaRouter::SimpleCallback::OnRouteRemoved(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    return NOERROR;
}

ECode CMediaRouter::SimpleCallback::OnRouteChanged(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    return NOERROR;
}

ECode CMediaRouter::SimpleCallback::OnRouteGrouped(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IMediaRouterRouteInfo* info,
    /* [in] */ IMediaRouterRouteGroup* group,
    /* [in] */ Int32 index)
{
    return NOERROR;
}

ECode CMediaRouter::SimpleCallback::OnRouteUngrouped(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IMediaRouterRouteInfo* info,
    /* [in] */ IMediaRouterRouteGroup* group)
{
    return NOERROR;
}

ECode CMediaRouter::SimpleCallback::OnRouteVolumeChanged(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    return NOERROR;
}

//------------------------
CAR_INTERFACE_IMPL(CMediaRouter::VolumeCallbackInfo, Object, IMediaRouterVolumeCallbackInfo)

CMediaRouter::VolumeCallbackInfo::VolumeCallbackInfo()
{}

CMediaRouter::VolumeCallbackInfo::~VolumeCallbackInfo()
{}

ECode CMediaRouter::VolumeCallbackInfo::constructor(
    /* [in] */ IMediaRouterVolumeCallback* vcb,
    /* [in] */ IMediaRouterRouteInfo* route)
{
    mVcb = vcb;
    mRoute = route;
    return NOERROR;
}

ECode CMediaRouter::VolumeCallbackInfo::SetVolumeCallback(
    /* [in] */ IMediaRouterVolumeCallback* vcb)
{
    mVcb = vcb;
    return NOERROR;
}

ECode CMediaRouter::VolumeCallbackInfo::GetVolumeCallback(
    /* [out] */ IMediaRouterVolumeCallback** vcb)
{
    VALIDATE_NOT_NULL(vcb);
    *vcb = mVcb;
    REFCOUNT_ADD(*vcb);
    return NOERROR;
}

ECode CMediaRouter::VolumeCallbackInfo::SetRouteInfo(
    /* [in] */ IMediaRouterRouteInfo* route)
{
    mRoute = route;
    return NOERROR;
}

ECode CMediaRouter::VolumeCallbackInfo::GetRouteInfo(
    /* [out] */ IMediaRouterRouteInfo** route)
{
    VALIDATE_NOT_NULL(route);
    *route = mRoute;
    REFCOUNT_ADD(*route);
    return NOERROR;
}

//------------------------

ECode CMediaRouter::VolumeChangeReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String tempText;
    intent->GetAction(&tempText);
    if (tempText.Equals(IAudioManager::VOLUME_CHANGED_ACTION))
    {
        Int32 streamType;
        intent->GetInt32Extra(IAudioManager::EXTRA_VOLUME_STREAM_TYPE, -1, &streamType);
        if (streamType != IAudioManager::STREAM_MUSIC) {
            return NOERROR;
        }

        Int32 newVolume;
        intent->GetInt32Extra(IAudioManager::EXTRA_VOLUME_STREAM_VALUE, 0, &newVolume);
        Int32 oldVolume;
        intent->GetInt32Extra(IAudioManager::EXTRA_PREV_VOLUME_STREAM_VALUE, 0, &oldVolume);
        if (newVolume != oldVolume) {
            SystemVolumeChanged(newVolume);
        }
    }
    return NOERROR;
}

//------------------------

ECode CMediaRouter::WifiDisplayStatusChangedReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String tempText;
    intent->GetAction(&tempText);
    if (tempText.Equals(IDisplayManager::ACTION_WIFI_DISPLAY_STATUS_CHANGED)) {
        AutoPtr<IParcelable> parcelable;
        intent->GetParcelableExtra(IDisplayManager::EXTRA_WIFI_DISPLAY_STATUS,
            (IParcelable**)&parcelable);
        UpdateWifiDisplayStatus((IWifiDisplayStatus*)&parcelable);
    }
    return NOERROR;
}

//------------------------

CMediaRouter::CMediaRouter()
{}

ECode CMediaRouter::constructor(
    /* [in] */ IContext* context)
{
    {
        AutoLock lock(mStaticClass);
        if (sStatic == NULL) {
            AutoPtr<IContext> appContext;
            context->GetApplicationContext((IContext**)&appContext);
            sStatic = new Static(appContext, this);
            ((Static*)(sStatic.Get()))->StartMonitoringRoutes(appContext);
        }
    }
    return NOERROR;
}

ECode CMediaRouter::GetSystemAudioRoute(
    /* [out] */ IMediaRouterRouteInfo** result)
{
    VALIDATE_NOT_NULL(result);

    *result = ((Static*)(sStatic.Get()))->mDefaultAudioVideo;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::GetSystemAudioCategory(
    /* [out] */ IMediaRouterRouteCategory** result)
{
    VALIDATE_NOT_NULL(result);

    *result = ((Static*)(sStatic.Get()))->mSystemCategory;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::GetSelectedRoute(
    /* [in] */ Int32 type,
    /* [out] */ IMediaRouterRouteInfo** result)
{
    VALIDATE_NOT_NULL(result);
    Int32 temp;
    if (((Static*)(sStatic.Get()))->mSelectedRoute != NULL &&
            (((Static*)(sStatic.Get()))->mSelectedRoute->GetSupportedTypes(&temp) & type) != 0) {
        // If the selected route supports any of the types supplied, it's still considered
        // 'selected' for that type.
        *result = ((Static*)(sStatic.Get()))->mSelectedRoute;
    } else if (type == ROUTE_TYPE_USER) {
        // The caller specifically asked for a user route and the currently selected route
        // doesn't qualify.
        *result = NULL;
    }
    // If the above didn't match and we're not specifically asking for a user route,
    // consider the default selected.
    *result = ((Static*)(sStatic.Get()))->mDefaultAudioVideo;

    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::AddCallback(
    /* [in] */ Int32 types,
    /* [in] */ IMediaRouterCallback* cb)
{
    List< AutoPtr<CallbackInfo> >::Iterator it = ((Static*)(sStatic.Get()))->mCallbacks.Begin();
    for (; it != ((Static*)(sStatic.Get()))->mCallbacks.End(); ++it) {
        AutoPtr<CallbackInfo> info = *it;
        if (cb == info->mCb) {
            info->mType |= types;
            return NOERROR;
        }
    }
    AutoPtr<CallbackInfo> tempCallbackInfo = new CallbackInfo(cb, types, this);
    ((Static*)(sStatic.Get()))->mCallbacks.PushBack(tempCallbackInfo);
    return NOERROR;
}

ECode CMediaRouter::RemoveCallback(
    /* [in] */ IMediaRouterCallback* cb)
{
    List< AutoPtr<CallbackInfo> >::Iterator it = ((Static*)(sStatic.Get()))->mCallbacks.Begin();
    for (; it != ((Static*)(sStatic.Get()))->mCallbacks.End(); ++it) {
        if (cb == (*it)->mCb) {
            ((Static*)(sStatic.Get()))->mCallbacks.Erase(it);
            return NOERROR;
        }
    }

    Logger::W(TAG, "removeCallback(" /*+ cb +*/ "): callback not registered");
    return NOERROR;
}

ECode CMediaRouter::SelectRoute(
    /* [in] */ Int32 types,
    /* [in] */ IMediaRouterRouteInfo* route)
{
    // Applications shouldn't programmatically change anything but user routes.
    types &= ROUTE_TYPE_USER;
    SelectRouteStatic(types, route);
    return NOERROR;
}

ECode CMediaRouter::SelectRouteInt(
    /* [in] */ Int32 types,
    /* [in] */ IMediaRouterRouteInfo* route)
{
    SelectRouteStatic(types, route);
    return NOERROR;
}

ECode CMediaRouter::AddUserRoute(
    /* [in] */ IMediaRouterUserRouteInfo* info)
{
    AddRouteStatic((IMediaRouterRouteInfo*)info);
    return NOERROR;
}

ECode CMediaRouter::AddRouteInt(
    /* [in] */ IMediaRouterRouteInfo* info)
{
    AddRouteStatic(info);
    return NOERROR;
}

ECode CMediaRouter::RemoveUserRoute(
    /* [in] */ IMediaRouterUserRouteInfo* info)
{
    RemoveRoute((IMediaRouterRouteInfo*)info);
    return NOERROR;
}

ECode CMediaRouter::ClearUserRoutes()
{
    for (Int32 i = 0; i < ((Static*)(sStatic.Get()))->mRoutes.GetSize(); i++) {
        AutoPtr<IMediaRouterRouteInfo> info = ((Static*)(sStatic.Get()))->mRoutes[i];
        // TODO Right now, RouteGroups only ever contain user routes.
        // The code below will need to change if this assumption does.
        if ((IMediaRouterUserRouteInfo::Probe(info) != NULL) || (IMediaRouterRouteGroup::Probe(info) != NULL)) {
            RemoveRouteAt(i);// sStatic->mRoutes is changed in this function!!!
            i--;
        }
    }
    return NOERROR;
}

ECode CMediaRouter::RemoveRouteInt32(
    /* [in] */ IMediaRouterRouteInfo* info)
{
    return RemoveRouteStatic(info);
}

ECode CMediaRouter::GetCategoryCount(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ((Static*)(sStatic.Get()))->mCategories.GetSize();
    return NOERROR;
}

ECode CMediaRouter::GetCategoryAt(
    /* [in] */ Int32 index,
    /* [out] */ IMediaRouterRouteCategory** result)
{
    VALIDATE_NOT_NULL(result);
    *result = ((Static*)(sStatic.Get()))->mCategories[index];
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::GetRouteCount(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ((Static*)(sStatic.Get()))->mRoutes.GetSize();
    return NOERROR;
}

ECode CMediaRouter::GetRouteAt(
    /* [in] */ Int32 index,
    /* [out] */ IMediaRouterRouteInfo** result)
{
    VALIDATE_NOT_NULL(result);

    *result = ((Static*)(sStatic.Get()))->mRoutes[index];
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::CreateUserRoute(
    /* [in] */ IMediaRouterRouteCategory* category,
    /* [out] */ IMediaRouterUserRouteInfo** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IMediaRouterUserRouteInfo> userRouteInfo;
    CUserRouteInfo::New(category, (IMediaRouterUserRouteInfo**)&userRouteInfo);
    *result = userRouteInfo;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::CreateRouteCategory(
    /* [in] */ ICharSequence* name,
    /* [in] */ Boolean isGroupable,
    /* [out] */ IMediaRouterRouteCategory** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IMediaRouterRouteCategory> routeCategory;
    CRouteCategory::New(name, ROUTE_TYPE_USER, isGroupable, (IMediaRouterRouteCategory**)&routeCategory);
    *result = routeCategory;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::CreateRouteCategory(
    /* [in] */ Int32 nameResId,
    /* [in] */ Boolean isGroupable,
    /* [out] */ IMediaRouterRouteCategory** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IMediaRouterRouteCategory> routeCategory;
    CRouteCategory::New(nameResId, ROUTE_TYPE_USER, isGroupable, (IMediaRouterRouteCategory**)&routeCategory);
    *result = routeCategory;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

String CMediaRouter::TypesToString(
    /* [in] */ Int32 types)
{
    StringBuilder result;

    if ((types & ROUTE_TYPE_LIVE_AUDIO) != 0) {
        result += "ROUTE_TYPE_LIVE_AUDIO ";
    }
    if ((types & ROUTE_TYPE_LIVE_VIDEO) != 0) {
        result += "ROUTE_TYPE_LIVE_VIDEO ";
    }
    if ((types & ROUTE_TYPE_USER) != 0) {
        result += "ROUTE_TYPE_USER ";
    }
    String tempText;
    result.ToString(&tempText);
    return tempText;
}

ECode CMediaRouter::SelectRouteStatic(
    /* [in] */ Int32 types,
    /* [in] */ IMediaRouterRouteInfo* route,
    /* [in] */ Boolean isexplicit)
{
    AutoPtr<IMediaRouterRouteInfo> oldRoute = ((Static*)(sStatic.Get()))->mSelectedRoute;
    if (route == oldRoute) {
        return NOERROR;
    }
    Int32 tempValue;
    route->GetSupportedTypes(&tempValue);
    if ((tempValue & types) == 0) {
        Logger::W(TAG, String("selectRoute ignored; cannot select route with supported types ") +
                TypesToString(tempValue) + String(" into route types ") +
                TypesToString(types));
        return NOERROR;
    }

    AutoPtr<IMediaRouterRouteInfo> btRoute = ((Static*)(sStatic.Get()))->mBluetoothA2dpRoute;
    if (btRoute != NULL && (types & ROUTE_TYPE_LIVE_AUDIO) != 0 &&
            (route == btRoute || route == ((Static*)(sStatic.Get()))->mDefaultAudioVideo)) {
        ECode ec;
//        try {
            ec = ((Static*)(sStatic.Get()))->mAudioService->SetBluetoothA2dpOn(route == btRoute);
//        } catch (RemoteException e) {
            if (FAILED(ec)) {
                Logger::E(TAG, "Error changing Bluetooth A2DP state"/*, e*/);
            }
//        }
    }

    AutoPtr<IWifiDisplay> activeDisplay;
    AutoPtr<IWifiDisplayStatus> wifiDisplayStatus;
    ((Static*)(sStatic.Get()))->mDisplayService->GetWifiDisplayStatus((IWifiDisplayStatus**)&wifiDisplayStatus);
    wifiDisplayStatus->GetActiveDisplay((IWifiDisplay**)&activeDisplay);

    String tempText1, tempText2;
    Boolean oldRouteHasAddress = oldRoute != NULL && (oldRoute->GetDeviceAddress(&tempText1), tempText1) != NULL;
    Boolean newRouteHasAddress = route != NULL && (route->GetDeviceAddress(&tempText2), tempText2) != NULL;

    if (activeDisplay != NULL || oldRouteHasAddress || newRouteHasAddress) {
        if (newRouteHasAddress && !MatchesDeviceAddress(activeDisplay, route)) {
            String tempText;
            route->GetDeviceAddress(&tempText);
            ((Static*)(sStatic.Get()))->mDisplayService->ConnectWifiDisplay(tempText);
        }
        else if (activeDisplay != NULL && !newRouteHasAddress) {
            ((Static*)(sStatic.Get()))->mDisplayService->DisconnectWifiDisplay();
        }
    }

    if (oldRoute != NULL) {
        // TODO filter types properly
        Int32 tempValue;
        oldRoute->GetSupportedTypes(&tempValue);
        DispatchRouteUnselected(types & tempValue, oldRoute);
    }
    ((Static*)(sStatic.Get()))->mSelectedRoute = route;
    if (route != NULL) {
        // TODO filter types properly
        Int32 tempValue;
        route->GetSupportedTypes(&tempValue);
        DispatchRouteSelected(types & tempValue, route);
    }
    return NOERROR;
}

ECode CMediaRouter::SelectDefaultRouteStatic()
{
    // TODO: Be smarter about the route types here; this selects for all valid.
    Boolean flag = FALSE;
    ((Static*)(sStatic.Get()))->IsBluetoothA2dpOn(&flag);
    if ((!IObject::Equals(((Static*)(sStatic.Get()))->mSelectedRoute.Get(), ((Static*)(sStatic.Get()))->mBluetoothA2dpRoute))
            && ((Static*)(sStatic.Get()))->mBluetoothA2dpRoute != NULL && flag) {
        SelectRouteStatic(ROUTE_TYPE_ANY, ((Static*)(sStatic.Get()))->mBluetoothA2dpRoute, FALSE);
    } else {
        SelectRouteStatic(ROUTE_TYPE_ANY, ((Static*)(sStatic.Get()))->mDefaultAudioVideo, FALSE);
    }
    return NOERROR;
}

ECode CMediaRouter::MatchesDeviceAddress(
    /* [in] */ IWifiDisplay* display,
    /* [in] */ IMediaRouterRouteInfo* info,
    /* [out] */ Boolean* result)
{
    String tempText1;
    Boolean routeHasAddress = info != NULL && (info->GetDeviceAddress(&tempText1), tempText1) != NULL;
    if (display == NULL && !routeHasAddress) {
        *result = TRUE;
        return NOERROR;
    }

    if (display != NULL && routeHasAddress) {
        String tempText2;
        display->GetDeviceAddress(&tempText1);
        info->GetDeviceAddress(&tempText2);
        *result = tempText1.Equals(tempText2);
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CMediaRouter::AddRouteStatic(
    /* [in] */ IMediaRouterRouteInfo* info)
{
    if (info == NULL) return NOERROR;

    AutoPtr<IMediaRouterRouteCategory> cat;
    info->GetCategory((IMediaRouterRouteCategory**)&cat);
    List<AutoPtr<IMediaRouterRouteCategory> >::Iterator it = Find(((Static*)(sStatic.Get()))->mCategories.Begin(), ((Static*)(sStatic.Get()))->mCategories.End(), cat);
    if (it == ((Static*)(sStatic.Get()))->mCategories.End()) {
        (((Static*)(sStatic.Get()))->mCategories).PushBack(cat);
    }

    Boolean tempState;
    if ((cat->IsGroupable(&tempState), tempState) && !(IMediaRouterRouteGroup::Probe(info) != NULL)) {
        // Enforce that any added route in a groupable category must be in a group.
        AutoPtr<IMediaRouterRouteGroup> group;
        AutoPtr<IMediaRouterRouteCategory> routeCategory;
        info->GetCategory((IMediaRouterRouteCategory**)&routeCategory);
        CRouteGroup::New(routeCategory, (IMediaRouterRouteGroup**)&group);
        Int32 tempValue;
        info->GetSupportedTypes(&tempValue);
        ((IMediaRouterRouteInfo*)&group)->SetSupportedTypes(tempValue);
        (((Static*)(sStatic.Get()))->mRoutes).PushBack((RouteInfo*)(group.Get()));
        DispatchRouteAdded((IMediaRouterRouteInfo*)&group);
        group->AddRoute(info);

        info = (IMediaRouterRouteInfo*)&group;
    } else {
        (((Static*)(sStatic.Get()))->mRoutes).PushBack(info);
        DispatchRouteAdded(info);
    }
    return NOERROR;
}

ECode CMediaRouter::RemoveRouteStatic(
    /* [in] */ IMediaRouterRouteInfo* info)
{
    List< AutoPtr<IMediaRouterRouteInfo> >::Iterator it_ = Find(((Static*)(sStatic.Get()))->mRoutes.Begin(), ((Static*)(sStatic.Get()))->mRoutes.End(), info);
    ((Static*)(sStatic.Get()))->mRoutes.Erase(it_);

    AutoPtr<IMediaRouterRouteCategory> removingCat;
    info->GetCategory((IMediaRouterRouteCategory**)&removingCat);
    Boolean found = FALSE;
    List< AutoPtr<IMediaRouterRouteInfo> >::Iterator it = ((Static*)(sStatic.Get()))->mRoutes.Begin();
    AutoPtr<IMediaRouterRouteCategory> cat;
    for (; it != ((Static*)(sStatic.Get()))->mRoutes.End(); ++it) {
        cat = NULL;
        (*it)->GetCategory((IMediaRouterRouteCategory**)&cat);
        if (IObject::Equals(removingCat, cat)) {
            found = TRUE;
            break;
        }
    }

    Boolean isSelected = FALSE;
    info->IsSelected(&isSelected);
    if (isSelected) {
        // Removing the currently selected route? Select the default before we remove it.
        SelectDefaultRouteStatic();
    }
    if (!found) {
        List< AutoPtr<IMediaRouterRouteCategory> >::Iterator it =
        Find((((Static*)(sStatic.Get()))->mCategories).Begin(), (((Static*)(sStatic.Get()))->mCategories).End(), removingCat);
        ((Static*)(sStatic.Get()))->mCategories.Erase(it);
    }
        DispatchRouteRemoved(info);
    }
    return NOERROR;
}

ECode CMediaRouter::GetRouteCountStatic(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (((Static*)sStatic.Get())->mRoutes).GetSize();
    return NOERROR;
}

ECode CMediaRouter::GetRouteAtStatic(
    /* [in] */ Int32 index,
    /* [out] */ IMediaRouterRouteInfo** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IMediaRouterRouteInfo> r = (((Static*)(sStatic.Get()))->mRoutes)[index];
    *result = r.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaRouter::UpdateRoute(
    /* [in] */ IMediaRouterRouteInfo* info)
{
    DispatchRouteChanged(info);
    return NOERROR;
}

ECode CMediaRouter::DispatchRouteSelected(
    /* [in] */ Int32 type,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    List<AutoPtr<CallbackInfo> >::Iterator it = ((Static*)(sStatic.Get()))->mCallbacks.Begin();
    Boolean flag = FALSE;
    AutoPtr<IMediaRouterRouteCallbackInfo> cbi_;
    AutoPtr<CallbackInfo> cbi;
    for (; it != ((Static*)(sStatic.Get()))->mCallbacks.End(); ++it) {
        cbi_ = NULL;
        cbi_ = (*it);
        cbi = (CallbackInfo*)(cbi_.Get());
        cbi_->FilterRouteEvent(info, &flag);
        if (flag) {
            cbi->mCb->OnRouteSelected((cbi->mRouter).Get(), type, info);
        }
    }
    return NOERROR;
}

ECode CMediaRouter::DispatchRouteUnselected(
    /* [in] */ Int32 type,
    /* [in] */ IMediaRouterRouteInfo* info)
{
    List<AutoPtr<IMediaRouterRouteCallbackInfo> >::Iterator it;
    it = (((Static*)(sStatic.Get()))->mCallbacks).Begin();
    Boolean flag = FALSE;
    AutoPtr<CallbackInfo> cbi;
    AutoPtr<IMediaRouterRouteCallbackInfo> cbi_;
    AutoPtr<IMediaRouterCallback> cb;
    for (; it != (((Static*)(sStatic.Get()))->mCallbacks).End(); ++it) {
        cbi = NULL;
        cbi_ = NULL;
        cbi_ = *it;
        cbi_->FilterRouteEvent(info, &flag);
        cbi = NULL;
        cbi = (CallbackInfo*)(cbi_.Get());
        if (flag) {
            cb = NULL;
            cb = cbi->mCb;
            cb->OnRouteUnselected((IMediaRouter*)(cbi->mRouter), type, info);
        }
    }
    return NOERROR;
}

ECode CMediaRouter::DispatchRouteChanged(
    /* [in] */ IMediaRouterRouteInfo* info,
    /* [in] */ Int32 oldSupportedTypes)
{
    Int32 newSupportedTypes = ((RouteInfo*)info)->mSupportedTypes;

    List<AutoPtr<IMediaRouterRouteCallbackInfo> >::Iterator it;
    it = (((Static*)(sStatic.Get()))->mCallbacks).Begin();
    Boolean oldVisibility = FALSE;
    Boolean newVisibility = FALSE;
    AutoPtr<IMediaRouterRouteCallbackInfo> cbi_;
    AutoPtr<CallbackInfo> cbi;
    Boolean flag = FALSE;
    for (; it != (((Static*)(sStatic.Get()))->mCallbacks).End(); ++it) {
        // Reconstruct some of the history for callbacks that may not have observed
        // all of the events needed to correctly interpret the current state.
        // FIXME: This is a strong signal that we should deprecate route type filtering
        // completely in the future because it can lead to inconsistencies in
        // applications.
        cbi_ = NULL;
        cbi = NULL;
        cbi_ = *it;
        cbi = (CallbackInfo*)(cbi_.Get());
        cbi->FilterRouteEvent(oldSupportedTypes, &oldVisibility);
        cbi->FilterRouteEvent(newSupportedTypes, &newVisibility);
        info->IsSelected(&flag);
        if (!oldVisibility && newVisibility) {
            cbi->mCb->OnRouteAdded((cbi->mRouter).Get(), info);
            if (flag) {
                cbi->mCb->OnRouteSelected((cbi->mRouter).Get(), newSupportedTypes, info);
            }
        }
        if (oldVisibility || newVisibility) {
            cbi->mCb->OnRouteChanged((cbi->mRouter).Get(), info);
        }
        if (oldVisibility && !newVisibility) {
            if (flag) {
                cbi->mCb->OnRouteUnselected((cbi->mRouter).Get(), oldSupportedTypes, info);
            }
            cbi->mCb->OnRouteRemoved((cbi->mRouter).Get(), info);
        }
    }
    return NOERROR;
}

ECode CMediaRouter::DispatchRouteChanged(
    /* [in] */ IMediaRouterRouteInfo* info)
{
    return DispatchRouteChanged(info, ((RouteInfo*)info)->mSupportedTypes);
}

ECode CMediaRouter::DispatchRouteAdded(
    /* [in] */ IMediaRouterRouteInfo* info)
{
    List< AutoPtr<CallbackInfo> >::Iterator it = ((Static*)sStatic)->mCallbacks.Begin();
    AutoPtr<CallbackInfo> cbi;
    Boolean flag = FALSE;
    for (; it != ((Static*)(sStatic.Get()))->mCallbacks.End(); ++it) {
        cbi = NULL;
        cbi = (*it);
        cbi->FilterRouteEvent(info, &flag);
        if (flag) {
            cbi->mCb->OnRouteAdded((cbi->mRouter).Get(), info);
        }
    }
    return NOERROR;
}

ECode CMediaRouter::DispatchRouteRemoved(
    /* [in] */ IMediaRouterRouteInfo* info)
{
    List< AutoPtr<CallbackInfo> >::Iterator it = ((Static*)(sStatic.Get()))->mCallbacks.Begin();
    AutoPtr<CallbackInfo> cbi;
    Boolean flag = FALSE;
    for (; it != ((Static*)(sStatic.Get()))->mCallbacks.End(); ++it) {
        cbi = NULL;
        cbi = (*it);
        cbi->FilterRouteEvent(info, &flag);
        if (flag) {
            cbi->mCb->OnRouteRemoved((cbi->mRouter).Get(), info);
        }
    }
    return NOERROR;
}

CMediaRouter::DispatchRouteGrouped(
    /* [in] */ IMediaRouterRouteInfo* info,
    /* [in] */ IMediaRouterRouteGroup* group,
    /* [in] */ Int32 index)
{
    List< AutoPtr<CallbackInfo> >::Iterator it = ((Static*)(sStatic.Get()))->mCallbacks.Begin();
    AutoPtr<CallbackInfo> cbi;
    Boolean flag = FALSE;
    for (; it != ((Static*)(sStatic.Get()))->mCallbacks.End(); ++it) {
        cbi = NULL;
        cbi = (*it);
        cbi->FilterRouteEvent(IMediaRouterRouteInfo::Probe(group), &flag);
        if (flag) {
            cbi->mCb->OnRouteGrouped((cbi->mRouter).Get(), info, group, index);
        }
    }
    return NOERROR;
}

ECode CMediaRouter::DispatchRouteUngrouped(
    /* [in] */ IMediaRouterRouteInfo* info,
    /* [in] */ IMediaRouterRouteGroup* group)
{
    List< AutoPtr<CallbackInfo> >::Iterator it = ((Static*)(sStatic.Get()))->mCallbacks.Begin();
    AutoPtr<CallbackInfo> cbi;
    Boolean flag = FALSE;
    for (; it != ((Static*)(sStatic.Get()))->mCallbacks.End(); ++it) {
        cbi = NULL;
        cbi = (*it);
        cbi->FilterRouteEvent(IMediaRouterRouteInfo::Probe(group), &flag);
        if (flag) {
            cbi->mCb->OnRouteUngrouped((cbi->mRouter).Get(), info, group);
        }
    }
    return NOERROR;
}

ECode CMediaRouter::DispatchRouteVolumeChanged(
    /* [in] */ IMediaRouterRouteInfo* info)
{
    List< AutoPtr<CallbackInfo> >::Iterator it = ((Static*)(sStatic.Get()))->mCallbacks.Begin();
    AutoPtr<CallbackInfo> cbi;
    Boolean flag = FALSE;
    for (; it != ((Static*)(sStatic.Get()))->mCallbacks.End(); ++it) {
        cbi = NULL;
        cbi = (*it);
        cbi->FilterRouteEvent(info, &flag);
        if (flag) {
            cbi->mCb->OnRouteVolumeChanged((cbi->mRouter).Get(), info);
        }
    }
    return NOERROR;
}

ECode CMediaRouter::DispatchRoutePresentationDisplayChanged(
    /* [in] */ IMediaRouterRouteInfo* info)
{
    List< AutoPtr<CallbackInfo> >::Iterator it = ((Static*)(sStatic.Get()))->mCallbacks.Begin();
    AutoPtr<CallbackInfo> cbi;
    Boolean flag = FALSE;
    for (; it != ((Static*)(sStatic.Get()))->mCallbacks.End(); ++it) {
        cbi = NULL;
        cbi = (*it);
        cbi->FilterRouteEvent(info, &flag);
        if (flag) {
            cbi->mCb->OnRoutePresentationDisplayChanged((cbi->mRouter).Get(), info);
        }
    }
    return NOERROR;
}

ECode CMediaRouter::SystemVolumeChanged(
    /* [in] */ Int32 newValue)
{
    AutoPtr<IMediaRouterRouteInfo> selectedRoute = ((Static*)(sStatic.Get()))->mSelectedRoute;
    if (selectedRoute == NULL) {
        return NOERROR;
    }

    if (selectedRoute == ((Static*)(sStatic.Get()))->mBluetoothA2dpRoute ||
            selectedRoute == ((Static*)(sStatic.Get()))->mDefaultAudioVideo) {
        DispatchRouteVolumeChanged(selectedRoute);
    }
    else if (((Static*)(sStatic.Get()))->mBluetoothA2dpRoute != NULL) {
//        try {
            Boolean tempState;
            DispatchRouteVolumeChanged((((Static*)(sStatic.Get()))->mAudioService->IsBluetoothA2dpOn(&tempState), tempState) ?
                    ((Static*)(sStatic.Get()))->mBluetoothA2dpRoute : ((Static*)(sStatic.Get()))->mDefaultAudioVideo);
//        } catch (RemoteException e) {
            Logger::E(TAG, "Error checking Bluetooth A2DP state to report volume change"/*, e*/);
//        }
    } else {
        DispatchRouteVolumeChanged(((Static*)(sStatic.Get()))->mDefaultAudioVideo);
    }
}

ECode CMediaRouter::UpdateWifiDisplayStatus(
    /* [in] */ IWifiDisplayStatus* newStatus)
{
    assert(0 && "todo");
    //todo
}

ECode CMediaRouter::GetWifiDisplayStatusCode(
    /* [in] */ IWifiDisplay* d,
    /* [in] */ IWifiDisplayStatus* wfdStatus,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 newStatus;
    wfdStatus->GetScanState(&newStatus);
    Boolean flag = FALSE;
    if (newStatus == IWifiDisplayStatus::SCAN_STATE_SCANNING) {
        newStatus = IMediaRouterRouteInfo::STATUS_SCANNING;
    } else if ((d->IsAvailable(&flag), flag)) {
        newStatus = (d->CanConnect(&flag), flag) ?
                IMediaRouterRouteInfo::STATUS_AVAILABLE : IMediaRouterRouteInfo::STATUS_IN_USE;
    } else {
        newStatus = IMediaRouterRouteInfo::STATUS_NOT_AVAILABLE;
    }

    AutoPtr<IWifiDisplay> display;
    wfdStatus->GetActiveDisplay((IWifiDisplay**)&display);
    d->Equals(display, &flag);
    if (flag) {
        Int32 activeState;
        wfdStatus->GetActiveDisplayState(&activeState);
        switch (activeState) {
            case 2:
                newStatus = IMediaRouterRouteInfo::STATUS_CONNECTED;
                break;
            case 1:
                newStatus = IMediaRouterRouteInfo::STATUS_CONNECTING;
                break;
            case 0:
                Logger::E(TAG, "Active display is not connected!");
                break;
        }
    }

    *result = newStatus;
    return NOERROR;
}

ECode CMediaRouter::IsWifiDisplayEnabled(
    /* [in] */ IWifiDisplay* d,
    /* [in] */ IWifiDisplayStatus* wfdStatus,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean isAvailable = FALSE;
    Boolean canConnect = FALSE;
    d->IsAvailable(&isAvailable);
    d->CanConnect(&canConnect);
    AutoPtr<IWifiDisplay> wd;
    wfdStatus->GetActiveDisplay((IWifiDisplay**)&wd);
    Boolean flag = FALSE;
    d->Equals(wd.Get(), &flag);
    *result = isAvailable && (canConnect || flag);
    return NOERROR;
}

ECode CMediaRouter::MakeWifiDisplayRoute(
    /* [in] */ IWifiDisplay* display,
    /* [in] */ IWifiDisplayStatus* wfdStatus,
    /* [out] */ IMediaRouterRouteInfo** result);
{
    AutoPtr<RouteInfo> newRoute = new RouteInfo();
    newRoute->constructor(((Static*)(sStatic.Get()))->mSystemCategory);

    String tempText;
    display->GetDeviceAddress(&tempText);
    newRoute->SetDeviceAddress(tempText);

    newRoute->SetSupportedTypes(ROUTE_TYPE_LIVE_AUDIO | ROUTE_TYPE_LIVE_VIDEO);
    newRoute->SetVolumeHandling(IMediaRouterRouteInfo::PLAYBACK_VOLUME_FIXED);
    newRoute->SetPlaybackType(IMediaRouterRouteInfo::PLAYBACK_TYPE_REMOTE);

    Boolean flag = FALSE;
    Int32 rsCode;
    GetWifiDisplayStatusCode(display, wfdStatus, &rsCode);
    newRoute->SetRealStatusCode(rsCode, &flag);
    newRoute->mEnabled = isWifiDisplayEnabled(display, wfdStatus);
    display->GetFriendlyDisplayName(&tempText);
    newRoute->SetName(StringUtils::ParseCharSequence(tempText).Get());
    AutoPtr<ICharSequence> cs;
    sStatic->mResources->GetText(R::string::wireless_display_route_description, (ICharSequence**)&cs);
    newRoute->SetDescription(cs.Get());
    newRoute->UpdatePresentationDisplay(&flag);
    *result = IMediaRouterRouteInfo::Probe(newRoute);
    REFCOUNT_ADD(*result);
    return NOERROR;
}

void CMediaRouter::UpdateWifiDisplayRoute(
    /* [in] */ IMediaRouterRouteInfo* route,
    /* [in] */ IWifiDisplay* display,
    /* [in] */ IWifiDisplayStatus* wfdStatus,
    /* [in] */ Boolean disconnected)
{
    Boolean changed = FALSE;
    String newName;
    display->GetFriendlyDisplayName(&newName);
    AutoPtr<ICharSequence> cs;
    route->GetName((ICharSequence**)&cs);
    String rName;
    cs->ToString(&rName);
    if (!rName.Equals(newName)) {
        ((RouteInfo*)route)->mName = newName;
        changed = TRUE;
    }

    Boolean enabled = FALSE;
    IsWifiDisplayEnabled(display, wfdStatus, &enabled);
    changed |= ((RouteInfo*)route)->mEnabled != enabled;
    ((RouteInfo*)route)->mEnabled = enabled;

    Int32 ret;
    GetWifiDisplayStatusCode(display, wfdStatus, &ret);
    route->SetRealStatusCode(ret, &enabled);
    changed |= enabled;

    if (changed) {
        DispatchRouteChanged(route);
    }

    route->IsSelected(&enabled);
    if ((!enabled || disconnected) && enabled) {
        // Oops, no longer available. Reselect the default.
        SelectDefaultRouteStatic();
    }
}

AutoPtr<IWifiDisplay> CMediaRouter::FindWifiDisplay(
    /* [in] */ ArrayOf<AutoPtr<IWifiDisplay> >* displays,
    /* [in] */ const String& deviceAddress)
{
    Int32 length = displays->GetLength();
    AutoPtr<IWifiDisplay> d;
    AutoPtr<IInterface> obj;
    String address;
    for (Int32 i = 0; i < length; i++) {
        d = NULL;
        d = (*displays)[i];
        d->GetDeviceAddress(&address);
            if (address.Equals(deviceAddress)) {
                return d;
            }
        }
        return NULL;
}

AutoPtr<IMediaRouterRouteInfo> CMediaRouter::FindWifiDisplayRoute(
    /* [in] */ IWifiDisplay* d)
{
    Int32 count = ((Static*)(sStatic.Get()))->mRoutes.GetSize();
    String tempText1, tempText2;

    List< AutoPtr<IMediaRouterRouteInfo> >::Iterator it = ((Static*)(sStatic.Get()))->mRoutes.Begin();
    for (; it != ((Static*)(sStatic.Get()))->mRoutes.End(); ++it) {
        AutoPtr<IMediaRouterRouteInfo> info = *it;
        d->GetDeviceAddress(&tempText1);
        info->GetDeviceAddress(&tempText2);
        if (tempText1.Equals(tempText2)) {
            return info;
        }
    }
    return NULL;
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
