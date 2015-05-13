
#ifndef __CUSBDEVICE_H__
#define __CUSBDEVICE_H__

#include "_CUsbDevice.h"
#include "ext/frameworkext.h"

using namespace Elastos::Core;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Usb {

/**
 * This class represents a USB device attached to the android device with the android device
 * acting as the USB host.
 * Each device contains one or more {@link UsbInterface}s, each of which contains a number of
 * {@link UsbEndpoint}s (the channels via which data is transmitted over USB).
 *
 * <p> This class contains information (along with {@link UsbInterface} and {@link UsbEndpoint})
 * that describes the capabilities of the USB device.
 * To communicate with the device, you open a {@link UsbDeviceConnection} for the device
 * and use {@link UsbRequest} to send and receive data on an endpoint.
 * {@link UsbDeviceConnection#controlTransfer} is used for control requests on endpoint zero.
 *
 * <div class="special reference">
 * <h3>Developer Guides</h3>
 * <p>For more information about communicating with USB hardware, read the
 * <a href="{@docRoot}guide/topics/usb/index.html">USB</a> developer guide.</p>
 * </div>
 */
CarClass(CUsbDevice)
{
public:
    CARAPI constructor();

    /**
     * UsbDevice should only be instantiated by UsbService implementation
     * @hide
     */
    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ Int32 vendorId,
        /* [in] */ Int32 productId,
        /* [in] */ Int32 cls,
        /* [in] */ Int32 subCls,
        /* [in] */ Int32 protocol,
        /* [in] */ ArrayOf<IParcelable*>* interfaces);

    /**
     * Returns the name of the device.
     * In the standard implementation, this is the path of the device file
     * for the device in the usbfs file system.
     *
     * @return the device name
     */
    CARAPI GetDeviceName(
        /* [out] */ String* name);

    /**
     * Returns a unique integer ID for the device.
     * This is a convenience for clients that want to use an integer to represent
     * the device, rather than the device name.
     * IDs are not persistent across USB disconnects.
     *
     * @return the device ID
     */
    CARAPI GetDeviceId(
        /* [out] */ Int32* id);

    /**
     * Returns a vendor ID for the device.
     *
     * @return the device vendor ID
     */
    CARAPI GetVendorId(
        /* [out] */ Int32* id);

    /**
     * Returns a product ID for the device.
     *
     * @return the device product ID
     */
    CARAPI GetProductId(
        /* [out] */ Int32* id);

    /**
     * Returns the devices's class field.
     * Some useful constants for USB device classes can be found in {@link UsbConstants}.
     *
     * @return the devices's class
     */
    CARAPI GetDeviceClass(
        /* [out] */ Int32* cls);

    /**
     * Returns the device's subclass field.
     *
     * @return the device's subclass
     */
    CARAPI GetDeviceSubclass(
        /* [out] */ Int32* cls);

    /**
     * Returns the device's protocol field.
     *
     * @return the device's protocol
     */
    CARAPI GetDeviceProtocol(
        /* [out] */ Int32* protocol);

    /**
     * Returns the number of {@link UsbInterface}s this device contains.
     *
     * @return the number of interfaces
     */
    CARAPI GetInterfaceCount(
        /* [out] */ Int32* count);

    /**
     * Returns the {@link UsbInterface} at the given index.
     *
     * @return the interface
     */
    CARAPI GetInterface(
        /* [in] */ Int32 index,
        /* [out] */ IUsbInterface** usbInterface);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* value);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI DescribeContents (
        /* [out] */ Int32* contents);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [out] */ IParcel* dest);

private:
    String mName;
    Int32 mVendorId;
    Int32 mProductId;
    Int32 mClass;
    Int32 mSubclass;
    Int32 mProtocol;
    AutoPtr< ArrayOf<IParcelable*> > mInterfaces;
};

} // namespace Usb
} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif // __CUSBDEVICE_H__
