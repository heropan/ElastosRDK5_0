
#include "net/BaseNetworkStateTracker.h"

using Elastos::Utility::Concurrent::Atomic::CAtomicBoolean;
using Elastos::Droid::Os::IMessage;

namespace Elastos {
namespace Droid {
namespace Net {

const String BaseNetworkStateTracker::PROP_TCP_BUFFER_UNKNOWN("net.tcp.buffersize.unknown");
const String BaseNetworkStateTracker::PROP_TCP_BUFFER_WIFI("net.tcp.buffersize.wifi");

BaseNetworkStateTracker::BaseNetworkStateTracker(
    /* [in] */ Int32 networkType)
{
    CAtomicBoolean::New((IAtomicBoolean**)&mTeardownRequested);
    CAtomicBoolean::New((IAtomicBoolean**)&mPrivateDnsRouteSet);
    CAtomicBoolean::New((IAtomicBoolean**)&mDefaultRouteSet);

    AutoPtr<IConnectivityManagerHelper> CMHelper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&CMHelper);
    String typeName;
    CMHelper->GetNetworkTypeName(networkType, &typeName);
    CNetworkInfo::New(networkType, -1, typeName, String(NULL), (INetworkInfo**)&mNetworkInfo);
    CLinkProperties::New((ILinkProperties**)&mLinkProperties);
    CLinkCapabilities::New((ILinkCapabilities**)&mLinkCapabilities);
}

PInterface BaseNetworkStateTracker::Probe(
    /* [in] */ REIID riid)
{
    assert(0);
    return NULL;
}

UInt32 BaseNetworkStateTracker::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 BaseNetworkStateTracker::Release()
{
    return ElRefBase::Release();
}

ECode BaseNetworkStateTracker::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    assert(0);
    return NOERROR;
}

AutoPtr<IHandler> BaseNetworkStateTracker::GetTargetHandler()
{
    return mTarget;
}

void BaseNetworkStateTracker::DispatchStateChanged()
{
    // TODO: include snapshot of other fields when sending
    AutoPtr<INetworkInfo> netInfo;
    GetNetworkInfo((INetworkInfo**)&netInfo);
    AutoPtr<IMessage> msg;
    mTarget->ObtainMessageEx(EVENT_STATE_CHANGED, netInfo, (IMessage**)&msg);
    msg->SendToTarget();
}

void BaseNetworkStateTracker::DispatchConfigurationChanged()
{
    // TODO: include snapshot of other fields when sending
    AutoPtr<INetworkInfo> netInfo;
    GetNetworkInfo((INetworkInfo**)&netInfo);
    AutoPtr<IMessage> msg;
    mTarget->ObtainMessageEx(EVENT_CONFIGURATION_CHANGED, netInfo, (IMessage**)&msg);
    msg->SendToTarget();
}

ECode BaseNetworkStateTracker::StartMonitoring(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* target)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(target);
    mContext = context;
    mTarget = target;
    StartMonitoringInternal();
    return NOERROR;
}

ECode BaseNetworkStateTracker::GetNetworkInfo(
    /* [out] */ INetworkInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return CNetworkInfo::New(mNetworkInfo, info);
}

ECode BaseNetworkStateTracker::GetLinkProperties(
    /* [out] */ ILinkProperties** result)
{
    VALIDATE_NOT_NULL(result);
    return CLinkProperties::New(mLinkProperties, result);
}

ECode BaseNetworkStateTracker::GetLinkCapabilities(
    /* [out] */ ILinkCapabilities** result)
{
    VALIDATE_NOT_NULL(result);
    return CLinkCapabilities::New(mLinkCapabilities, result);
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

} // namespace Net
} // namepsace Droid
} // namespace Elastos
