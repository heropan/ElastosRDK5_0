
#include "hardware/usb/CUsbDeviceConnection.h"
#include <usbhost/usbhost.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Usb {

const String CUsbDeviceConnection::TAG = String("UsbDeviceConnection");

ECode CUsbDeviceConnection::constructor(
    /* [in] */ IUsbDevice* device)
{
    mDevice = device;
    device->Release();
    return NOERROR;
}

ECode CUsbDeviceConnection::Open(
    /* [in] */ const String& name,
    /* [in] */ IParcelFileDescriptor* pfd,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IFileDescriptor> fd;
    FAIL_RETURN(pfd->GetFileDescriptor((IFileDescriptor**)&fd));
    *result = NativeOpen(name, fd);

    return NOERROR;
}

ECode CUsbDeviceConnection::Close()
{
    NativeClose();
    return NOERROR;
}

ECode CUsbDeviceConnection::GetFileDescriptor(
    /* [out] */ Int32* descriptor)
{
    VALIDATE_NOT_NULL(descriptor);
    *descriptor = NativeGetFd();
    return NOERROR;
}

ECode CUsbDeviceConnection::GetRawDescriptors(
    /* [out, callee] */ ArrayOf<Byte>** descriptors)
{
    VALIDATE_NOT_NULL(descriptors);
    AutoPtr<ArrayOf<Byte> > tmp = NativeGetDesc();
    *descriptors = tmp;
    INTERFACE_ADDREF(*descriptors);
    return NOERROR;
}

ECode CUsbDeviceConnection::ClaimInterface(
    /* [in] */ IUsbInterface* intf,
    /* [in] */ Boolean force,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 id;
    FAIL_RETURN(intf->GetId(&id));
    *result = NativeClaimInterface(id, force);

    return NOERROR;
}

ECode CUsbDeviceConnection::ReleaseInterface(
    /* [in] */ IUsbInterface* intf,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 id;
    FAIL_RETURN(intf->GetId(&id));
    *result = NativeReleaseInterface(id);

    return NOERROR;
}

ECode CUsbDeviceConnection::ControlTransfer(
    /* [in] */ Int32 requestType,
    /* [in] */ Int32 request,
    /* [in] */ Int32 value,
    /* [in] */ Int32 index,
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 length,
    /* [in] */ Int32 timeout,
    /* [out] */ Int32* transferredLength)
{
    VALIDATE_NOT_NULL(transferredLength);
    *transferredLength = NativeControlRequest(requestType, request, value, index, buffer, length, timeout);
    return NOERROR;
}

ECode CUsbDeviceConnection::BulkTransfer(
    /* [in] */ IUsbEndpoint* endpoint,
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 length,
    /* [in] */ Int32 timeout,
    /* [out] */ Int32* transferredLength)
{
    VALIDATE_NOT_NULL(transferredLength);

    Int32 address;
    FAIL_RETURN(endpoint->GetAddress(&address));
    *transferredLength = NativeBulkRequest(address, buffer, length, timeout);

    return NOERROR;
}

ECode CUsbDeviceConnection::RequestWait(
    /* [out] */ IUsbRequest** request)
{
    VALIDATE_NOT_NULL(request);

    AutoPtr<IUsbRequest> r = NativeRequestWait();
    *request = r;
    INTERFACE_ADDREF(*request);

    if (*request != NULL) {
        (*request)->Dequeue();
    }

    return NOERROR;
}

ECode CUsbDeviceConnection::GetSerial(
    /* [out] */ String* serialNumber)
{
    VALIDATE_NOT_NULL(serialNumber);

    *serialNumber = NativeGetSerial();
    return NOERROR;
}

struct usb_device* CUsbDeviceConnection_get_device_from_object(IUsbDeviceConnection* connection)
{
    return (struct usb_device*)(((CUsbDeviceConnection*)connection)->mNativeContext);
}

Boolean CUsbDeviceConnection::NativeOpen(
    /* [in] */ const String& deviceName,
    /* [in] */ IFileDescriptor* pfd)
{
    Int32 fd;
    FAIL_RETURN(pfd->GetDescriptor(&fd));

    // duplicate the file descriptor, since ParcelFileDescriptor will eventually close its copy
    fd = dup(fd);
    if (fd < 0){
        return FALSE;
    }

    const char *deviceNameStr = deviceName.string();
    struct usb_device* device = usb_device_new(deviceNameStr, fd);

    if (device) {
        this->mNativeContext = (Int32) device ;
    } else {
        //ALOGE("usb_device_open failed for %s", deviceNameStr);
        Logger::D(CUsbDeviceConnection::TAG, "usb_device_open failed for " + *deviceNameStr);
        close(fd);
    }

    return (device != NULL);
}

void CUsbDeviceConnection::NativeClose()
{
    //ALOGD("close\n");
    Logger::D(CUsbDeviceConnection::TAG, "close\n");

    struct usb_device* device = CUsbDeviceConnection_get_device_from_object((IUsbDeviceConnection*)this);

    if (!device) {
        return;
    }

    usb_device_close(device);
    this->mNativeContext = 0;
}

Int32 CUsbDeviceConnection::NativeGetFd()
{
    struct usb_device* device = CUsbDeviceConnection_get_device_from_object((IUsbDeviceConnection*)this);

    if (!device) {
        //ALOGE("device is closed in native_get_fd");
        Logger::D(CUsbDeviceConnection::TAG, "device is closed in native_get_fd");
        return -1;
    }

    return usb_device_get_fd(device);
}

