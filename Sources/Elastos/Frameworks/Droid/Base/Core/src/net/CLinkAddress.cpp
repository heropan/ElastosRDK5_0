
#include "net/CLinkAddress.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Net::IInet4Address;
using Elastos::Net::IInetAddressHelper;
using Elastos::Net::CInetAddressHelper;

namespace Elastos {
namespace Droid {
namespace Net {

ECode CLinkAddress::constructor()
{
    return NOERROR;
}

ECode CLinkAddress::constructor(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength)
{
    if (address == NULL || prefixLength < 0 ||
            ((IInet4Address::Probe(address) != NULL) && prefixLength > 32) ||
            (prefixLength > 128)) {
//        throw new IllegalArgumentException("Bad LinkAddress params " + address +
//                prefixLength);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mAddress = address;
    mPrefixLength = prefixLength;
    return NOERROR;
}

ECode CLinkAddress::constructor(
    /* [in] */ IInterfaceAddress* interfaceAddress)
{
    interfaceAddress->GetAddress((IInetAddress**)&mAddress);
    Int16 len;
    interfaceAddress->GetNetworkPrefixLength(&len);
    mPrefixLength = (Int32)len;
    return NOERROR;
}

//@Override
ECode CLinkAddress::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    if (mAddress == NULL) {
        *result = "";
    }
    else {
        String addr;
        mAddress->GetHostAddress(&addr);
        StringBuilder builder(addr);
        builder += "/";
        builder += StringUtils::Int32ToString(mPrefixLength);
        *result = builder.ToString();
    }
    return NOERROR;
}

/**
 * Compares this {@code LinkAddress} instance against the specified address
 * in {@code obj}. Two addresses are equal if their InetAddress and prefixLength
 * are equal
 *
 * @param obj the object to be tested for equality.
 * @return {@code true} if both objects are equal, {@code FALSE} otherwise.
 */
//@Override
ECode CLinkAddress::Equals(
    /* [in] */ ILinkAddress* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(obj);

    AutoPtr<IInetAddress> addr;
    Int32 len;
    obj->GetAddress((IInetAddress**)&addr);
    obj->GetNetworkPrefixLength(&len);
    Boolean isEqual;
    mAddress->Equals(addr, &isEqual);
    *result = isEqual && (mPrefixLength == len);
    return NOERROR;
}

ECode CLinkAddress::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Equals(ILinkAddress::Probe(obj), result);
}

/*
 * generate hashcode based on significant fields
 */
 //@Override
ECode CLinkAddress::GetHashCode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 hashcode = 0;
    if (mAddress) {
        mAddress->GetHashCode(&hashcode);
    }
    *result = hashcode + mPrefixLength;
    return NOERROR;
}

/**
 * Returns the InetAddress for this address.
 */
ECode CLinkAddress::GetAddress(
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mAddress;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Get network prefix length
 */
ECode CLinkAddress::GetNetworkPrefixLength(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mPrefixLength;
    return NOERROR;
}

ECode CLinkAddress::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    Byte b;
    source->ReadByte(&b);
    if (b== 1) {
        AutoPtr< ArrayOf<Byte> > array;
        source->ReadArrayOf((Handle32*)&array);
        AutoPtr<IInetAddressHelper> inetaddresshelper;
        CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&inetaddresshelper);
        inetaddresshelper->GetByAddress(array, (IInetAddress**)&mAddress);
        source->ReadInt32(&mPrefixLength);
    }
    return NOERROR;
}

/**
 * Implement the Parcelable interface.
 * @hide
 */
ECode CLinkAddress::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    if (mAddress != NULL) {
        dest->WriteByte(1);
        AutoPtr< ArrayOf<Byte> > addrs;
        mAddress->GetAddress((ArrayOf<Byte>**)&addrs);
        dest->WriteArrayOf((Handle32)addrs.Get());
        dest->WriteInt32(mPrefixLength);
    }
    else {
        dest->WriteByte(0);
    }
    return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
