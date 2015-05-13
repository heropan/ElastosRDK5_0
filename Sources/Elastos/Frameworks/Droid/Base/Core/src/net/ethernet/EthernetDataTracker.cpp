
#include "net/ethernet/EthernetDataTracker.h"
#include "net/NetworkUtils.h"
#ifdef DROID_CORE
#include "content/CIntent.h"
#include "content/CIntentFilter.h"
#include "net/CInterfaceConfiguration.h"
#include "net/CRouteInfo.h"
#include "net/CLinkCapabilities.h"
#include "net/CLinkProperties.h"
#include "net/CNetworkInfo.h"
#include "net/ethernet/CEthernetDevInfo.h"
#include "net/ethernet/CEDTInterfaceObserver.h"
#include "net/ethernet/CEthernetManager.h"
#include "os/CSystemProperties.h"
#endif
#include "os/Handler.h"
#include "os/ServiceManager.h"
#include <elastos/Logger.h>
#include <elastos/Thread.h>
#include <cutils/properties.h>
#include "R.h"

using Elastos::Core::Threading::Thread;
using Elastos::Core::Threading::IThread;
using Elastos::Core::Threading::CThread;
using Elastos::Utility::Regex::IPatternHelper;
using Elastos::Utility::Regex::CPatternHelper;
using Elastos::Utility::Concurrent::Atomic::CAtomicBoolean;
using Elastos::Utility::Concurrent::Atomic::CAtomicInteger32;
using Elastos::Utility::Logging::Logger;
using Elastos::Net::IInetAddress;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::Os::INetworkManagementService;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Net::CNetworkInfo;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Ethernet {

const String EthernetDataTracker::NETWORKTYPE("ETHERNET");
const String EthernetDataTracker::TAG("Ethernet");

AutoPtr<EthernetDataTracker> EthernetDataTracker::sInstance;
Mutex EthernetDataTracker::sInstanceLock;

static Boolean Matches(
    /* [in] */ const String& regularExpression,
    /* [in] */ const String& input)
{
    AutoPtr<IPatternHelper> ph;
    CPatternHelper::AcquireSingleton((IPatternHelper**)&ph);
    Boolean matched;
    ph->Matches(regularExpression, input, &matched);
    return matched;
}

EthernetDataTracker::DhcpRunnable::DhcpRunnable(
    /* [in] */ EthernetDataTracker* owner)
    : mOwner(owner)
{}

ECode EthernetDataTracker::DhcpRunnable::Run()
{
    AutoPtr<DhcpInfoInternal> dhcpInfoInternal = new DhcpInfoInternal();
    if (!NetworkUtils::RunDhcp(String("eth_") + mOwner->mIface, dhcpInfoInternal)) {
        Logger::E(TAG, String("DHCP request error:") + NetworkUtils::GetDhcpError());

        if (mOwner->mPppoe_ok) {
            return NOERROR;
        }

        mOwner->mNetworkInfo->SetIsAvailable(FALSE);
        mOwner->mNetworkInfo->SetDetailedState(NetworkInfoDetailedState_DISCONNECTED, String(NULL), String(NULL));
        Logger::E(TAG, "rundhcp failed ---------------------------------");
        AutoPtr<IMessage> msg = mOwner->ObtainMessage(EVENT_STATE_CHANGED, mOwner->mNetworkInfo);
        Boolean res;
        mOwner->mCsHandler->SendMessage(msg, &res);
        mOwner->SendStateBroadcast(IEthernetManager::EVENT_CONFIGURATION_FAILED);
        return NOERROR;
    }

    mOwner->mLinkProperties = dhcpInfoInternal->MakeLinkProperties();
    mOwner->mLinkProperties->SetInterfaceName(mOwner->mIface);

    mOwner->mNetworkInfo->SetDetailedState(NetworkInfoDetailedState_CONNECTED, String(NULL), String(NULL));
    mOwner->mNetworkInfo->SetIsAvailable(TRUE);
    AutoPtr<IMessage> msg = mOwner->ObtainMessage(EVENT_STATE_CHANGED, mOwner->mNetworkInfo);
    Boolean res;
    mOwner->mCsHandler->SendMessage(msg, &res);
    mOwner->SendStateBroadcast(IEthernetManager::EVENT_CONFIGURATION_SUCCEEDED);

    return NOERROR;
}

EthernetDataTracker::EthernetDataTracker()
    : mLinkUp(FALSE)
    , mIfaceMatch("")
    , mIface("")
    , mOldState(-1)
    , mOldisAvailable(FALSE)
    , mPppoe_ok(FALSE)
{
    CAtomicBoolean::New(FALSE, (IAtomicBoolean**)&mTeardownRequested);
    CAtomicBoolean::New(FALSE, (IAtomicBoolean**)&mPrivateDnsRouteSet);
    CAtomicInteger32::New(0, (IAtomicInteger32**)&mDefaultGatewayAddr);
    CAtomicBoolean::New(FALSE, (IAtomicBoolean**)&mDefaultRouteSet);

    CNetworkInfo::New(IConnectivityManager::TYPE_ETHERNET, 0, NETWORKTYPE, String(""), (INetworkInfo**)&mNetworkInfo);
    CLinkProperties::New((ILinkProperties**)&mLinkProperties);
    CLinkCapabilities::New((ILinkCapabilities**)&mLinkCapabilities);

    mNetworkInfo->SetIsAvailable(FALSE);
    SetTeardownRequested(FALSE);

    mPppoeStateReceiver = new PppoeStateReceiver(this);
}

EthernetDataTracker::~EthernetDataTracker()
{}

PInterface EthernetDataTracker::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(INetworkStateTracker*)this;
    }
    return NULL;
}

