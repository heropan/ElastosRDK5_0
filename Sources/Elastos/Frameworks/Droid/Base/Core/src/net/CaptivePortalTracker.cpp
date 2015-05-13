
#include "net/CaptivePortalTracker.h"
#include "net/Uri.h"
#include "R.h"
#ifdef DROID_CORE
#include "provider/CSettingsGlobal.h"
#include "content/CIntent.h"
#include "content/CIntentFilter.h"
#include "content/res/CResourcesHelper.h"
#include "app/CNotification.h"
#include "app/CPendingIntentHelper.h"
#else
#include "Elastos.Droid.Core.h"
#endif
#include <elastos/StringUtils.h>
#include <elastos/Logger.h>

using Elastos::IO::IInputStream;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IResourcesHelper;
using Elastos::Droid::Content::Res::CResourcesHelper;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::App::IPendingIntentHelper;
using Elastos::Droid::App::CPendingIntentHelper;
using Elastos::Droid::App::INotification;
using Elastos::Droid::App::CNotification;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::App::INotificationManager;
using Elastos::Droid::Provider::CSettingsGlobal;
using Elastos::Droid::Provider::ISettingsGlobal;
using Elastos::Net::IURL;
using Elastos::Net::CURL;
using Elastos::Net::IInet4Address;
using Elastos::Net::IURLConnection;
using Elastos::Net::IInetAddressHelper;
using Elastos::Net::CInetAddressHelper;
using Elastos::Net::IHttpURLConnection;


namespace Elastos {
namespace Droid {
namespace Net {

const Boolean CaptivePortalTracker::DBG = FALSE;
const String CaptivePortalTracker::TAG = String("CaptivePortalTracker");
const String CaptivePortalTracker::DEFAULT_SERVER("clients3.google.com");
const String CaptivePortalTracker::NOTIFICATION_ID("CaptivePortal.Notification");
const Int32 CaptivePortalTracker::SOCKET_TIMEOUT_MS= 10000;
const Int32 CaptivePortalTracker::CMD_DETECT_PORTAL = 0;
const Int32 CaptivePortalTracker::CMD_CONNECTIVITY_CHANGE = 1;
const Int32 CaptivePortalTracker::CMD_DELAYED_CAPTIVE_CHECK = 2;
const Int32 CaptivePortalTracker::DELAYED_CHECK_INTERVAL_MS = 10000;

//==============================================================
// CaptivePortalTracker::DefaultState
//==============================================================
CaptivePortalTracker::DefaultState::DefaultState(
    /* [in] */ CaptivePortalTracker* owner)
    : mOwner(owner)
{}

ECode CaptivePortalTracker::DefaultState::Enter()
{
    // if (DBG) Log.d(TAG, getName() + "\n");
    return NOERROR;
}

ECode CaptivePortalTracker::DefaultState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    Int32 what;
    msg->GetWhat(&what);
    if (DBG) Logger::D("CaptivePortalTracker::DefaultState", "ProcessMessage %08x", what);
    switch (what) {
        case CMD_DETECT_PORTAL:
        {
            AutoPtr<INetworkInfo> info;
            msg->GetObj((IInterface**)&info);

            // Checking on a secondary connection is not supported
            // yet
            mOwner->NotifyPortalCheckComplete(info);
            break;
        }
        case CMD_CONNECTIVITY_CHANGE:
        case CMD_DELAYED_CAPTIVE_CHECK:
            break;
        default:
        {
// //            mOwner->Loge("Ignoring " + message);
            break;
        }
    }
    *result =  HANDLED;
    return NOERROR;
}

//==============================================================
// CaptivePortalTracker::NoActiveNetworkState
//==============================================================
CaptivePortalTracker::NoActiveNetworkState::NoActiveNetworkState(
    /* [in] */ CaptivePortalTracker* owner)
    : mOwner(owner)
{}

ECode CaptivePortalTracker::NoActiveNetworkState::Enter()
{
//    if (DBG) mOwner->Log(getName() + "\n");
    mOwner->mNetworkInfo = NULL;
    /* Clear any previous notification */
    mOwner->SetNotificationVisible(FALSE);
    return NOERROR;
}

ECode CaptivePortalTracker::NoActiveNetworkState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
// if (DBG) mOwner->Log(getName() + message.toString() + "\n");
    Int32 what;
    msg->GetWhat(&what);
    AutoPtr<IInetAddress> server;
    switch (what) {
        case CMD_CONNECTIVITY_CHANGE:
        {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            INetworkInfo* info = INetworkInfo::Probe(obj);
            Boolean infoBool;
            info->IsConnected(&infoBool);
            if (infoBool && mOwner->IsActiveNetwork(info)) {
                mOwner->mNetworkInfo = info;
                mOwner->TransitionTo(mOwner->mDelayedCaptiveCheckState);
            }
            break;
        }
        default:
            *result =  NOT_HANDLED;
            break;
    }
    *result =  HANDLED;
    return NOERROR;
}

//==============================================================
// CaptivePortalTracker::ActiveNetworkState
//==============================================================
CaptivePortalTracker::ActiveNetworkState::ActiveNetworkState(
    /* [in] */ CaptivePortalTracker* owner)
    : mOwner(owner)
{}

ECode CaptivePortalTracker::ActiveNetworkState::Enter()
{
//    if (DBG) mOwner->Log(getName() + "\n");
    return NOERROR;
}

ECode CaptivePortalTracker::ActiveNetworkState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case CMD_CONNECTIVITY_CHANGE:
        {
            Boolean infoConnected;
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            INetworkInfo* info = INetworkInfo::Probe(obj);
            info->IsConnected(&infoConnected);
            Int32 infoType, hostType;
            info->GetType(&infoType);
            mOwner->mNetworkInfo->GetType(&hostType);
            if (!infoConnected && infoType == hostType) {
//                if (DBG) mOwner->Log("Disconnected from active network " + info);
                mOwner->TransitionTo(mOwner->mNoActiveNetworkState);
            } else if (infoType != hostType && infoConnected &&
                     mOwner->IsActiveNetwork(info)) {
//               if (DBG) mOwner->Log("Active network switched " + info);
                mOwner->DeferMessage(msg);
                mOwner->TransitionTo(mOwner->mNoActiveNetworkState);
            }
            break;
        }
        default:
            *result =  NOT_HANDLED;
            break;
    }
    *result =  HANDLED;
    return NOERROR;
}

