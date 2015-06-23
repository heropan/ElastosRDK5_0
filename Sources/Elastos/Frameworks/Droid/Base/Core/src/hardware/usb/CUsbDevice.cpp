
#include "hardware/usb/CUsbDevice.h"
#include "hardware/usb/UsbDevice.h"
#include <elastos/core/StringBuffer.h>

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Usb {

ECode CUsbDevice::constructor()
{
    return NOERROR;
}

ECode CUsbDevice::constructor(
    /* [in] */ const String& name,
    /* [in] */ Int32 vendorId,
    /* [in] */ Int32 productId,
    /* [in] */ Int32 cls,
    /* [in] */ Int32 subCls,
    /* [in] */ Int32 protocol,
    /* [in] */ ArrayOf<IParcelable*>* interfaces)
{
    mName = name;
    mVendorId = vendorId;
    mProductId = productId;
    mClass = cls;
    mSubclass = subCls;
    mProtocol = protocol;
    mInterfaces = interfaces;
    return NOERROR;
}

ECode CUsbDevice::GetDeviceName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;

    return NOERROR;
}

ECode CUsbDevice::GetDeviceId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = UsbDevice::NativeGetDeviceId(mName);

    return NOERROR;
}

ECode CUsbDevice::GetVendorId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mVendorId;

    return NOERROR;
}

ECode CUsbDevice::GetProductId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mProductId;

    return NOERROR;
}

ECode CUsbDevice::GetDeviceClass(
    /* [out] */ Int32* cls)
{
    VALIDATE_NOT_NULL(cls);
    *cls = mClass;

    return NOERROR;
}

ECode CUsbDevice::GetDeviceSubclass(
    /* [out] */ Int32* cls)
{
    VALIDATE_NOT_NULL(cls);
    *cls = mSubclass;

    return NOERROR;
}

ECode CUsbDevice::GetDeviceProtocol(
    /* [out] */ Int32* protocol)
{
    VALIDATE_NOT_NULL(protocol);
    *protocol = mProtocol;

    return NOERROR;
}

ECode CUsbDevice::GetInterfaceCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mInterfaces->GetLength();

    return NOERROR;
}

ECode CUsbDevice::GetInterface(
    /* [in] */ Int32 index,
    /* [out] */ IUsbInterface** usbInterface)
{
    VALIDATE_NOT_NULL(usbInterface);

    AutoPtr<IUsbInterface> tmpInterface = IUsbInterface::Probe((*mInterfaces)[index]);
    *usbInterface = tmpInterface;
    REFCOUNT_ADD(*usbInterface);

    return NOERROR;
}

ECode CUsbDevice::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (obj == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    if (IUsbDevice::Probe(obj) != NULL) {
        CUsbDevice* device = (CUsbDevice*)IUsbDevice::Probe(obj);
        *result = device->mName.Equals(mName);
        return NOERROR;
    }

    *result = FALSE;
    return NOERROR;
}

ECode CUsbDevice::GetHashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mName.GetHashCode();

    return NOERROR;
}

ECode CUsbDevice::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuffer buf;
    buf += "UsbDevice[mName=";
    buf += mName;

    buf += ",mVendorId=";
    buf += mVendorId;

    buf += ",mProductId=";
    buf += mProductId;

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

ECode CUsbDevice::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);
    *contents = 0;

    return NOERROR;
}

ECode CUsbDevice::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadString(&mName);
    source->ReadInt32(&mVendorId);
    source->ReadInt32(&mProductId);
    source->ReadInt32(&mClass);
    source->ReadInt32(&mSubclass);
    source->ReadInt32(&mProtocol);
    source->ReadArrayOf((Handle32*)&mInterfaces);

    return NOERROR;
}

ECode CUsbDevice::WriteToParcel(
    /* [out] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);

    dest->WriteString(mName);
    dest->WriteInt32(mVendorId);
    dest->WriteInt32(mProductId);
    dest->WriteInt32(mClass);
    dest->WriteInt32(mSubclass);
    dest->WriteInt32(mProtocol);
    dest->WriteArrayOf((Handle32)mInterfaces.Get());

    return NOERROR;
}

} // namespace Usb
} // namespace Hardware
} // namespace Droid
} // namespace Elastos
