
#include "elastos/droid/net/NetworkState.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(NetworkState, Object, IParcelable, INetworkState)

ECode NetworkState::constructor(
    /* [in] */ INetworkInfo* networkInfo,
    /* [in] */ ILinkProperties* linkProperties,
    /* [in] */ INetworkCapabilities* networkCapabilities)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(networkInfo, linkProperties, networkCapabilities, NULL, NULL);
#endif
}

ECode NetworkState::constructor(
    /* [in] */ INetworkInfo* networkInfo,
    /* [in] */ ILinkProperties* linkProperties,
    /* [in] */ INetworkCapabilities* networkCapabilities,
    /* [in] */ const String& subscriberId,
    /* [in] */ const String& networkId)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.networkInfo = networkInfo;
        this.linkProperties = linkProperties;
        this.networkCapabilities = networkCapabilities;
        this.subscriberId = subscriberId;
        this.networkId = networkId;
#endif
}

ECode NetworkState::constructor(
    /* [in] */ IParcel* in)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        networkInfo = in.readParcelable(NULL);
        linkProperties = in.readParcelable(NULL);
        networkCapabilities = in.readParcelable(NULL);
        subscriberId = in.readString();
        networkId = in.readString();
#endif
}

ECode NetworkState::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(source);
    source->ReadInterfacePtr((Handle32*)&mNetworkInfo);
    source->ReadInterfacePtr((Handle32*)&mLinkProperties);
    source->ReadInterfacePtr((Handle32*)&mLinkCapabilities);
    source->ReadString(&mSubscriberId);
    source->ReadString(&mNetworkId);
    return NOERROR;
#endif
}

ECode NetworkState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(dest);
    dest->WriteInterfacePtr(mNetworkInfo);
    dest->WriteInterfacePtr(mLinkProperties);
    dest->WriteInterfacePtr(mLinkCapabilities);
    dest->WriteString(mSubscriberId);
    dest->WriteString(mNetworkId);
    return NOERROR;
#endif
}

ECode NetworkState::GetNetworkInfo(
    /* [out] */ INetworkInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(networkInfo);
    *networkInfo = mNetworkInfo;
    REFCOUNT_ADD(*networkInfo);
    return NOERROR;
#endif
}

ECode NetworkState::GetLinkProperties(
    /* [out] */ ILinkProperties** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(linkProperties);
    *linkProperties = mLinkProperties;
    REFCOUNT_ADD(*linkProperties);
    return NOERROR;
#endif
}

ECode NetworkState::GetNetworkCapabilities(
    /* [out] */ INetworkCapabilities** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mNetworkCapabilities;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode NetworkState::GetSubscriberId(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(subscriberId);
    *subscriberId = mSubscriberId;
    return NOERROR;
#endif
}

ECode NetworkState::GetNetworkId(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(networkId);
    *networkId = mNetworkId;
    return NOERROR;
#endif
}

} // namespace Net
} // namespace Droid
} // namespace Elastos

#if 0 // old CNetworkState.cpp
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
#endif