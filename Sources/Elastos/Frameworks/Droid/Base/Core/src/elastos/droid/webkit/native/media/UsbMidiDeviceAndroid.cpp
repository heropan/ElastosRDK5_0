#include "elastos/droid/webkit/native/media/UsbMidiDeviceAndroid.h"
#include "elastos/droid/webkit/native/media/api/UsbMidiDeviceAndroid_dec.h"

//TODO #include "elastos/io/CByteBufferHelper.h"
#include "elastos/droid/os/Build.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/os/CHandler.h"
#include "elastos/droid/hardware/usb/CUsbRequest.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Hardware::Usb::IUsbConstants;
using Elastos::Droid::Hardware::Usb::IUsbDevice;
using Elastos::Droid::Hardware::Usb::IUsbDeviceConnection;
using Elastos::Droid::Hardware::Usb::IUsbEndpoint;
using Elastos::Droid::Hardware::Usb::IUsbInterface;
using Elastos::Droid::Hardware::Usb::IUsbManager;
using Elastos::Droid::Hardware::Usb::IUsbRequest;
using Elastos::Droid::Hardware::Usb::CUsbRequest;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Utility::ISparseArray;
//TODO using Elastos::Droid::Utility::CSparseArray;

using Elastos::IO::IBuffer;
using Elastos::IO::IByteBuffer;
using Elastos::IO::IByteBufferHelper;
using Elastos::IO::CByteBufferHelper;
using Elastos::Core::IThread;
using Elastos::Core::EIID_IThread;
using Elastos::Core::EIID_IRunnable;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

//===============================================================
//             UsbMidiDeviceAndroid::InnerThread
//===============================================================

UsbMidiDeviceAndroid::InnerThread::InnerThread(
    /* [in] */ UsbMidiDeviceAndroid* owner)
    : mOwner(owner)
{
}

ECode UsbMidiDeviceAndroid::InnerThread::Run()
{
    while (TRUE) {
        AutoPtr<IUsbRequest> request;
        mOwner->mConnection->RequestWait((IUsbRequest**)&request);
        if (request == NULL) {
            // When the device is closed requestWait will fail.
            break;
        }

        AutoPtr<IUsbEndpoint> endpoint;
        request->GetEndpoint((IUsbEndpoint**)&endpoint);
        Int32 direction;
        endpoint->GetDirection(&direction);
        if (direction != IUsbConstants::_USB_DIR_IN) {
            continue;
        }

        //TODO
        AutoPtr<IByteBuffer> byteBuffer = mOwner->mBufferForEndpoints[endpoint];
        AutoPtr<IBuffer> iBuffer = IBuffer::Probe(byteBuffer);
        Int32 length = GetInputDataLength(byteBuffer);
        if (length > 0) {
            iBuffer->Rewind();
            AutoPtr<ArrayOf<Byte> > bs = ArrayOf<Byte>::Alloc(length);
            byteBuffer->Get(bs, 0, length);
            Int32 number;
            endpoint->GetEndpointNumber(&number);
            mOwner->PostOnDataEvent(number, bs);
        }

        iBuffer->Rewind();
        Int32 capacity;
        iBuffer->GetCapacity(&capacity);
        Boolean result;
        request->Queue(byteBuffer, capacity, &result);
    }
    return NOERROR;
}

//===============================================================
//            UsbMidiDeviceAndroid::InnerRunnable
//===============================================================
CAR_INTERFACE_IMPL(UsbMidiDeviceAndroid::InnerRunnable, Object, IRunnable);

UsbMidiDeviceAndroid::InnerRunnable::InnerRunnable(
    /* [in] */ UsbMidiDeviceAndroid* owner,
    /* [in] */ Int32 endpointNumber,
    /* [in] */ ArrayOf<Byte>* bs)
    : mOwner(owner)
    , mEndpointNumber(endpointNumber)
    , mBs(bs)
{
}

ECode UsbMidiDeviceAndroid::InnerRunnable::Run()
{
    if (mOwner->mIsClosed) {
        return NOERROR;
    }

    NativeOnData(mOwner->mNativePointer, mEndpointNumber, mBs);

    return NOERROR;
}

//===============================================================
//                    UsbMidiDeviceAndroid
//===============================================================