//==============================================================
// CaptivePortalTracker::DelayedCaptiveCheckState
//==============================================================
CaptivePortalTracker::DelayedCaptiveCheckState::DelayedCaptiveCheckState(
    /* [in] */ CaptivePortalTracker* owner)
    : mOwner(owner)
{}

ECode CaptivePortalTracker::DelayedCaptiveCheckState::Enter()
{
    if (mOwner->DBG) Logger::D("CaptivePortalTracker::DelayedCaptiveCheckState", "Enter()");
    AutoPtr<IMessage> m = mOwner->ObtainMessage(
        CMD_DELAYED_CAPTIVE_CHECK, ++mOwner->mDelayedCheckToken, 0);
    mOwner->SendMessageDelayed(m, DELAYED_CHECK_INTERVAL_MS);
    return NOERROR;
}

ECode CaptivePortalTracker::DelayedCaptiveCheckState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    Int32 what;
    msg->GetWhat(&what);
//  if (DBG) mOwner->Log(getName() + message.toString() + "\n");
    switch (what) {
        case CMD_DELAYED_CAPTIVE_CHECK:
        {
			Int32 arg1;
			msg->GetArg1(&arg1);
            if (arg1 == mOwner->mDelayedCheckToken) {
            AutoPtr<IInetAddress> server = mOwner->LookupHost(mOwner->mServer);
            if (server != NULL) {
                if (mOwner->IsCaptivePortal(server)) {
                    //if (DBG) mOwner->Log("Captive network " + mNetworkInfo);
                    mOwner->SetNotificationVisible(TRUE);
                }
            }
//                 if (DBG) mOwner->Log("Not captive network " + mNetworkInfo);
                mOwner->TransitionTo(mOwner->mActiveNetworkState);
                   assert(0);
            }
            break;
        }
        default:
            *result =  NOT_HANDLED;
            break;
    }
    *result =  HANDLED;
    return NOERROR;
}

