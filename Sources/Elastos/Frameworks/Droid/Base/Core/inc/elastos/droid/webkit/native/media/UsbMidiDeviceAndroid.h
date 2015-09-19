
#ifndef __ELASTOS_DROID_WEBKIT_MEDIA_USBMIDIDEVICEANDROID_H__
#define __ELASTOS_DROID_WEBKIT_MEDIA_USBMIDIDEVICEANDROID_H__

// import android.annotation.TargetApi;
// import android.hardware.usb.UsbConstants;
// import android.hardware.usb.UsbDevice;
// import android.hardware.usb.UsbDeviceConnection;
// import android.hardware.usb.UsbEndpoint;
// import android.hardware.usb.UsbInterface;
// import android.hardware.usb.UsbManager;
// import android.hardware.usb.UsbRequest;
// import android.os.Build;
// import android.os.Handler;
// import android.util.SparseArray;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

// import java.nio.ByteBuffer;
// import java.util.HashMap;
// import java.util.Map;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

/**
 * Owned by its native counterpart declared in usb_midi_device_android.h.
 * Refer to that class for general comments.
 */
//@JNINamespace("media")
class UsbMidiDeviceAndroid
{
private:
    class InnerThread
        : public Object
        , public IThread
    {
    public:
        InnerThread(
            /* [in] */ UsbMidiDeviceAndroid* owner);

        CARAPI Run();

    private:
        UsbMidiDeviceAndroid* mOwner;
    };

    class InnerRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerRunnable(
            /* [in] */ UsbMidiDeviceAndroid* owner,
            /* [in] */ Int32 endpointNumber,
            /* [in] */ ArrayOf<Byte>* bs);

        CARAPI Run();

    private:
        UsbMidiDeviceAndroid* mOwner;
        Int32 mEndpointNumber,
        AutoPtr< ArrayOf<Byte> > mBs
    };

public:
    /**
     * Audio interface subclass code for MIDI.
     */
    static const Int32 MIDI_SUBCLASS = 3;

    /**
     * Constructs a UsbMidiDeviceAndroid.
     * @param manager
     * @param device The USB device which this object is assocated with.
     */
    UsbMidiDeviceAndroid(
        /* [in] */ IUsbManager* manager,
        /* [in] */ IUsbDevice* device);

    /**
     * Register the own native pointer.
     */
    //@CalledByNative
    CARAPI_(void) RegisterSelf(
        /* [in] */ Int64 nativePointer);

    /**
     * Sends a USB-MIDI data to the device.
     * @param endpointNumber The endpoint number of the destination endpoint.
     * @param bs The data to be sent.
     */
    //@TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR2)
    //@CalledByNative
    CARAPI_(void) Send(
        /* [in] */ Int32 endpointNumber,
        /* [in] */ ArrayOf<Byte>* bs);

    /**
     * Returns the descriptors bytes of this device.
     * @return The descriptors bytes of this device.
     */
    //@CalledByNative
    CARAPI_(AutoPtr< ArrayOf<Byte> >) GetDescriptors();

    /**
     * Closes the device connection.
     */
    //@CalledByNative
    CARAPI_(void) Close();

private:
    /**
     * Starts listening for input endpoints.
     */
    CARAPI_(void) StartListen(
        /* [in] */ IUsbDevice* device);

    /**
     * Posts a data input event to the main thread.
     */
    CARAPI_(void) PostOnDataEvent(
        /* [in] */ Int32 endpointNumber,
        /* [in] */ ArrayOf<Byte>* bs);

    /**
     * Returns true if |bulkTransfer| should be used in |send|.
     * See comments in |send|.
     */
    CARAPI_(Boolean) ShouldUseBulkTransfer();

    /**
     * Returns the length of a USB-MIDI input.
     * Since the Android API doesn't provide us the length,
     * we calculate it manually.
     */
    static CARAPI_(Int32) GetInputDataLength(
        /* [in] */ IByteBuffer* buffer);

    static CARAPI_(void) NativeOnData(
        /* [in] */ Int64 nativeUsbMidiDeviceAndroid,
        /* [in] */ Int32 endpointNumber,
        /* [in] */ ArrayOf<Byte>* data);
private:
    /**
     * A connection handle for this device.
     */
    const AutoPtr<IUsbDeviceConnection> mConnection;

    /**
     * A map from endpoint number to UsbEndpoint.
     */
    const SparseArray<UsbEndpoint> mEndpointMap;

    /**
     * A map from UsbEndpoint to UsbRequest associated to it.
     */
    const Map<UsbEndpoint, UsbRequest> mRequestMap;

    /**
     * The handler used for posting events on the main thread.
     */
    AutoPtr<IHandler> mHandler;

    /**
     * True if this device is closed.
     */
    Boolean mIsClosed;

    /**
     * True if there is a thread processing input data.
     */
    Boolean mHasInputThread;

    /**
     * The identifier of this device.
     */
    Int64 mNativePointer;
};

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_MEDIA_USBMIDIDEVICEANDROID_H__