/**
 * Audio interface subclass code for MIDI.
 */
const Int32 UsbMidiDeviceAndroid::MIDI_SUBCLASS;

/**
 * Constructs a UsbMidiDeviceAndroid.
 * @param manager
 * @param device The USB device which this object is assocated with.
 */
UsbMidiDeviceAndroid::UsbMidiDeviceAndroid(
    /* [in] */ IUsbManager* manager,
    /* [in] */ IUsbDevice* device)
    : mIsClosed(FALSE)
    , mHasInputThread(FALSE)
    , mNativePointer(0)
{
    //mConnection = manager->OpenDevice(device);
    manager->OpenDevice(device, (IUsbDeviceConnection**)&mConnection);
    //mEndpointMap = new SparseArray<UsbEndpoint>();
    //TODO CSparseArray::New((ISparseArray**)&mEndpointMap);
    //mRequestMap = new HashMap<UsbEndpoint, UsbRequest>();
    CHandler::New((IHandler**)&mHandler);

    Int32 interfaceCount;
    device->GetInterfaceCount(&interfaceCount);
    for (Int32 i = 0; i < interfaceCount; ++i) {
        AutoPtr<IUsbInterface> iface;
        device->GetInterface(i, (IUsbInterface**)&iface);
        Int32 interfaceClass, interfaceSubclass;
        iface->GetInterfaceClass(&interfaceClass);
        iface->GetInterfaceSubclass(&interfaceSubclass);
        if (interfaceClass != IUsbConstants::_USB_CLASS_AUDIO ||
            interfaceSubclass != MIDI_SUBCLASS) {
            continue;
        }
        Boolean result;
        mConnection->ClaimInterface(iface, true, &result);
        Int32 endpointCount;
        iface->GetEndpointCount(&endpointCount);
        for (Int32 j = 0; j < endpointCount; ++j) {
            //UsbEndpoint endpoint = iface.getEndpoint(j);
            AutoPtr<IUsbEndpoint> endpoint;
            iface->GetEndpoint(j, (IUsbEndpoint**)&endpoint);
            Int32 direction;
            endpoint->GetDirection(&direction);
            if (direction == IUsbConstants::_USB_DIR_OUT) {
                Int32 endpointNumber;
                endpoint->GetEndpointNumber(&endpointNumber);
                mEndpointMap->Put(endpointNumber, endpoint);
            }
        }
    }
    // Start listening for input endpoints.
    // This function will create and run a thread if there is USB-MIDI endpoints in the
    // device. Note that because UsbMidiDevice is shared among all tabs and the thread
    // will be terminated when the device is disconnected, at most one thread can be created
    // for each connected USB-MIDI device.
    StartListen(device);
}

/**
 * Starts listening for input endpoints.
 */
void UsbMidiDeviceAndroid::StartListen(
    /* [in] */ IUsbDevice* device)
{
    //final Map<UsbEndpoint, ByteBuffer> bufferForEndpoints =
    //    new HashMap<UsbEndpoint, ByteBuffer>();

    Int32 interfaceCount;
    device->GetInterfaceCount(&interfaceCount);
    for (Int32 i = 0; i < interfaceCount; ++i) {
        AutoPtr<IUsbInterface> iface;
        device->GetInterface(i, (IUsbInterface**)&iface);
        Int32 interfaceClass, interfaceSubclass;
        iface->GetInterfaceClass(&interfaceClass);
        iface->GetInterfaceSubclass(&interfaceSubclass);
        if (interfaceClass != IUsbConstants::_USB_CLASS_AUDIO ||
            interfaceSubclass != MIDI_SUBCLASS) {
            continue;
        }
        Int32 endpointCount;
        iface->GetEndpointCount(&endpointCount);
        for (int j = 0; j < endpointCount; ++j) {
            //UsbEndpoint endpoint = iface.getEndpoint(j);
            AutoPtr<IUsbEndpoint> endpoint;
            iface->GetEndpoint(j, (IUsbEndpoint**)&endpoint);
            Int32 endpointDirection;
            endpoint->GetDirection(&endpointDirection);
            if (endpointDirection == IUsbConstants::_USB_DIR_IN) {
                //ByteBuffer buffer = ByteBuffer.allocate(endpoint.getMaxPacketSize());
                Int32 capacity;
                endpoint->GetMaxPacketSize(&capacity);
                AutoPtr<IByteBuffer> byteBuffer;
                AutoPtr<IByteBufferHelper> bbHelper;
                CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&bbHelper);
                bbHelper->Allocate(capacity, (IByteBuffer**)&byteBuffer);
                //UsbRequest request = new UsbRequest();
                AutoPtr<IUsbRequest> request;
                CUsbRequest::New((IUsbRequest**)&request);
                Boolean result;
                request->Initialize(mConnection, endpoint, &result);
                AutoPtr<IBuffer> buffer = IBuffer::Probe(byteBuffer);
                Int32 remain;
                buffer->GetRemaining(&remain);
                request->Queue(byteBuffer, remain, &result);
                mBufferForEndpoints[endpoint] =  byteBuffer;
            }
        }
    }

    if (mBufferForEndpoints.IsEmpty()) {
        return;
    }

    mHasInputThread = TRUE;
    // bufferForEndpoints must not be accessed hereafter on this thread.
    AutoPtr<IThread> thread = new InnerThread(this);
    thread->Start();
}

