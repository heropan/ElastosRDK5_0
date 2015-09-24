
#include "elastos/droid/net/NetworkKey.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(NetworkKey, Object, IParcelable, INetworkKey)

ECode NetworkKey::constructor(
    /* [in] */ IWifiKey* wifiKey)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.type = TYPE_WIFI;
        this.wifiKey = wifiKey;

#endif
}

NetworkKey::NetworkKey()
{}

NetworkKey::NetworkKey(
    /* [in] */ IParcel* in)
{
#if 0 // TODO: Translate codes below
        type = in.readInt();
        switch (type) {
            case TYPE_WIFI:
                wifiKey = WifiKey.CREATOR.createFromParcel(in);
                break;
            default:
                throw new IllegalArgumentException("Parcel has unknown type: " + type);
        }

#endif
}

ECode NetworkKey::DescribeContents(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return 0;

#endif
}

ECode NetworkKey::WriteToParcel(
    /* [in] */ IParcel* out,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        out.writeInt(type);
        switch (type) {
            case TYPE_WIFI:
                wifiKey.writeToParcel(out, flags);
                break;
            default:
                throw new IllegalStateException("NetworkKey has unknown type " + type);
        }

#endif
}

ECode NetworkKey::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        NetworkKey that = (NetworkKey) o;
        return type == that.type && Objects.equals(wifiKey, that.wifiKey);

#endif
}

ECode NetworkKey::GetHashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return Objects.hash(type, wifiKey);

#endif
}

ECode NetworkKey::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        switch (type) {
            case TYPE_WIFI:
                return wifiKey.toString();
            default:
                // Don't throw an exception here in case someone is logging this object in a catch
                // block for debugging purposes.
                return "InvalidKey";
        }

#endif
}

ECode NetworkKey::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                @Override
                public NetworkKey createFromParcel(Parcel in) {
                    return new NetworkKey(in);
                }
                @Override
                public NetworkKey[] newArray(int size) {
                    return new NetworkKey[size];
                }

#endif
}

ECode NetworkKey::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                @Override
                public NetworkKey createFromParcel(Parcel in) {
                    return new NetworkKey(in);
                }
                @Override
                public NetworkKey[] newArray(int size) {
                    return new NetworkKey[size];
                }

#endif
}

ECode NetworkKey::GetType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mType;
    return NOERROR;
}

ECode NetworkKey::GetWifiKey(
    /* [out] */ IWifiKey** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mWifiKey;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
