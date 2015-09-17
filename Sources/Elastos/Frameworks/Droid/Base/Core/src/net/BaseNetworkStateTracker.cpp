
#include "net/BaseNetworkStateTracker.h"

using Elastos::Utility::Concurrent::Atomic::CAtomicBoolean;
using Elastos::Droid::Os::IMessage;

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(BaseNetworkStateTracker, Object, INetworkStateTracker, IBaseNetworkStateTracker)

BaseNetworkStateTracker::BaseNetworkStateTracker()
    : mContext(NULL)
    , mNetworkInfo(NULL)
    , mLinkProperties(NULL)
    , mNetworkCapabilities(NULL)
    , mTarget(NULL)
{
#if 0 // TODO: Waiting for INetwork
    CNetwork::New(IConnectivityManager::NETID_UNSET, (INetwork**)&mNetwork);
#else
    assert(0);
#endif
    CAtomicBoolean::New((IAtomicBoolean**)&mTeardownRequested);
    CAtomicBoolean::New((IAtomicBoolean**)&mPrivateDnsRouteSet);
    CAtomicBoolean::New((IAtomicBoolean**)&mDefaultRouteSet);
}

ECode BaseNetworkStateTracker::constructor()
{
    // By default, let the sub classes construct everything
    return NOERROR;
}

ECode BaseNetworkStateTracker::constructor(
    /* [in] */ Int32 networkType)
{

    AutoPtr<IConnectivityManagerHelper> CMHelper;
#if 0 // TODO: Waiting for IConnectivityManagerHelper
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&CMHelper);
#else
    assert(0);
#endif
    String typeName;
    CMHelper->GetNetworkTypeName(networkType, &typeName);
#if 0 // TODO: Waiting for INetworkInfo, ILinkProperties, ILinkCapabilities
    CNetworkInfo::New(networkType, -1, typeName, String(NULL), (INetworkInfo**)&mNetworkInfo);
    CLinkProperties::New((ILinkProperties**)&mLinkProperties);
    CLinkCapabilities::New((ILinkCapabilities**)&mLinkCapabilities);
#else
    assert(0);
#endif

    return NOERROR;
}

ECode BaseNetworkStateTracker::GetTargetHandler(
    /* [out] */ IHandler** handler)
{
    VALIDATE_NOT_NULL(*handler)

    *handler = mTarget;
    REFCOUNT_ADD(*handler)
    return NOERROR;
}

ECode BaseNetworkStateTracker::DispatchStateChanged()
{
    // TODO: include snapshot of other fields when sending
    AutoPtr<INetworkInfo> netInfo;
    GetNetworkInfo((INetworkInfo**)&netInfo);
    AutoPtr<IMessage> msg;
    mTarget->ObtainMessage(EVENT_STATE_CHANGED, netInfo, (IMessage**)&msg);
    return msg->SendToTarget();
}

ECode BaseNetworkStateTracker::DispatchConfigurationChanged()
{
    // TODO: include snapshot of other fields when sending
    AutoPtr<INetworkInfo> netInfo;
    GetNetworkInfo((INetworkInfo**)&netInfo);
    AutoPtr<IMessage> msg;
    mTarget->ObtainMessage(EVENT_CONFIGURATION_CHANGED, netInfo, (IMessage**)&msg);
    return msg->SendToTarget();
}

ECode BaseNetworkStateTracker::StartMonitoring(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* target)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(target);

    mContext = context;
    mTarget = target;
    return StartMonitoringInternal();
}

ECode BaseNetworkStateTracker::StartMonitoringInternal()
{
    return NOERROR;
}

ECode BaseNetworkStateTracker::GetNetworkInfo(
    /* [out] */ INetworkInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;
#if 0 // TODO: Waiting for INetworkInfo
    return CNetworkInfo::New(mNetworkInfo, info);
#else
    assert(0);
    return NOERROR;
#endif
}

ECode BaseNetworkStateTracker::GetLinkProperties(
    /* [out] */ ILinkProperties** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

#if 0 // TODO: Waiting for ILinkProperties
    return CLinkProperties::New(mLinkProperties, result);
#else
    assert(0);
    return NOERROR;
#endif
}

ECode BaseNetworkStateTracker::GetLinkCapabilities(
    /* [out] */ ILinkCapabilities** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

#if 0 // TODO: Waiting for ILinkCapabilities
    return CLinkCapabilities::New(mLinkCapabilities, result);
#else
    assert(0);
    return NOERROR;
#endif
}