CaptivePortalTracker::CaptivePortalTracker(
    /* [in] */ IContext* context,
    /* [in] */ IIConnectivityManager* cs)
    : StateMachine(TAG)
    , mNotificationShown(FALSE)
    , mConnService(cs)
    , mContext(context)
    , mDelayedCheckToken(0)
{
//    super(TAG);

    mDefaultState = new DefaultState(this);
    mNoActiveNetworkState = new NoActiveNetworkState(this);
    mActiveNetworkState = new ActiveNetworkState(this);
    mDelayedCaptiveCheckState = new DelayedCaptiveCheckState(this);
    //mContext->GetSystemService(IContext::TELEPHONY_SERVICE, (IInterface**)(ITelephonyManager**)&mTelephonyManager);
    AutoPtr<ISettingsGlobal> settings;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settings);

    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(IConnectivityManager::CONNECTIVITY_ACTION);
    AutoPtr<IIntent> iinet;
    mContext->RegisterReceiver(mReceiver, filter, (IIntent**)&iinet);
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    settings->GetString(resolver,ISettingsGlobal::CAPTIVE_PORTAL_SERVER, &mServer);
    if (mServer.IsNull()) mServer = DEFAULT_SERVER;

    Int32 val;
    AutoPtr<IContentResolver> conResolver;
    mContext->GetContentResolver((IContentResolver**)&conResolver);
    settings->GetInt32(conResolver, ISettingsGlobal::CAPTIVE_PORTAL_DETECTION_ENABLED, 1, &val);
    mIsCaptivePortalCheckEnabled = (val == 1);

    AddState(mDefaultState);
    AddState(mNoActiveNetworkState, mDefaultState);
    AddState(mActiveNetworkState, mDefaultState);
    AddState(mDelayedCaptiveCheckState, mActiveNetworkState);
    SetInitialState(mNoActiveNetworkState);
}

AutoPtr<CaptivePortalTracker> CaptivePortalTracker::MakeCaptivePortalTracker(
    /* [in] */ IContext* context,
    /* [in] */ IIConnectivityManager* cs)
{
    AutoPtr<CaptivePortalTracker> cpt = new CaptivePortalTracker(context, cs);
    cpt->Start();
    return cpt;
}

void CaptivePortalTracker::DetectCaptivePortal(
    /* [in] */ INetworkInfo* info)
{
    AutoPtr<IMessage> m = ObtainMessage(CMD_DETECT_PORTAL, info);
    SendMessage(m);
}

void CaptivePortalTracker::NotifyPortalCheckComplete(
    /* [in] */ INetworkInfo* info)
{
    if (info == NULL) {
        Logger::E(TAG, "notifyPortalCheckComplete on NULL");
        return;
    }

    if (DBG) Logger::D(TAG, "notifyPortalCheckComplete %p", info);
//     try {
    mConnService->CaptivePortalCheckComplete(info);
//     } catch(RemoteException e) {
//         e.printStackTrace();
//     }
}

Boolean CaptivePortalTracker::IsActiveNetwork(
    /* [in] */ INetworkInfo* info)
{
//     try {
    AutoPtr<INetworkInfo> active;
    mConnService->GetActiveNetworkInfo((INetworkInfo**)&active);
    Int32 infoType, activeType;
    active->GetType(&activeType);
    info->GetType(&infoType);
    if (active != NULL && activeType == infoType) {
        return TRUE;
    }
//     } catch (RemoteException e) {
//         e.printStackTrace();
//     }
    return FALSE;
}

/**
 * Do a URL fetch on a known server to see if we get the data we expect
 */
Boolean CaptivePortalTracker::IsCaptivePortal(
    /* [in] */ IInetAddress* server)
{
    AutoPtr<IHttpURLConnection> urlConnection ;
    if (!mIsCaptivePortalCheckEnabled) return FALSE;

    String str;
    server->GetHostAddress(&str);

    mUrl = String("http://" )+ str + String("/generate_204");
//     if (DBG) Log("Checking " + mUrl);
//     try {
    AutoPtr<IURL> url;
    CURL::New(mUrl, (IURL**)&url);
    AutoPtr<IURLConnection> urlconnect;
    url->OpenConnection((IURLConnection**)&urlconnect);
    urlConnection = IHttpURLConnection::Probe(urlconnect);
    urlConnection->SetInstanceFollowRedirects(FALSE);
    urlConnection->SetConnectTimeout(SOCKET_TIMEOUT_MS);
    urlConnection->SetReadTimeout(SOCKET_TIMEOUT_MS);
    urlConnection->SetUseCaches(FALSE);
    AutoPtr<IInputStream> inputstream;
    urlConnection->GetInputStream((IInputStream**)&inputstream);
    // we got a valid response, but not from the real google
    Int32 n;
    urlConnection->GetResponseCode(&n);
//     } catch (IOException e) {
//        if (DBG) Log("Probably not a portal: exception " + e);
//         return FALSE;
//     } finally {
    if (urlConnection != NULL) {
        urlConnection->Disconnect();
    }
//     }
// }
    return (n != 204)? TRUE: FALSE;
}

AutoPtr<IInetAddress> CaptivePortalTracker::LookupHost(
    /* [in] */ const String& hostname)
{

    AutoPtr<ArrayOf<IInetAddress*> > inetAddr;
//     try {
        AutoPtr<IInetAddressHelper> inetaddresshelper;
        CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&inetaddresshelper);
        inetaddresshelper->GetAllByName(hostname, (ArrayOf<IInetAddress*>**)&inetAddr);