UInt32 EthernetDataTracker::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 EthernetDataTracker::Release()
{
    return ElRefBase::Release();
}

ECode EthernetDataTracker::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return NOERROR;
}

void EthernetDataTracker::InterfaceAdded(
    /* [in] */ const String& iface)
{
    /**  It will add an new interface to EthernetService and check it.  **/
    Boolean res = FALSE;
    mEthManage->AddInterfaceToService(iface, &res);
    if(!res)
        return;

    /**  The first adding interface will be reconnect.  **/
    {
        Mutex::Autolock lock(mIfaceLock);
        if(!mIface.IsEmpty())
            return;
        mIface = iface;
    }

    NetworkUtils::EnableInterface(mIface);
    Reconnect(&res);
}

void EthernetDataTracker::InterfaceRemoved(
    /* [in] */ const String& iface)
{
    mEthManage->RemoveInterfaceFormService(iface);
    if (!Matches(mIface, iface))
        return;

    /*
    NetworkUtils.stopDhcp("eth_" + mIface);
    mLinkProperties.clear();
    mNetworkInfo.setIsAvailable(FALSE);
    mNetworkInfo.setDetailedState(DetailedState.DISCONNECTED, String(NULL), String(NULL));

    Message msg = mCsHandler.obtainMessage(EVENT_CONFIGURATION_CHANGED,
                                                mNetworkInfo);
    msg.sendToTarget();
    msg = mCsHandler.obtainMessage(EVENT_STATE_CHANGED, mNetworkInfo);
    msg.sendToTarget();
    */
    Boolean res;
    Teardown(&res);

    {
        Mutex::Autolock lock(mIfaceLock);
        mIface = "";
    }
}

