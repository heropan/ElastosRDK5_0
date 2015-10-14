
#include "elastos/droid/net/ScoredNetwork.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(ScoredNetwork, Object, IParcelable, IScoredNetwork)

ScoredNetwork::ScoredNetwork()
{}

ECode ScoredNetwork::constructor(
    /* [in] */ INetworkKey* networkKey,
    /* [in] */ IRssiCurve* rssiCurve)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.networkKey = networkKey;
        this.rssiCurve = rssiCurve;

#endif
}

ScoredNetwork::ScoredNetwork(
    /* [in] */ IParcel* in)
{
#if 0 // TODO: Translate codes below
        networkKey = NetworkKey.CREATOR.createFromParcel(in);
        if (in.readByte() == 1) {
            rssiCurve = RssiCurve.CREATOR.createFromParcel(in);
        } else {
            rssiCurve = null;
        }

#endif
}

ECode ScoredNetwork::DescribeContents(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return 0;

#endif
}

ECode ScoredNetwork::WriteToParcel(
    /* [in] */ IParcel* out,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        networkKey.writeToParcel(out, flags);
        if (rssiCurve != null) {
            out.writeByte((byte) 1);
            rssiCurve.writeToParcel(out, flags);
        } else {
            out.writeByte((byte) 0);
        }

#endif
}

ECode ScoredNetwork::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ScoredNetwork that = (ScoredNetwork) o;
        return Objects.equals(networkKey, that.networkKey) &&
                Objects.equals(rssiCurve, that.rssiCurve);

#endif
}

ECode ScoredNetwork::GetHashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return Objects.hash(networkKey, rssiCurve);

#endif
}

ECode ScoredNetwork::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return "ScoredNetwork[key=" + networkKey + ",score=" + rssiCurve + "]";

#endif
}

ECode ScoredNetwork::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                @Override
                public ScoredNetwork createFromParcel(Parcel in) {
                    return new ScoredNetwork(in);
                }
                @Override
                public ScoredNetwork[] newArray(int size) {
                    return new ScoredNetwork[size];
                }

#endif
}


ECode ScoredNetwork::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                @Override
                public ScoredNetwork createFromParcel(Parcel in) {
                    return new ScoredNetwork(in);
                }
                @Override
                public ScoredNetwork[] newArray(int size) {
                    return new ScoredNetwork[size];
                }

#endif
}

ECode ScoredNetwork::GetNetworkKey(
    /* [out] */ INetworkKey** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mNetworkKey;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode ScoredNetwork::GetRssiCurve(
    /* [out] */ IRssiCurve** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mRssiCurve;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