//     } catch (UnknownHostException e) {
//         return NULL;
//     }
    for (Int32 i = 0; i < inetAddr->GetLength(); i++) {
        if (IInet4Address::Probe((*inetAddr)[i]) != NULL)
             return (*inetAddr)[i];
    }
    return NULL;
}

void CaptivePortalTracker::SetNotificationVisible(
    /* [in] */ Boolean visible)
{
    // if it should be hidden and it is already hidden, then noop
    if (!visible && !mNotificationShown) {
        return;
    }

    AutoPtr<IResourcesHelper> resHelper;
    CResourcesHelper::AcquireSingleton((IResourcesHelper**)&resHelper);
    AutoPtr<IResources> r;
    resHelper->GetSystem((IResources**)&r);

    AutoPtr<INotificationManager> notificationManager;
    mContext->GetSystemService(IContext::NOTIFICATION_SERVICE, (IInterface**)(INotificationManager**)&notificationManager);

    if (visible) {
        AutoPtr<ICharSequence> title;
        AutoPtr<ICharSequence> details;
        Int32 icon, netType;
        mNetworkInfo->GetType(&netType);
        String extrInfo;
        mNetworkInfo->GetExtraInfo(&extrInfo);
        switch (netType) {
            case IConnectivityManager::TYPE_WIFI:
            {
                String titleStr;
                r->GetString(R::string::wifi_available_sign_in, &titleStr);
                titleStr += StringUtils::Int32ToString(0);
                CStringWrapper::New(titleStr, (ICharSequence**)&title);

                String detailsStr;
                r->GetString(R::string::network_available_sign_in_detailed, &detailsStr);
                detailsStr += extrInfo;
                CStringWrapper::New(detailsStr, (ICharSequence**)&details);
                icon = R::drawable::stat_notify_wifi_in_range;
                break;
            }
            case IConnectivityManager::TYPE_MOBILE:
            {
                // String titleStr;
                // r->GetString(R::string::network_available_sign_in, &titleStr);
                // titleStr += StringUtils::Int32ToString(0);
                // CStringWrapper::New(titleStr, (ICharSequence**)&title);
                // // TODO: Change this to pull from NetworkInfo once a printable
                // // name has been added to it
                // mTelephonyManager->GetNetworkOperatorName((ICharSequence**)&details);
                // icon = R::drawable::stat_notify_rssi_in_range;
                break;
                assert(0);
            }
            default:
            {
                String titleStr;
                r->GetString(R::string::network_available_sign_in, &titleStr);
                titleStr += StringUtils::Int32ToString(0);
                CStringWrapper::New(titleStr, (ICharSequence**)&title);
                String detailsStr;
                r->GetString(R::string::network_available_sign_in_detailed, &detailsStr);
                detailsStr += extrInfo;
                CStringWrapper::New(detailsStr, (ICharSequence**)&details);
                icon = R::drawable::stat_notify_rssi_in_range;
                break;
            }
        }

        AutoPtr<INotification> notification;
        CNotification::New((INotification**)&notification);
        notification->SetWhen(0);
        notification->SetIcon(icon);
        notification->SetFlags(INotification::FLAG_AUTO_CANCEL);
        AutoPtr<IIntent> intent;
        AutoPtr<IUri> uri;
        Uri::Parse(mUrl, (IUri**)&uri);
        CIntent::New(IIntent::ACTION_VIEW, (IUri*)uri, (IIntent**)&intent);
        intent->SetFlags(IIntent::FLAG_ACTIVITY_BROUGHT_TO_FRONT | IIntent::FLAG_ACTIVITY_NEW_TASK);
        AutoPtr<IPendingIntent> pendingintent;
        notification->GetContentIntent((IPendingIntent**)&pendingintent);
        AutoPtr<IPendingIntentHelper> helper;
        CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&helper);
        pendingintent = NULL;
        helper->GetActivity(mContext, 0, intent, 0, (IPendingIntent**)&pendingintent);
        notification->SetTickerText(title);
        AutoPtr<IPendingIntent> pintent;
        notification->GetContentIntent((IPendingIntent**)&pintent);
        notification->SetLatestEventInfo(mContext, title, details, pintent);

        notificationManager->NotifyEx(NOTIFICATION_ID, 1, notification);
    } else {
        notificationManager->CancelEx(NOTIFICATION_ID, 1);
    }

    mNotificationShown = visible;
}


} // namespace Net
} // namepsace Droid
} // namespace Elastos