void EthernetDataTracker::InterfaceLinkStateChanged(
    /* [in] */ const String& iface,
    /* [in] */ Boolean up)
{
    Int32 tmpStatus = 0;
    Int32 i = 0;
    if (Matches(iface, mIface)) {
        for(i = 0; i < 3; i++) {
            // try{
                Thread::Sleep(100);
            // }catch(Exception e){ }
            mEthManage->CheckLink(iface, &tmpStatus);
        }

        if(tmpStatus == -1) {
            return;
        }

        up = (tmpStatus == 1) ? TRUE : FALSE;
        if(mLinkUp == up)
            return;

        mLinkUp = up;
        AutoPtr<IIntent> intent;
        if (up) {
            CIntent::New(IEthernetManager::ETHERNET_LINKED_ACTION, (IIntent**)&intent);
            intent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
            Boolean res;
            Reconnect(&res);
            mContext->SendBroadcast(intent);
        }
        else {
            NetworkUtils::StopDhcp(String("eth_") + mIface);
            mLinkProperties->Clear();
            mNetworkInfo->SetIsAvailable(FALSE);
            mNetworkInfo->SetDetailedState(NetworkInfoDetailedState_DISCONNECTED, String(NULL), String(NULL));
            Logger::E(TAG, "link down------------------------------");
            AutoPtr<IMessage> msg = ObtainMessage(EVENT_STATE_CHANGED, mNetworkInfo);
            Boolean res;
            mCsHandler->SendMessage(msg, &res);
            SendStateBroadcast(IEthernetManager::EVENT_DISCONNECTED);
            CIntent::New(IEthernetManager::ETHERNET_DISLINKED_ACTION, (IIntent**)&intent);
            intent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
            mContext->SendBroadcast(intent);
            mPppoe_ok = FALSE;
        }
    }
}

void EthernetDataTracker::RunDhcp()
{
    AutoPtr<DhcpRunnable> r = new DhcpRunnable(this);
    AutoPtr<IThread> dhcpThread;
    CThread::New(r.Get(), String("ETH_DHCP"), (IThread**)&dhcpThread);
    dhcpThread->Start();
}

AutoPtr<DhcpInfoInternal> EthernetDataTracker::GetIpConfigure(
    /* [in] */ IEthernetDevInfo* info)
{
    AutoPtr<IInetAddress> netmask;
    AutoPtr<IInetAddress> gw;
    AutoPtr<IRouteInfo> routeAddress;
    Int32 prefixLength = 0;
    AutoPtr<DhcpInfoInternal> infoInternal = new DhcpInfoInternal();

    if(info == NULL)
        return NULL;

    String mask;
    info->GetNetMask(&mask);
    if(mask.IsNull() || Matches(String(""), mask)) {
        NetworkUtils::NumericToInetAddress(String("0.0.0.0"), (IInetAddress**)&netmask);
    }
    else {
        NetworkUtils::NumericToInetAddress(mask, (IInetAddress**)&netmask);
    }

    Int32 intAddr = 0;
    NetworkUtils::InetAddressToInt32(netmask, &intAddr);
    prefixLength = NetworkUtils::NetmaskIntToPrefixLength(intAddr);

    String gateWay;
    info->GetGateWay(&gateWay);
    if(!gateWay.IsNull() && !Matches(String(""), gateWay))
        NetworkUtils::NumericToInetAddress(gateWay, (IInetAddress**)&gw);
    else
        NetworkUtils::NumericToInetAddress(String("0.0.0.0"), (IInetAddress**)&gw);

    info->GetIpAddress(&infoInternal->mIpAddress);
    info->GetDnsAddr(&infoInternal->mDns1);
    infoInternal->mPrefixLength = prefixLength;
    CRouteInfo::New(NULL, gw, (IRouteInfo**)&routeAddress);
    infoInternal->AddRoute(routeAddress);

    return infoInternal;
}

