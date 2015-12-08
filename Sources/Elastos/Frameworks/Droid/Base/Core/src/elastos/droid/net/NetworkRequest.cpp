
#include "elastos/droid/net/CNetworkCapabilities.h"
#include "elastos/droid/net/NetworkRequest.h"
#include "elastos/droid/net/ReturnOutValue.h"
#include "elastos/droid/net/CNetworkCapabilities.h"
#include "elastos/droid/net/CNetworkRequest.h"

using Elastos::Droid::Net::IConnectivityManager;

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(NetworkRequest, Object, IParcelable, INetworkRequest)

ECode NetworkRequest::constructor(
    /* [in] */ INetworkCapabilities* nc,
    /* [in] */ Int32 legacyType,
    /* [in] */ Int32 rId)
{
    if (nc == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mRequestId = rId;
    mNetworkCapabilities = nc;
    mLegacyType = legacyType;
    return NOERROR;
}

ECode NetworkRequest::constructor(
    /* [in] */ INetworkRequest* that)
{
    CNetworkCapabilities::New(((NetworkRequest*)that)->mNetworkCapabilities, (INetworkCapabilities**)&mNetworkCapabilities);
    mRequestId = ((NetworkRequest*)that)->mRequestId;
    mLegacyType = ((NetworkRequest*)that)->mLegacyType;
    return NOERROR;
}

ECode NetworkRequest::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    AutoPtr<IInterface> obj;
    parcel->ReadInterfacePtr((Handle32*)&obj);
    mNetworkCapabilities = INetworkCapabilities::Probe(obj);
    parcel->ReadInt32(&mRequestId);
    parcel->ReadInt32(&mLegacyType);
    return NOERROR;
}

ECode NetworkRequest::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInterfacePtr(mNetworkCapabilities);
    dest->WriteInt32(mRequestId);
    dest->WriteInt32(mLegacyType);
    return NOERROR;
}

ECode NetworkRequest::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    *result = String("NetworkRequest [ id=");
    String s;
    IObject::Probe(mNetworkCapabilities)->ToString(&s);
    result->AppendFormat("%d, legacyType=%d, %s ]", mRequestId, mLegacyType, s.string());
    return NOERROR;
}

ECode NetworkRequest::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    if (TO_IINTERFACE(this) != IInterface::Probe(obj)) FUNC_RETURN(FALSE);
    if (INetworkRequest::Probe(obj) == NULL) FUNC_RETURN(FALSE);
    NetworkRequest* that = (NetworkRequest*)(INetworkRequest::Probe(obj));
    Boolean bEquals;
    IObject::Probe(that->mNetworkCapabilities)->Equals(mNetworkCapabilities, &bEquals);
    FUNC_RETURN(that->mLegacyType == mLegacyType &&
            that->mRequestId == mRequestId &&
            ((that->mNetworkCapabilities == NULL && mNetworkCapabilities == NULL) ||
            (that->mNetworkCapabilities != NULL && bEquals)));
}

ECode NetworkRequest::GetHashCode(
    /* [out] */ Int32* result)
{
    Int32 hashCode;
    IObject::Probe(mNetworkCapabilities)->GetHashCode(&hashCode);
    FUNC_RETURN(mRequestId + (mLegacyType * 1013) + hashCode * 1051);
}

ECode NetworkRequest::GetNetworkCapabilities(
    /* [out] */ INetworkCapabilities** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mNetworkCapabilities;
    REFCOUNT_ADD(*result)
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
    return NOERROR;
}

ECode NetworkRequestBuilder::Build(
    /* [out] */ INetworkRequest** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<INetworkRequest> rev;
    CNetworkRequest::New(mNetworkCapabilities, IConnectivityManager::TYPE_NONE,
            IConnectivityManager::REQUEST_ID_UNSET, (INetworkRequest**)&rev);
    FUNC_RETURN(rev);
}

ECode NetworkRequestBuilder::AddCapability(
    /* [in] */ Int32 capability)
{
    mNetworkCapabilities->AddCapability(capability);
    return NOERROR;
}

ECode NetworkRequestBuilder::RemoveCapability(
    /* [in] */ Int32 capability)
{
    mNetworkCapabilities->RemoveCapability(capability);
    return NOERROR;
}

ECode NetworkRequestBuilder::AddTransportType(
    /* [in] */ Int32 transportType)
{
    mNetworkCapabilities->AddTransportType(transportType);
    return NOERROR;
}

ECode NetworkRequestBuilder::RemoveTransportType(
    /* [in] */ Int32 transportType)
{
    mNetworkCapabilities->RemoveTransportType(transportType);
    return NOERROR;
}

ECode NetworkRequestBuilder::SetLinkUpstreamBandwidthKbps(
    /* [in] */ Int32 upKbps)
{
    mNetworkCapabilities->SetLinkUpstreamBandwidthKbps(upKbps);
    return NOERROR;
}

ECode NetworkRequestBuilder::SetLinkDownstreamBandwidthKbps(
    /* [in] */ Int32 downKbps)
{
    mNetworkCapabilities->SetLinkDownstreamBandwidthKbps(downKbps);
    return NOERROR;
}

ECode NetworkRequestBuilder::SetNetworkSpecifier(
    /* [in] */ const String& networkSpecifier)
{
    mNetworkCapabilities->SetNetworkSpecifier(networkSpecifier);
    return NOERROR;
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