ECode BaseNetworkStateTracker::GetLinkQualityInfo(
        /* [out] */ ILinkQualityInfo** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;

    return NOERROR;
}

ECode BaseNetworkStateTracker::CaptivePortalCheckComplete()
{
    // not implemented
    return NOERROR;
}

ECode BaseNetworkStateTracker::SetRadio(
    /* [in] */ Boolean turnOn,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // Base tracker doesn't handle radios
    *result = TRUE;
    return NOERROR;
}

ECode BaseNetworkStateTracker::IsAvailable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    return mNetworkInfo->IsAvailable(result);
}

ECode BaseNetworkStateTracker::SetUserDataEnable(
    /* [in] */ Boolean enabled)
{
    // Base tracker doesn't handle enabled flags
    return NOERROR;
}

ECode BaseNetworkStateTracker::SetPolicyDataEnable(
    /* [in] */ Boolean enabled)
{
    // Base tracker doesn't handle enabled flags
    return NOERROR;
}

ECode BaseNetworkStateTracker::IsPrivateDnsRouteSet(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    return mPrivateDnsRouteSet->Get(result);
}

ECode BaseNetworkStateTracker::SetPrivateDnsRoute(
    /* [in] */ Boolean enabled)
{
    return mPrivateDnsRouteSet->Set(enabled);
}

ECode BaseNetworkStateTracker::IsDefaultRouteSet(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    return mDefaultRouteSet->Get(result);
}

ECode BaseNetworkStateTracker::SetDefaultRoute(
    /* [in] */ Boolean enabled)
{
    return mDefaultRouteSet->Set(enabled);
}

ECode BaseNetworkStateTracker::IsTeardownRequested(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    return mTeardownRequested->Get(result);
}

ECode BaseNetworkStateTracker::SetTeardownRequested(
    /* [in] */ Boolean isRequested)
{
    return mTeardownRequested->Set(isRequested);
}

ECode BaseNetworkStateTracker::SetDependencyMet(
    /* [in] */ Boolean met)
{
    // Base tracker doesn't handle dependencies
    return NOERROR;
}

ECode BaseNetworkStateTracker::AddStackedLink(
        /* [in] */ ILinkProperties* link)
{
    VALIDATE_NOT_NULL(link)

#if 0 // TODO: Waiting for ILinkProperties
    return mLinkProperties->AddStackedLink(link);
#else
    assert(0);
    return NOERROR;
#endif
}

ECode BaseNetworkStateTracker::RemoveStackedLink(
    /* [in] */ ILinkProperties* link)
{
    VALIDATE_NOT_NULL(link)

#if 0 // TODO: Waiting for ILinkProperties
    return mLinkProperties->RemoveStackedLink(link);
#else
    assert(0);
    return NOERROR;
#endif
}


ECode BaseNetworkStateTracker::SupplyMessenger(
    /* [in] */ IMessenger* messenger)
{
    VALIDATE_NOT_NULL(messenger)

    // not supported on this network
    return NOERROR;
}


ECode BaseNetworkStateTracker::GetNetworkInterfaceName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = String(NULL);

    if (mLinkProperties != NULL) {
        return mLinkProperties->GetInterfaceName(result);
    }
    return NOERROR;
}


ECode BaseNetworkStateTracker::StartSampling(
    /* [in] */ ISamplingDataTrackerSamplingSnapshot* s)
{
    VALIDATE_NOT_NULL(s)

    // nothing to do
    return NOERROR;
}


ECode BaseNetworkStateTracker::StopSampling(
    /* [in] */ ISamplingDataTrackerSamplingSnapshot* s)
{
    VALIDATE_NOT_NULL(s)

    // nothing to do
    return NOERROR;
}


ECode BaseNetworkStateTracker::SetNetId(
    /* [in] */ Int32 netId)
{
#if 0 // TODO: Waiting for INetwork
    return CNetwork::New(netId, (INetwork**)&mNetwork);
#else
    assert(0);
    return NOERROR;
#endif
}

ECode BaseNetworkStateTracker::GetNetwork(
    /* [out] */ INetwork** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mNetwork;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode BaseNetworkStateTracker::GetNetworkCapabilities(
    /* [out] */ INetworkCapabilities** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;

#if 0 // TODO: Waiting for CNetworkCapabilities
    CNetworkCapabilities::New(mNetworkCapabilities, result);
#else
    assert(0);
#endif
    return NOERROR;
}


} // namespace Net
} // namepsace Droid
} // namespace Elastos