void EthernetDataTracker::ConnectNetwork(
    /* [in] */ Boolean up)
{
    //Log.d(TAG, "Up is " + up + " mLinkUp is " + mLinkUp +
    //          " On is " + mEthManage.isOn() + " mIface " + mIface);
    Boolean isConfigured;
    mEthManage->IsConfigured(&isConfigured);
    if(!isConfigured) {
        return;
    }

    Boolean isOn, isRequested;
    if(up && (mEthManage->IsOn(&isOn), isOn)) {
        AutoPtr<IEthernetDevInfo> ifaceInfo;
        mEthManage->GetSavedConfig((IEthernetDevInfo**)&ifaceInfo);
        if(ifaceInfo == NULL)
            return;

        {
            Mutex::Autolock lock(mIfaceLock);
            String ifName;
            ifaceInfo->GetIfName(&ifName);
            if (!mIface.Equals(ifName)) {
                if (!mIface.IsEmpty()) {
                    NetworkUtils::StopDhcp(String("eth_") + mIface);
                    NetworkUtils::DisableInterface(mIface);
                }
                mIface = ifName;
            }
        }

        NetworkUtils::EnableInterface(mIface);

        if(mLinkUp == FALSE)
            return;

        Boolean isDhcp;
        if (mEthManage->IsDhcp(&isDhcp), isDhcp) {
            // try{
                mNMService->ClearInterfaceAddresses(mIface);
                NetworkUtils::ResetConnections(mIface, 0);
            // } catch (RemoteException e) {
            //     Log.e(TAG, "ERROR: " + e);
            // }

            AutoPtr<ISystemProperties> sysProp;
            CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
            String value;
            if (sysProp->Get(String("dhcp.") + mIface + ".result", &value), value.Equals("ok")) {
                NetworkUtils::StopDhcp(String("eth_") + mIface);
                SendStateBroadcast(IEthernetManager::EVENT_CONFIGURATION_FAILED);
            }
            RunDhcp();
        }
        else {
            NetworkUtils::StopDhcp(String("eth_") + mIface);

            AutoPtr<IInterfaceConfiguration> ifcg;
            CInterfaceConfiguration::New((IInterfaceConfiguration**)&ifcg);
            AutoPtr<DhcpInfoInternal> dhcpInfoInternal = GetIpConfigure(ifaceInfo);
            mLinkProperties = dhcpInfoInternal->MakeLinkProperties();
            mLinkProperties->SetInterfaceName(mIface);

            ifcg->SetLinkAddress(dhcpInfoInternal->MakeLinkAddress());
            ifcg->SetInterfaceUp();

            ECode ec = mNMService->SetInterfaceConfig(mIface, ifcg);
            if (FAILED(ec)) {
                Logger::E(TAG, "ERROR: 0x%x", ec);
                SendStateBroadcast(IEthernetManager::EVENT_CONFIGURATION_FAILED);
                return;
            }

            Logger::D(TAG, "network connected.");
            mNetworkInfo->SetDetailedState(NetworkInfoDetailedState_CONNECTED, String(NULL), String(NULL));
            mNetworkInfo->SetIsAvailable(TRUE);
            AutoPtr<IMessage> msg = ObtainMessage(EVENT_STATE_CHANGED, mNetworkInfo);
            Boolean res;
            mCsHandler->SendMessage(msg, &res);
            SendStateBroadcast(IEthernetManager::EVENT_CONFIGURATION_SUCCEEDED);
        }
    }
    else if (IsTeardownRequested(&isRequested), isRequested) {
        NetworkUtils::StopDhcp(String("eth_") + mIface);
        mLinkProperties->Clear();
        NetworkUtils::DisableInterface(mIface);

        Logger::D(TAG, "network disconnected.");
        mNetworkInfo->SetIsAvailable(TRUE);
        mNetworkInfo->SetDetailedState(NetworkInfoDetailedState_DISCONNECTED, String(NULL), String(NULL));
        AutoPtr<IMessage> msg = ObtainMessage(EVENT_STATE_CHANGED, mNetworkInfo);
        Boolean res;
        mCsHandler->SendMessage(msg, &res);
        SendStateBroadcast(IEthernetManager::EVENT_DISCONNECTED);
    }
}