AutoPtr<ArrayOf<Byte> > CUsbDeviceConnection::NativeGetDesc()
{
    char buffer[16384];
    Int32 fd = NativeGetFd();

    if (fd < 0){
        return NULL;
    }

    lseek(fd, 0, SEEK_SET);
    Int32 length = read(fd, buffer, sizeof(buffer));

    if (length < 0){
        return NULL;
    }

    AutoPtr< ArrayOf<Byte> > ret = ArrayOf<Byte>::Alloc(length);
    ret->Copy((Byte*)buffer, length);

    return ret;
}

Boolean CUsbDeviceConnection::NativeClaimInterface(
    /* [in] */ Int32 interfaceID,
    /* [in] */ Boolean force)
{
    struct usb_device* device = CUsbDeviceConnection_get_device_from_object((IUsbDeviceConnection*)this);

    if (!device) {
        //ALOGE("device is closed in native_claim_interface");
        Logger::D(CUsbDeviceConnection::TAG, "device is closed in native_claim_interface");
        return -1;
    }

    Int32 ret = usb_device_claim_interface(device, interfaceID);
    if (ret && force && errno == EBUSY) {
        // disconnect kernel driver and try again
        usb_device_connect_kernel_driver(device, interfaceID, FALSE);
        ret = usb_device_claim_interface(device, interfaceID);
    }

    return ret == 0;
}

Boolean CUsbDeviceConnection::NativeReleaseInterface(
    /* [in] */ Int32 interfaceID)
{
    struct usb_device* device = CUsbDeviceConnection_get_device_from_object((IUsbDeviceConnection*)this);

    if (!device) {
        //ALOGE("device is closed in native_release_interface");
        Logger::D(CUsbDeviceConnection::TAG, "device is closed in native_release_interface");
        return -1;
    }

    Int32 ret = usb_device_release_interface(device, interfaceID);
    if (ret == 0) {
        // allow kernel to reconnect its driver
        usb_device_connect_kernel_driver(device, interfaceID, TRUE);
    }

    return ret;
}

Int32 CUsbDeviceConnection::NativeControlRequest(
    /* [in] */ Int32 requestType,
    /* [in] */ Int32 request,
    /* [in] */ Int32 value,
    /* [in] */ Int32 index,
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 length,
    /* [in] */ Int32 timeout)
{
    struct usb_device* device = CUsbDeviceConnection_get_device_from_object((IUsbDeviceConnection*)this);

    if (!device) {
        //ALOGE("device is closed in native_control_request");
        Logger::D(CUsbDeviceConnection::TAG, "device is closed in native_control_request");
        return -1;
    }

    Byte* bufferBytes = NULL;
    if (&buffer && buffer.GetPayload() != NULL) {
        if (buffer.GetLength() < length) {
            //jniThrowException(env, "java/lang/ArrayIndexOutOfBoundsException", NULL);
            return -1;
        }

        bufferBytes = buffer.GetPayload();
    }

    return usb_device_control_transfer(device, requestType, request, value, index, bufferBytes, length, timeout);
}

Int32 CUsbDeviceConnection::NativeBulkRequest(
    /* [in] */ Int32 endpoint,
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 length,
    /* [in] */ Int32 timeout)
{
    struct usb_device* device = CUsbDeviceConnection_get_device_from_object((IUsbDeviceConnection*)this);

    if (!device) {
        //ALOGE("device is closed in native_control_request");
        Logger::D(CUsbDeviceConnection::TAG, "device is closed in native_control_request");
        return -1;
    }

    Byte* bufferBytes = NULL;
    if (&buffer && buffer.GetPayload() != NULL) {
        if (buffer.GetLength() < length) {
            //jniThrowException(env, "java/lang/ArrayIndexOutOfBoundsException", NULL);
            return -1;
        }

        bufferBytes = buffer.GetPayload();
    }

    return usb_device_bulk_transfer(device, endpoint, bufferBytes, length, timeout);
}

AutoPtr<IUsbRequest> CUsbDeviceConnection::NativeRequestWait()
{
    struct usb_device* device = CUsbDeviceConnection_get_device_from_object((IUsbDeviceConnection*)this);

    if (!device) {
        //ALOGE("device is closed in native_request_wait");
        Logger::D(CUsbDeviceConnection::TAG, "device is closed in native_request_wait");
        return NULL;
    }

    struct usb_request* request = usb_request_wait(device);

    AutoPtr<IUsbRequest> result;
    if (request){
        result = (IUsbRequest*)request->client_data;
    }
    return result;
}

String CUsbDeviceConnection::NativeGetSerial()
{
    struct usb_device* device = CUsbDeviceConnection_get_device_from_object((IUsbDeviceConnection*)this);

    if (!device) {
        //ALOGE("device is closed in native_request_wait");
        Logger::D(CUsbDeviceConnection::TAG, "device is closed in native_request_wait");
        return String("");
    }

    char* serial = usb_device_get_serial(device);
    if (!serial){
        return String("");
    }

    String result = String(serial);
    free(serial);
    return result;
}

} // namespace Usb
} // namespace Hardware
} // namespace Droid
} // namespace Elastos
