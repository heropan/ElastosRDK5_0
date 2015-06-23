
#ifndef __CUSBDEVICECONNECTION_H__
#define __CUSBDEVICECONNECTION_H__

#include "_CUsbDeviceConnection.h"
#include "ext/frameworkext.h"
#include <elastos/utility/logging/Logger.h>

using namespace Elastos::Core;
using namespace Elastos::IO;
using namespace Elastos::Droid::Os;
using namespace Elastos::Utility::Logging;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Usb {

/**
 * This class is used for sending and receiving data and control messages to a USB device.
 * Instances of this class are created by {@link UsbManager#openDevice}.
 */
CarClass(CUsbDeviceConnection)
{
public:
    /**
     * UsbDevice should only be instantiated by UsbService implementation
     * @hide
     */
    CARAPI constructor(
        /* [in] */ IUsbDevice* device);

    CARAPI Open(
            /* [in] */ const String& name,
            /* [in] */ IParcelFileDescriptor* pfd,
            /* [out] */ Boolean* result);

    /**
     * Releases all system resources related to the device.
     * Once the object is closed it cannot be used again.
     * The client must call {@link UsbManager#openDevice} again
     * to retrieve a new instance to reestablish communication with the device.
     */
    CARAPI Close();

    /**
     * Returns the native file descriptor for the device, or
     * -1 if the device is not opened.
     * This is intended for passing to native code to access the device.
     *
     * @return the native file descriptor
     */
    CARAPI GetFileDescriptor(
        /* [out] */ Int32* descriptor);

    /**
     * Returns the raw USB descriptors for the device.
     * This can be used to access descriptors not supported directly
     * via the higher level APIs.
     *
     * @return raw USB descriptors
     */
    CARAPI GetRawDescriptors(
        /* [out, callee] */ ArrayOf<Byte>** descriptors);

    /**
     * Claims exclusive access to a {@link android.hardware.usb.UsbInterface}.
     * This must be done before sending or receiving data on any
     * {@link android.hardware.usb.UsbEndpoint}s belonging to the interface.
     *
     * @param intf the interface to claim
     * @param force true to disconnect kernel driver if necessary
     * @return true if the interface was successfully claimed
     */
    CARAPI ClaimInterface(
        /* [in] */ IUsbInterface* intf,
        /* [in] */ Boolean force,
        /* [out] */ Boolean* result);

    /**
     * Releases exclusive access to a {@link android.hardware.usb.UsbInterface}.
     *
     * @return true if the interface was successfully released
     */
    CARAPI ReleaseInterface(
        /* [in] */ IUsbInterface* intf,
        /* [out] */ Boolean* result);

    /**
     * Performs a control transaction on endpoint zero for this device.
     * The direction of the transfer is determined by the request type.
     * If requestType & {@link UsbConstants#USB_ENDPOINT_DIR_MASK} is
     * {@link UsbConstants#USB_DIR_OUT}, then the transfer is a write,
     * and if it is {@link UsbConstants#USB_DIR_IN}, then the transfer
     * is a read.
     *
     * @param requestType request type for this transaction
     * @param request request ID for this transaction
     * @param value value field for this transaction
     * @param index index field for this transaction
     * @param buffer buffer for data portion of transaction,
     * or null if no data needs to be sent or received
     * @param length the length of the data to send or receive
     * @param timeout in milliseconds
     * @return length of data transferred (or zero) for success,
     * or negative value for failure
     */
    CARAPI ControlTransfer(
        /* [in] */ Int32 requestType,
        /* [in] */ Int32 request,
        /* [in] */ Int32 value,
        /* [in] */ Int32 index,
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 length,
        /* [in] */ Int32 timeout,
        /* [out] */ Int32* transferredLength);

    /**
     * Performs a bulk transaction on the given endpoint.
     * The direction of the transfer is determined by the direction of the endpoint
     *
     * @param endpoint the endpoint for this transaction
     * @param buffer buffer for data to send or receive,
     * @param length the length of the data to send or receive
     * @param timeout in milliseconds
     * @return length of data transferred (or zero) for success,
     * or negative value for failure
     */
    CARAPI BulkTransfer(
        /* [in] */ IUsbEndpoint* endpoint,
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 length,
        /* [in] */ Int32 timeout,
        /* [out] */ Int32* transferredLength);

    /**
     * Waits for the result of a {@link android.hardware.usb.UsbRequest#queue} operation
     * Note that this may return requests queued on multiple
     * {@link android.hardware.usb.UsbEndpoint}s.
     * When multiple endpoints are in use, {@link android.hardware.usb.UsbRequest#getEndpoint} and
     * {@link android.hardware.usb.UsbRequest#getClientData} can be useful in determining
     * how to process the result of this function.
     *
     * @return a completed USB request, or null if an error occurred
     */
    CARAPI RequestWait(
        /* [out] */ IUsbRequest** request);

    /**
     * Returns the serial number for the device.
     * This will return null if the device has not been opened.
     *
     * @return the device serial number
     */
    CARAPI GetSerial(
        /* [out] */ String* serialNumber);

private:
    CARAPI_(Boolean) NativeOpen(
        /* [in] */ const String& deviceName,
        /* [in] */ IFileDescriptor* pfd);

    CARAPI_(void) NativeClose();

    CARAPI_(Int32) NativeGetFd();

    CARAPI_(AutoPtr<ArrayOf<Byte> >) NativeGetDesc();

    CARAPI_(Boolean) NativeClaimInterface(
        /* [in] */ Int32 interfaceID,
        /* [in] */ Boolean force);

    CARAPI_(Boolean) NativeReleaseInterface(
        /* [in] */ Int32 interfaceID);

    CARAPI_(Int32) NativeControlRequest(
        /* [in] */ Int32 requestType,
        /* [in] */ Int32 request,
        /* [in] */ Int32 value,
        /* [in] */ Int32 index,
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 length,
        /* [in] */ Int32 timeout);

    CARAPI_(Int32) NativeBulkRequest(
        /* [in] */ Int32 endpoint,
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 length,
        /* [in] */ Int32 timeout);

    CARAPI_(AutoPtr<IUsbRequest>) NativeRequestWait();

    CARAPI_(String) NativeGetSerial();

private:
    AutoPtr<IUsbDevice> mDevice;

public:
    static const String TAG;
    Int32 mNativeContext; // used by the JNI code
};

} // namespace Usb
} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif // __CUSBDEVICECONNECTION_H__