void EthernetDataTracker::SendStateBroadcast(
    /* [in] */ Int32 event)
{
    AutoPtr<IIntent> intent;
    CIntent::New(IEthernetManager::NETWORK_STATE_CHANGED_ACTION, (IIntent**)&intent);
    intent->AddFlags(
        IIntent::FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT
        | IIntent::FLAG_RECEIVER_REPLACE_PENDING);
    intent->PutParcelableExtra(
        IEthernetManager::EXTRA_NETWORK_INFO, IParcelable::Probe(mNetworkInfo));
    AutoPtr<ILinkProperties> lp;
    CLinkProperties::New(mLinkProperties, (ILinkProperties**)&lp);
    intent->PutParcelableExtra(
        IEthernetManager::EXTRA_LINK_PROPERTIES, IParcelable::Probe(lp));
    intent->PutInt32Extra(IEthernetManager::EXTRA_ETHERNET_STATE, event);
    mContext->SendStickyBroadcast(intent);
}

/*static synchronized*/
AutoPtr<EthernetDataTracker> EthernetDataTracker::GetInstance()
{
    Mutex::Autolock lock(sInstanceLock);
    if (sInstance == NULL){
        sInstance = new EthernetDataTracker();
    }
    return sInstance;
}

ECode EthernetDataTracker::SetTeardownRequested(
    /* [in] */ Boolean isRequested)
{
    return mTeardownRequested->Set(isRequested);
}

ECode EthernetDataTracker::IsTeardownRequested(
    /* [out] */ Boolean* isRequested)
{
    return mTeardownRequested->Get(isRequested);
}

ECode EthernetDataTracker::StartMonitoring(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* target)
{
    mContext = context;
    mCsHandler = target;

    // register for notifications from NetworkManagement Service
    mNMService = (INetworkManagementService*)ServiceManager::GetService(
        IContext::NETWORKMANAGEMENT_SERVICE).Get();

    mEthManage = NULL;
    CEthernetManager::AcquireSingleton((IEthernetManager**)&mEthManage);

    if (mEthManage == NULL) {
        Logger::E("SHUGE", "mEthManage is nulllllllllllllllllllllllllllllllllllll");
    }
    else {
        Logger::D("SHUGE", "mEthManage is OKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK");
    }

    mInterfaceObserver = NULL;
    CEDTInterfaceObserver::New((Handle32)this, (INetworkManagementEventObserver**)&mInterfaceObserver);

    // enable and try to connect to an ethernet interface that
    // already exists
    AutoPtr<IResources> r;
    context->GetResources((IResources**)&r);
    r->GetString(R::string::config_ethernet_iface_regex, &mIfaceMatch);

    // try {
    AutoPtr<IObjectContainer> ethInfos;
    mEthManage->GetDeviceNameList((IObjectContainer**)&ethInfos);
    AutoPtr<IEthernetDevInfo> saveInfo;
    mEthManage->GetSavedConfig((IEthernetDevInfo**)&saveInfo);

    if (saveInfo != NULL && ethInfos != NULL) {
        AutoPtr<IObjectEnumerator> it;
        ethInfos->GetObjectEnumerator((IObjectEnumerator**)&it);
        Boolean succeeded;
        while (it->MoveNext(&succeeded), succeeded) {
            AutoPtr<IEthernetDevInfo> info;
            it->Current((IInterface**)&info);
            String infoIfName, saveIfName;
            info->GetIfName(&infoIfName);
            saveInfo->GetIfName(&saveIfName);
            if (Matches(saveIfName, infoIfName)) {
                saveInfo->SetIfName(infoIfName);
                String hwAddr;
                info->GetHwaddr(&hwAddr);
                saveInfo->SetHwaddr(hwAddr);
                mEthManage->UpdateDevInfo(saveInfo);
            }
        }
    }

    AutoPtr<ArrayOf<String> > ifaces;
    mNMService->ListInterfaces((ArrayOf<String>**)&ifaces);
    //for (String iface : ifaces) {
    //  if (iface.matches(mIfaceMatch)) {
    //      mIface = iface;
    //      mEthManage.setEnabled(mEthManage.isOn());
    //      break;
    //  }
    //}
    // } catch (RemoteException e) {
        // Log.e(TAG, "Could not get list of interfaces " + e);
    // }

    // try {
    mNMService->RegisterObserver(mInterfaceObserver);
    // } catch (RemoteException e) {
        // Log.e(TAG, "Could not register InterfaceObserver " + e);
    // }
    mPppoeStateFilter = NULL;
    CIntentFilter::New(
        IEthernetManager::PPPOE_STATE_CHANGED_ACTION, (IIntentFilter**)&mPppoeStateFilter);
    AutoPtr<IIntent> intent;
    mContext->RegisterReceiverEx(mPppoeStateReceiver, mPppoeStateFilter,
        String(NULL), NULL, (IIntent**)&intent);

    return NOERROR;
}

