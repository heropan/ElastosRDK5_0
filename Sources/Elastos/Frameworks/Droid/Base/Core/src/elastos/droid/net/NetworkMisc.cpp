
#include "elastos/droid/net/NetworkMisc.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(NetworkMisc, Object, IParcelable, INetworkMisc)

ECode NetworkMisc::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below

#endif
}

ECode NetworkMisc::constructor(
    /* [in] */ INetworkMisc* nm)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (nm != null) {
            allowBypass = nm.allowBypass;
            explicitlySelected = nm.explicitlySelected;
        }

#endif
}

ECode NetworkMisc::DescribeContents(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return 0;

#endif
}

ECode NetworkMisc::WriteToParcel(
    /* [in] */ IParcel* out,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        out.writeInt(allowBypass ? 1 : 0);
        out.writeInt(explicitlySelected ? 1 : 0);

#endif
}
ECode NetworkMisc::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        @Override
        public NetworkMisc createFromParcel(Parcel in) {
            NetworkMisc networkMisc = new NetworkMisc();
            networkMisc.allowBypass = in.readInt() != 0;
            networkMisc.explicitlySelected = in.readInt() != 0;
            return networkMisc;
        }
        @Override
        public NetworkMisc[] newArray(int size) {
            return new NetworkMisc[size];
        }

#endif
}

ECode NetworkMisc::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        @Override
        public NetworkMisc createFromParcel(Parcel in) {
            NetworkMisc networkMisc = new NetworkMisc();
            networkMisc.allowBypass = in.readInt() != 0;
            networkMisc.explicitlySelected = in.readInt() != 0;
            return networkMisc;
        }
        @Override
        public NetworkMisc[] newArray(int size) {
            return new NetworkMisc[size];
        }

#endif
}

ECode NetworkMisc::GetAllowBypass(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mAllowBypass;
    return NOERROR;
}

ECode NetworkMisc::SetAllowBypass(
    /* [in] */ Boolean allowBypass)
{
    mAllowBypass = allowBypass;
    return NOERROR;
}

ECode NetworkMisc::GetExplicitlySelected(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mExplicitlySelected;
    return NOERROR;
}

ECode NetworkMisc::SetExplicitlySelected(
    /* [in] */ Boolean explicitlySelected)
{
    mExplicitlySelected = explicitlySelected;
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
