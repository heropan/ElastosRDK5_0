
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
        if (direction != IUsbConstants::USB_DIR_IN) {
            continue;
        }

        ByteBuffer buffer = bufferForEndpoints.get(endpoint);
        Int32 length = GetInputDataLength(buffer);
        if (length > 0) {
            buffer->Rewind();
            AutoPtr< ArrayOf<Byte> > bs = ArrayOf<Byte>::Alloc(length);
            buffer->Get(bs, 0, length);
            Int32 number;
            endpoint->GetEndpointNumber(&number);
            PostOnDataEvent(number, bs);
        }

        buffer->Rewind();
        Int32 capacity;
        buffer->Capacity(&capacity);
        request->Queue(buffer, capacity);
    }

    return NOERROR;
}

//===============================================================
//            UsbMidiDeviceAndroid::InnerRunnable
//===============================================================

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
    if (mIsClosed) {
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
    mConnection = manager.openDevice(device);
    mEndpointMap = new SparseArray<UsbEndpoint>();
    mRequestMap = new HashMap<UsbEndpoint, UsbRequest>();
    CHandler::New((IHandler**)&mHandler);

    for (Int32 i = 0; i < device.getInterfaceCount(); ++i) {
        UsbInterface iface = device.getInterface(i);
        if (iface.getInterfaceClass() != UsbConstants.USB_CLASS_AUDIO ||
            iface.getInterfaceSubclass() != MIDI_SUBCLASS) {
            continue;
        }
        mConnection.claimInterface(iface, true);
        for (int j = 0; j < iface.getEndpointCount(); ++j) {
            UsbEndpoint endpoint = iface.getEndpoint(j);
            if (endpoint.getDirection() == UsbConstants.USB_DIR_OUT) {
                mEndpointMap.put(endpoint.getEndpointNumber(), endpoint);
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
    final Map<UsbEndpoint, ByteBuffer> bufferForEndpoints =
        new HashMap<UsbEndpoint, ByteBuffer>();

    for (Int32 i = 0; i < device.getInterfaceCount(); ++i) {
        UsbInterface iface = device.getInterface(i);
        if (iface.getInterfaceClass() != UsbConstants.USB_CLASS_AUDIO ||
            iface.getInterfaceSubclass() != MIDI_SUBCLASS) {
            continue;
        }
        for (int j = 0; j < iface.getEndpointCount(); ++j) {
            UsbEndpoint endpoint = iface.getEndpoint(j);
            if (endpoint.getDirection() == UsbConstants.USB_DIR_IN) {
                ByteBuffer buffer = ByteBuffer.allocate(endpoint.getMaxPacketSize());
                UsbRequest request = new UsbRequest();
                request.initialize(mConnection, endpoint);
                request.queue(buffer, buffer.remaining());
                bufferForEndpoints.put(endpoint, buffer);
            }
        }
    }

    if (bufferForEndpoints.isEmpty()) {
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
    mHandler->Post(runnable);
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

    UsbEndpoint endpoint = mEndpointMap.get(endpointNumber);
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
        const Int32 TIMEOUT = 100;
        mConnection.bulkTransfer(endpoint, bs, 0, bs.length, TIMEOUT);
    }
    else {
        UsbRequest request = mRequestMap.get(endpoint);
        if (request == null) {
            request = new UsbRequest();
            request.initialize(mConnection, endpoint);
            mRequestMap.put(endpoint, request);
        }
        request.queue(ByteBuffer.wrap(bs), bs.length);
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
AutoPtr< ArrayOf<Byte> > UsbMidiDeviceAndroid::GetDescriptors()
{
    AutoPtr< ArrayOf<Byte> > array
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
    mEndpointMap.clear();
    for (UsbRequest request : mRequestMap.values()) {
        request.close();
    }
    mRequestMap.clear();
    mConnection.close();
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
    buffer->Position(&position);
    // We assume that the data length is always divisable by 4.
    for (Int32 i = 0; i < position; i += 4) {
        // Since Code Index Number 0 is reserved, it is not a valid USB-MIDI data.
        Int32 pos;
        buffer->Get(i, &pos);
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
}

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos