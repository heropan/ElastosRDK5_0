
#include "ext/frameworkext.h"
#include "net/CNetworkState.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CNetworkState::constructor()
{
    return NOERROR;
}

ECode CNetworkState::constructor(
    /* [in] */ INetworkInfo* networkInfo,
    /* [in] */ ILinkProperties* linkProperties,
    /* [in] */ ILinkCapabilities* linkCapabilities)
{
    String str;
    constructor(networkInfo, linkProperties, linkCapabilities, str, str);
    return NOERROR;
}

ECode CNetworkState::constructor(
    /* [in] */ INetworkInfo* networkInfo,
    /* [in] */ ILinkProperties* linkProperties,
    /* [in] */ ILinkCapabilities* linkCapabilities,
    /* [in] */ const String& subscriberId,
    /* [in] */ const String& networkId)
{
    mNetworkInfo = networkInfo;
    mLinkProperties = linkProperties;
    mLinkCapabilities = linkCapabilities;
    mSubscriberId = subscriberId;
    mNetworkId = networkId;
    return NOERROR;
}

ECode CNetworkState::GetNetworkInfo(
    /* [out] */ INetworkInfo** networkInfo)
{
    VALIDATE_NOT_NULL(networkInfo);
    *networkInfo = mNetworkInfo;
    REFCOUNT_ADD(*networkInfo);
    return NOERROR;
}

ECode CNetworkState::GetLinkProperties(
    /* [out] */ ILinkProperties** linkProperties)
{
    VALIDATE_NOT_NULL(linkProperties);
    *linkProperties = mLinkProperties;
    REFCOUNT_ADD(*linkProperties);
    return NOERROR;
}

ECode CNetworkState::GetLinkCapabilities(
    /* [out] */ ILinkCapabilities** linkCapabilities)
{
    VALIDATE_NOT_NULL(linkCapabilities);
    *linkCapabilities = mLinkCapabilities;
    REFCOUNT_ADD(*linkCapabilities);
    return NOERROR;
}

ECode CNetworkState::GetSubscriberId(
    /* [out] */ String* subscriberId)
{
    VALIDATE_NOT_NULL(subscriberId);
    *subscriberId = mSubscriberId;
    return NOERROR;
}

ECode CNetworkState::GetNetworkId(
    /* [out] */ String* networkId)
{
    VALIDATE_NOT_NULL(networkId);
    *networkId = mNetworkId;
    return NOERROR;
}

//@Override
ECode CNetworkState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);
    dest->WriteInterfacePtr(mNetworkInfo);
    dest->WriteInterfacePtr(mLinkProperties);
    dest->WriteInterfacePtr(mLinkCapabilities);
    dest->WriteString(mSubscriberId);
    dest->WriteString(mNetworkId);
    return NOERROR;
}

ECode CNetworkState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);
    source->ReadInterfacePtr((Handle32*)&mNetworkInfo);
    source->ReadInterfacePtr((Handle32*)&mLinkProperties);
    source->ReadInterfacePtr((Handle32*)&mLinkCapabilities);
    source->ReadString(&mSubscriberId);
    source->ReadString(&mNetworkId);
    return NOERROR;
}


} // namespace Net
} // namepsace Droid
} // namespace Elastos
