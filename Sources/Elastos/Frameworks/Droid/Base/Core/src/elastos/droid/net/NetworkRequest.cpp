
#include "elastos/droid/net/NetworkRequest.h"
#include "elastos/droid/net/CNetworkCapabilities.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(NetworkRequest, Object, IParcelable, INetworkRequest)

ECode NetworkRequest::constructor(
    /* [in] */ INetworkCapabilities* nc,
    /* [in] */ Int32 legacyType,
    /* [in] */ Int32 rId)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (nc == null) {
            throw new NullPointerException();
        }
        requestId = rId;
        networkCapabilities = nc;
        this.legacyType = legacyType;

#endif
}

ECode NetworkRequest::constructor(
    /* [in] */ INetworkRequest* that)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        networkCapabilities = new NetworkCapabilities(that.networkCapabilities);
        requestId = that.requestId;
        this.legacyType = that.legacyType;

#endif
}

ECode NetworkRequest::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public NetworkRequest createFromParcel(Parcel in) {
                NetworkCapabilities nc = (NetworkCapabilities)in.readParcelable(null);
                int legacyType = in.readInt();
                int requestId = in.readInt();
                NetworkRequest result = new NetworkRequest(nc, legacyType, requestId);
                return result;
            }
            public NetworkRequest[] newArray(int size) {
                return new NetworkRequest[size];
            }

#endif
}

ECode NetworkRequest::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public NetworkRequest createFromParcel(Parcel in) {
                NetworkCapabilities nc = (NetworkCapabilities)in.readParcelable(null);
                int legacyType = in.readInt();
                int requestId = in.readInt();
                NetworkRequest result = new NetworkRequest(nc, legacyType, requestId);
                return result;
            }
            public NetworkRequest[] newArray(int size) {
                return new NetworkRequest[size];
            }

#endif
}

ECode NetworkRequest::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return "NetworkRequest [ id=" + requestId + ", legacyType=" + legacyType +
                ", " + networkCapabilities.toString() + " ]";

#endif
}

ECode NetworkRequest::Equals(
    /* [in] */ IObject* obj,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (obj instanceof NetworkRequest == false) return false;
        NetworkRequest that = (NetworkRequest)obj;
        return (that.legacyType == this.legacyType &&
                that.requestId == this.requestId &&
                ((that.networkCapabilities == null && this.networkCapabilities == null) ||
                 (that.networkCapabilities != null &&
                  that.networkCapabilities.equals(this.networkCapabilities))));

#endif
}

ECode NetworkRequest::GetHashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return requestId + (legacyType * 1013) +
                (networkCapabilities.hashCode() * 1051);

#endif
}

ECode NetworkRequest::GetNetworkCapabilities(
    /* [out] */ INetworkCapabilities** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mNetworkCapabilities;
    return NOERROR;
}

ECode NetworkRequest::GetRequestId(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mRequestId;
    return NOERROR;
}

ECode NetworkRequest::GetLegacyType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mLegacyType;
    return NOERROR;
}

//============================================================
// NetworkRequestBuilder
//============================================================

CAR_INTERFACE_IMPL(NetworkRequestBuilder, Object, INetworkRequestBuilder)

NetworkRequestBuilder::NetworkRequestBuilder()
    : mNetworkCapabilities(CreateNetworkCapabilities())
{}

ECode NetworkRequestBuilder::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
#endif
}

ECode NetworkRequestBuilder::Build(
    /* [out] */ INetworkRequest** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            return new NetworkRequest(mNetworkCapabilities, ConnectivityManager.TYPE_NONE,
                    ConnectivityManager.REQUEST_ID_UNSET);

#endif
}

ECode NetworkRequestBuilder::AddCapability(
    /* [in] */ Int32 capability,
    /* [out] */ INetworkRequestBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            mNetworkCapabilities.addCapability(capability);
            return this;

#endif
}

ECode NetworkRequestBuilder::RemoveCapability(
    /* [in] */ Int32 capability,
    /* [out] */ INetworkRequestBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            mNetworkCapabilities.removeCapability(capability);
            return this;

#endif
}

ECode NetworkRequestBuilder::AddTransportType(
    /* [in] */ Int32 transportType,
    /* [out] */ INetworkRequestBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            mNetworkCapabilities.addTransportType(transportType);
            return this;

#endif
}

ECode NetworkRequestBuilder::RemoveTransportType(
    /* [in] */ Int32 transportType,
    /* [out] */ INetworkRequestBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            mNetworkCapabilities.removeTransportType(transportType);
            return this;

#endif
}

ECode NetworkRequestBuilder::SetLinkUpstreamBandwidthKbps(
    /* [in] */ Int32 upKbps,
    /* [out] */ INetworkRequestBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            mNetworkCapabilities.setLinkUpstreamBandwidthKbps(upKbps);
            return this;

#endif
}

ECode NetworkRequestBuilder::SetLinkDownstreamBandwidthKbps(
    /* [in] */ Int32 downKbps,
    /* [out] */ INetworkRequestBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            mNetworkCapabilities.setLinkDownstreamBandwidthKbps(downKbps);
            return this;

#endif
}

ECode NetworkRequestBuilder::SetNetworkSpecifier(
    /* [in] */ const String& networkSpecifier,
    /* [out] */ INetworkRequestBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            mNetworkCapabilities.setNetworkSpecifier(networkSpecifier);
            return this;

#endif
}

AutoPtr<INetworkCapabilities> NetworkRequestBuilder::CreateNetworkCapabilities()
{
    AutoPtr<INetworkCapabilities> rev;
    CNetworkCapabilities::New((INetworkCapabilities**)&rev);
    return rev;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