/**
 * Posts a data input event to the main thread.
 */
void UsbMidiDeviceAndroid::PostOnDataEvent(
    /* [in] */ Int32 endpointNumber,
    /* [in] */ ArrayOf<Byte>* bs)
{
    AutoPtr<IRunnable> runnable = new InnerRunnable(this, endpointNumber, bs);
    Boolean result;
    mHandler->Post(runnable, &result);
}

/**
 * Register the own native pointer.
 */
//@CalledByNative
void UsbMidiDeviceAndroid::RegisterSelf(
    /* [in] */ Int64 nativePointer)
{
    mNativePointer = nativePointer;
}

/**
 * Sends a USB-MIDI data to the device.
 * @param endpointNumber The endpoint number of the destination endpoint.
 * @param bs The data to be sent.
 */
//@TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR2)
//@CalledByNative
void UsbMidiDeviceAndroid::Send(
    /* [in] */ Int32 endpointNumber,
    /* [in] */ ArrayOf<Byte>* bs)
{
    if (mIsClosed) {
        return;
    }

    //UsbEndpoint endpoint = mEndpointMap.get(endpointNumber);
    AutoPtr<IUsbEndpoint> endpoint;
    mEndpointMap->Get(endpointNumber, (IInterface**)&endpoint);
    if (endpoint == NULL) {
        return;
    }

    if (ShouldUseBulkTransfer()) {
        // We use bulkTransfer instead of UsbRequest.queue because queueing
        // a UsbRequest is currently not thread safe.
        // Note that this is not exactly correct because we don't care
        // about the transfer attribute (bmAttribute) of the endpoint.
        // See also:
        //  http://stackoverflow.com/questions/9644415/
        //  https://code.google.com/p/android/issues/detail?id=59467
        //
        // TODO(yhirano): Delete this block once the problem is fixed.
        Int32 TIMEOUT = 100;
        Int32 transferNum;
        //TODO pay attention to the 2th parameter
        mConnection->BulkTransfer(endpoint, *bs, bs->GetLength(), TIMEOUT, &transferNum);
    }
    else {
        //UsbRequest request = mRequestMap.get(endpoint);
        AutoPtr<IUsbRequest> request = mRequestMap[endpoint];
        if (request == NULL) {
            //request = new UsbRequest();
            //request.initialize(mConnection, endpoint);
            //mRequestMap.put(endpoint, request);
            AutoPtr<IUsbRequest> request;
            CUsbRequest::New((IUsbRequest**)&request);
            Boolean result;
            request->Initialize(mConnection, endpoint, &result);
            mRequestMap[endpoint] =  request;
        }
        Boolean result;
        AutoPtr<IByteBufferHelper> bbHelper;
        CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&bbHelper);
        AutoPtr<IByteBuffer> ibyteBuffer;
        bbHelper->Wrap(bs, (IByteBuffer**)&ibyteBuffer);
        request->Queue(ibyteBuffer, bs->GetLength(), &result);
    }
}

