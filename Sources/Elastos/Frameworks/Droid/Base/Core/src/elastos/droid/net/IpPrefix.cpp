
#include "elastos/droid/net/IpPrefix.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(IpPrefix, Object, IIpPrefix, IParcelable)

ECode IpPrefix::CheckAndMaskAddressAndPrefixLength()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (address.length != 4 && address.length != 16) {
            throw new IllegalArgumentException(
                    "IpPrefix has " + address.length + " bytes which is neither 4 nor 16");
        }
        NetworkUtils.maskRawAddress(address, prefixLength);

#endif
}

ECode IpPrefix::constructor(
    /* [in] */ ArrayOf<Byte>* address,
    /* [in] */ Int32 prefixLength)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.address = address.clone();
        this.prefixLength = prefixLength;
        checkAndMaskAddressAndPrefixLength();

#endif
}

ECode IpPrefix::constructor(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // We don't reuse the (byte[], int) constructor because it calls clone() on the byte array,
        // which is unnecessary because getAddress() already returns a clone.
        this.address = address.getAddress();
        this.prefixLength = prefixLength;
        checkAndMaskAddressAndPrefixLength();

#endif
}

ECode IpPrefix::constructor(
    /* [in] */ String prefix)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // We don't reuse the (InetAddress, int) constructor because "error: call to this must be
        // first statement in constructor". We could factor out setting the member variables to an
        // init() method, but if we did, then we'd have to make the members non-final, or "error:
        // cannot assign a value to final variable address". So we just duplicate the code here.
        Pair<InetAddress, Integer> ipAndMask = NetworkUtils.parseIpAndMask(prefix);
        this.address = ipAndMask.first.getAddress();
        this.prefixLength = ipAndMask.second;
        checkAndMaskAddressAndPrefixLength();

#endif
}

ECode IpPrefix::Equals(
    /* [in] */ IObject* obj,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!(obj instanceof IpPrefix)) {
            return false;
        }
        IpPrefix that = (IpPrefix) obj;
        return Arrays.equals(this.address, that.address) && this.prefixLength == that.prefixLength;

#endif
}

ECode IpPrefix::HashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return Arrays.hashCode(address) + 11 * prefixLength;

#endif
}

ECode IpPrefix::GetAddress(
    /* [out] */ IInetAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            return InetAddress.getByAddress(address);
        } catch (UnknownHostException e) {
            // Cannot happen. InetAddress.getByAddress can only throw an exception if the byte
            // array is the wrong length, but we check that in the constructor.
            return null;
        }

#endif
}

ECode IpPrefix::GetRawAddress(
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return address.clone();

#endif
}

ECode IpPrefix::GetPrefixLength(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return prefixLength;

#endif
}

ECode IpPrefix::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            return InetAddress.getByAddress(address).getHostAddress() + "/" + prefixLength;
        } catch(UnknownHostException e) {
            // Cosmic rays?
            throw new IllegalStateException("IpPrefix with invalid address! Shouldn't happen.", e);
        }

#endif
}

ECode IpPrefix::DescribeContents(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return 0;

#endif
}

ECode IpPrefix::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        dest.writeByteArray(address);
        dest.writeInt(prefixLength);

#endif
}

ECode IpPrefix::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                public IpPrefix createFromParcel(Parcel in) {
                    byte[] address = in.createByteArray();
                    int prefixLength = in.readInt();
                    return new IpPrefix(address, prefixLength);
                }
                public IpPrefix[] newArray(int size) {
                    return new IpPrefix[size];
                }

#endif
}

ECode IpPrefix::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                public IpPrefix createFromParcel(Parcel in) {
                    byte[] address = in.createByteArray();
                    int prefixLength = in.readInt();
                    return new IpPrefix(address, prefixLength);
                }
                public IpPrefix[] newArray(int size) {
                    return new IpPrefix[size];
                }

#endif
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