ECode EthernetDataTracker::Teardown(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Logger::E(TAG,"teardown ----------------------------------------------------");
    mTeardownRequested->Set(TRUE);
    ConnectNetwork(FALSE);
    *result = TRUE;
    return NOERROR;
}

ECode EthernetDataTracker::Reconnect(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    mTeardownRequested->Set(FALSE);
    ConnectNetwork(TRUE);
    *result = TRUE;
    return NOERROR;
}

ECode EthernetDataTracker::CaptivePortalCheckComplete()
{
    // not implemented
    return NOERROR;
}

ECode EthernetDataTracker::SetRadio(
    /* [in] */ Boolean turnOn,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

/*synchronized*/
ECode EthernetDataTracker::IsAvailable(
    /* [out] */ Boolean* isAvailable)
{
    Mutex::Autolock lock(mSelfLock);
    return mNetworkInfo->IsAvailable(isAvailable);
}

Int32 EthernetDataTracker::StartUsingNetworkFeature(
    /* [in] */ const String& feature,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid)
{
    return -1;
}

Int32 EthernetDataTracker::StopUsingNetworkFeature(
    /* [in] */ const String& feature,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid)
{
    return -1;
}

ECode EthernetDataTracker::SetUserDataEnable(
    /* [in] */ Boolean enabled)
{
    Logger::W(TAG, "ignoring setUserDataEnable(%d)", enabled);
    return NOERROR;
}

ECode EthernetDataTracker::SetPolicyDataEnable(
    /* [in] */ Boolean enabled)
{
    Logger::W(TAG, "ignoring setPolicyDataEnable(%d)", enabled);
    return NOERROR;
}

ECode EthernetDataTracker::IsPrivateDnsRouteSet(
    /* [out] */ Boolean* isSet)
{
    return mPrivateDnsRouteSet->Get(isSet);
}

ECode EthernetDataTracker::SetPrivateDnsRoute(
    /* [in] */ Boolean enabled)
{
    return mPrivateDnsRouteSet->Set(enabled);
}

/*Synchronized*/
ECode EthernetDataTracker::GetNetworkInfo(
    /* [out] */ INetworkInfo** info)
{
    Mutex::Autolock lock(mSelfLock);
    *info = mNetworkInfo;
    INTERFACE_ADDREF(*info);
    return NOERROR;
}

/*Synchronized*/
ECode EthernetDataTracker::GetLinkProperties(
    /* [out] */ ILinkProperties** linkProperties)
{
    VALIDATE_NOT_NULL(linkProperties);
    Mutex::Autolock lock(mSelfLock);
    return CLinkProperties::New(mLinkProperties, linkProperties);
}

ECode EthernetDataTracker::GetLinkCapabilities(
    /* [out] */ ILinkCapabilities** linkCapabilities)
{
    VALIDATE_NOT_NULL(linkCapabilities);
    return CLinkCapabilities::New(mLinkCapabilities, linkCapabilities);
}

Int32 EthernetDataTracker::GetDefaultGatewayAddr()
{
    Int32 value;
    mDefaultGatewayAddr->Get(&value);
    return value;
}

ECode EthernetDataTracker::IsDefaultRouteSet(
    /* [out] */ Boolean* isSet)
{
    return mDefaultRouteSet->Get(isSet);
}

ECode EthernetDataTracker::SetDefaultRoute(
    /* [in] */ Boolean enabled)
{
    return mDefaultRouteSet->Set(enabled);
}

ECode EthernetDataTracker::GetTcpBufferSizesPropName(
    /* [out] */ String* propName)
{
    VALIDATE_NOT_NULL(propName);
    *propName = "net.tcp.buffersize.wifi";
    return NOERROR;
}

ECode EthernetDataTracker::SetDependencyMet(
    /* [in] */ Boolean met)
{
    // not supported on this network
    return NOERROR;
}

AutoPtr<IMessage> EthernetDataTracker::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj)
{
    if (mCsHandler == NULL)
        return NULL;

    AutoPtr<IMessage> m;
    mCsHandler->ObtainMessageEx(what, obj, (IMessage**)&m);
    return m;
}