/**
 * Returns true if |bulkTransfer| should be used in |send|.
 * See comments in |send|.
 */
Boolean UsbMidiDeviceAndroid::ShouldUseBulkTransfer()
{
    return mHasInputThread;
}

/**
 * Returns the descriptors bytes of this device.
 * @return The descriptors bytes of this device.
 */
//@CalledByNative
AutoPtr<ArrayOf<Byte> > UsbMidiDeviceAndroid::GetDescriptors()
{
    AutoPtr<ArrayOf<Byte> > array;
    if (mConnection == NULL) {
        array = ArrayOf<Byte>::Alloc(0);
        return array;
    }

    mConnection->GetRawDescriptors((ArrayOf<Byte>**)&array);

    return array;
}

/**
 * Closes the device connection.
 */
//@CalledByNative
void UsbMidiDeviceAndroid::Close()
{
    mEndpointMap->Clear();
    HashMap<AutoPtr<IUsbEndpoint>, AutoPtr<IUsbRequest> >::Iterator iter =
        mRequestMap.Begin();
    //for (UsbRequest request : mRequestMap.values())
    for(;iter != mRequestMap.End(); ++iter)
    {
        //request.close();
        iter->mSecond->Close();
    }
    mRequestMap.Clear();
    mConnection->Close();
    mNativePointer = 0;
    mIsClosed = true;
}

/**
 * Returns the length of a USB-MIDI input.
 * Since the Android API doesn't provide us the length,
 * we calculate it manually.
 */
Int32 UsbMidiDeviceAndroid::GetInputDataLength(
    /* [in] */ IByteBuffer* buffer)
{
    Int32 position;
    AutoPtr<IBuffer> iBuffer = IBuffer::Probe(buffer);
    iBuffer->GetPosition(&position);
    // We assume that the data length is always divisable by 4.
    for (Int32 i = 0; i < position; i += 4) {
        // Since Code Index Number 0 is reserved, it is not a valid USB-MIDI data.
        Int32 pos;
        buffer->GetInt32(i, &pos);
        if (pos == 0) {
            return i;
        }
    }

    return position;
}

void UsbMidiDeviceAndroid::NativeOnData(
    /* [in] */ Int64 nativeUsbMidiDeviceAndroid,
    /* [in] */ Int32 endpointNumber,
    /* [in] */ ArrayOf<Byte>* data)
{
    Elastos_UsbMidiDeviceAndroid_nativeOnData((Handle32)nativeUsbMidiDeviceAndroid, endpointNumber, data);
}

//callback function definition
void UsbMidiDeviceAndroid::RegisterSelf(
    /* [in] */ IInterface* obj,
    /* [in] */ Int64 nativePointer)
{
    AutoPtr<UsbMidiDeviceAndroid> mObj = (UsbMidiDeviceAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("UsbMidiDeviceAndroid", "UsbMidiDeviceAndroid::RegisterSelf, mObj is NULL");
        return;
    }
    mObj->RegisterSelf(nativePointer);
}

void UsbMidiDeviceAndroid::Send(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 endpointNumber,
    /* [in] */ ArrayOf<Byte>* bs)
{
    AutoPtr<UsbMidiDeviceAndroid> mObj = (UsbMidiDeviceAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("UsbMidiDeviceAndroid", "UsbMidiDeviceAndroid::Send, mObj is NULL");
        return;
    }
    mObj->Send(endpointNumber, bs);
}

AutoPtr<ArrayOf<Byte> > UsbMidiDeviceAndroid::GetDescriptors(
    /* [in] */ IInterface* obj)
{
    AutoPtr<UsbMidiDeviceAndroid> mObj = (UsbMidiDeviceAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("UsbMidiDeviceAndroid", "UsbMidiDeviceAndroid::GetDescriptors, mObj is NULL");
        return NULL;
    }
    return mObj->GetDescriptors();
}

void UsbMidiDeviceAndroid::Close(
    /* [in] */ IInterface* obj)
{
    AutoPtr<UsbMidiDeviceAndroid> mObj = (UsbMidiDeviceAndroid*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("UsbMidiDeviceAndroid", "UsbMidiDeviceAndroid::Close, mObj is NULL");
        return;
    }
    mObj->Close();
}

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
