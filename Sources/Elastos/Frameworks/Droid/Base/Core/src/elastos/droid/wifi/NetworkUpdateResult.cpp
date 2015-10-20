
#include "elastos/droid/net/wifi/NetworkUpdateResult.h"

namespace Elastos {
namespace Droid {
namespace Wifi {

NetworkUpdateResult::NetworkUpdateResult(
    /* [in] */ Int32 id)
    : mNetId(id)
    , mIpChanged(FALSE)
    , mProxyChanged(FALSE)
    , mIsNewNetwork(FALSE)
{}

NetworkUpdateResult::NetworkUpdateResult(
    /* [in] */ Boolean ip,
    /* [in] */ Boolean proxy)
    : mNetId(IWifiConfiguration::INVALID_NETWORK_ID)
    , mIpChanged(ip)
    , mProxyChanged(proxy)
    , mIsNewNetwork(FALSE)
{}

void NetworkUpdateResult::SetNetworkId(
    /* [in] */ Int32 id)
{
    mNetId = id;
}

Int32 NetworkUpdateResult::GetNetworkId()
{
    return mNetId;
}

void NetworkUpdateResult::SetIpChanged(
    /* [in] */ Boolean ip)
{
    mIpChanged = ip;
}

Boolean NetworkUpdateResult::HasIpChanged()
{
    return mIpChanged;
}

void NetworkUpdateResult::SetProxyChanged(
    /* [in] */ Boolean proxy)
{
    mProxyChanged = proxy;
}

Boolean NetworkUpdateResult::HasProxyChanged()
{
    return mProxyChanged;
}

Boolean NetworkUpdateResult::IsNewNetwork()
{
    return mIsNewNetwork;
}

void NetworkUpdateResult::SetIsNewNetwork(
    /* [in] */ Boolean isNew)
{
    mIsNewNetwork = isNew;
}

} // namespace Wifi
} // namespace Droid
} // namespace Elastos