//add by celema //////////////////////////////////////////////////////////////

EthernetDataTracker::PppoeStateReceiver::PppoeStateReceiver(
    /* [in] */ EthernetDataTracker* owner)
    : mOwner(owner)
{
}

ECode EthernetDataTracker::PppoeStateReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (action.Equals(IEthernetManager::PPPOE_STATE_CHANGED_ACTION)) {
        Int32 pppoeState;
        intent->GetInt32Extra(IEthernetManager::EXTRA_PPPOE_STATE, -1, &pppoeState);
        mOwner->HandlePppoe(pppoeState);
    }

    return NOERROR;
}

void EthernetDataTracker::HandlePppoe (
    /* [in] */ Int32 pppoeState)
{
    AutoPtr<IMessage> pppoe_msg;
    // AutoPtr<LinkProperties> backupLinkP;
    switch (pppoeState) {
        case IEthernetManager::PPPOE_STATE_ENABLED:
        {
            AutoPtr<ISystemProperties> sysProp;
            CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
            String value;
            sysProp->Get(String("net.pppoe.interface"), &value);
            Logger::D(TAG, "broadcast pppoe enable:%s", value.string());

            mLinkProperties->SetInterfaceName(value);
            // befor we change the state ,  backup them
            mNetworkInfo->GetDetailedState(&mOldState);
            mNetworkInfo->IsAvailable(&mOldisAvailable);

            mNetworkInfo->SetDetailedState(NetworkInfoDetailedState_CONNECTED, String(NULL), String("pppoe"));
            mNetworkInfo->SetIsAvailable(TRUE);
            pppoe_msg = ObtainMessage(EVENT_STATE_CHANGED, mNetworkInfo);
            Boolean res;
            mCsHandler->SendMessage(pppoe_msg, &res);
            mPppoe_ok = TRUE;
            break;
        }
        case IEthernetManager::PPPOE_STATE_DISABLED:
            Logger::D(TAG, "broadcast pppoe disable:%s", mIface.string());

            if(mOldState == -1)
                break;
            //set back mInface and state
            Logger::D(TAG, "11111111111111111---oldiface : %s", mIface.string());
            mLinkProperties->SetInterfaceName(mIface);
            mNetworkInfo->SetDetailedState(mOldState, String(NULL), String("pppoe"));
            mNetworkInfo->SetIsAvailable(mOldisAvailable);
          /*
            backupLinkP = mLinkProperties;
            mLinkProperties.clear();
            pppoe_msg = mCsHandler.obtainMessage(EVENT_CONFIGURATION_CHANGED, mNetworkInfo);
            pppoe_msg.sendToTarget();
            Log.e(TAG, "clear link pro");

            mLinkProperties = backupLinkP;
            pppoe_msg = mCsHandler.obtainMessage(EVENT_CONFIGURATION_CHANGED, mNetworkInfo);
            pppoe_msg.sendToTarget();
            Log.e(TAG, "reset link pro");
            */
            //reset emac ,reconnect ethernet service
            Boolean res;
            Teardown(&res);
            Reconnect(&res);

            mOldState = -1;
            mPppoe_ok = FALSE;
            break;

        default:
            Logger::D(TAG, "broadcast pppoe default:");
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
}
