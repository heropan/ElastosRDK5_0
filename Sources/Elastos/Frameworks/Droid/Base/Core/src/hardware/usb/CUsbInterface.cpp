
#include "hardware/usb/CUsbInterface.h"
#include <elastos/core/StringBuffer.h>

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Usb {

ECode CUsbInterface::constructor()
{
    return NOERROR;
}

ECode CUsbInterface::constructor(
    /* [in] */ Int32 id,
    /* [in] */ Int32 cls,
    /* [in] */ Int32 subCls,
    /* [in] */ Int32 protocol,
    /* [in] */ const ArrayOf<IParcelable*>& endpoints)
{
    mId = id;
    mClass = cls;
    mSubclass = subCls;
    mProtocol = protocol;

    mEndpoints = ArrayOf< AutoPtr<IParcelable> >::Alloc(endpoints.GetLength());

    for (Int32 i = 0, size = endpoints.GetLength(); i < size; i++) {
        AutoPtr<IParcelable> ptr = endpoints[i];
        mEndpoints->Set(i, ptr);
    }

    return NOERROR;
}

ECode CUsbInterface::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;

    return NOERROR;
}

ECode CUsbInterface::GetInterfaceClass(
    /* [out] */ Int32* cls)
{
    VALIDATE_NOT_NULL(cls);
    *cls = mClass;

    return NOERROR;
}

ECode CUsbInterface::GetInterfaceSubclass(
    /* [out] */ Int32* cls)
{
    VALIDATE_NOT_NULL(cls);
    *cls = mSubclass;

    return NOERROR;
}

ECode CUsbInterface::GetInterfaceProtocol(
    /* [out] */ Int32* protocol)
{
    VALIDATE_NOT_NULL(protocol);
    *protocol = mProtocol;

    return NOERROR;
}

ECode CUsbInterface::GetEndpointCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mEndpoints->GetLength();

    return NOERROR;
}

ECode CUsbInterface::GetEndpoint(
    /* [in] */ Int32 index,
    /* [out] */ IUsbEndpoint** endpoint)
{
    VALIDATE_NOT_NULL(endpoint);

    if (index < 0 || index >= mEndpoints->GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    AutoPtr<IUsbEndpoint> tmpEndpoint = IUsbEndpoint::Probe((*mEndpoints)[index]);
    *endpoint = tmpEndpoint;
    REFCOUNT_ADD(*endpoint);

    return NOERROR;
}

ECode CUsbInterface::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuffer buf;
    buf += "UsbInterface[mId=";
    buf += mId;

    buf += ",mClass=";
    buf += mClass;

    buf += ",mSubclass=";
    buf += mSubclass;

    buf += ",mProtocol=";
    buf += mProtocol;
    buf += "]";

    buf.ToString(str);
    return NOERROR;
}

ECode CUsbInterface::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);
    *contents = 0;

    return NOERROR;
}

ECode CUsbInterface::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&mId);
    source->ReadInt32(&mClass);
    source->ReadInt32(&mSubclass);
    source->ReadInt32(&mProtocol);
    source->ReadArrayOf((Handle32*)&mEndpoints);
    return NOERROR;
}

ECode CUsbInterface::WriteToParcel(
    /* [out] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);

    dest->WriteInt32(mId);
    dest->WriteInt32(mClass);
    dest->WriteInt32(mSubclass);
    dest->WriteInt32(mProtocol);
    dest->WriteArrayOf((Handle32)mEndpoints.Get());
    return NOERROR;
}

} // namespace Usb
} // namespace Hardware
} // namespace Droid
} // namespace Elastos
