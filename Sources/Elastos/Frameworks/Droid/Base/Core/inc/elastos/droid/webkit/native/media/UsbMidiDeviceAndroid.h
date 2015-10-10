#ifndef __ELASTOS_DROID_WEBKIT_MEDIA_USBMIDIDEVICEANDROID_H__
#define __ELASTOS_DROID_WEBKIT_MEDIA_USBMIDIDEVICEANDROID_H__
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/utility/etl/HashMap.h"
#include "elastos/core/Thread.h"
// import android.annotation.TargetApi;
using Elastos::Droid::Hardware::Usb::IUsbManager;
using Elastos::Droid::Hardware::Usb::IUsbDevice;
using Elastos::Droid::Hardware::Usb::IUsbDeviceConnection;
using Elastos::Droid::Hardware::Usb::IUsbEndpoint;
using Elastos::Droid::Hardware::Usb::IUsbRequest;
using Elastos::Droid::Os::IHandler;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import java.util.HashMap;
// import java.util.Map;

using Elastos::Core::IThread;
using Elastos::Core::Thread;
using Elastos::Core::IRunnable;
using Elastos::IO::IByteBuffer;
using Elastos::Droid::Utility::ISparseArray;
using Elastos::Utility::Etl::HashMap;

_ETL_NAMESPACE_BEGIN
template<> struct Hash<AutoPtr<Elastos::Droid::Hardware::Usb::IUsbEndpoint> >
{
            size_t operator() (AutoPtr<IUsbEndpoint> x) const { return (size_t)x.Get(); }
};
_ETL_NAMESPACE_END

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
:public Object
{
private:
    class InnerThread
        : public Thread
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
        CAR_INTERFACE_DECL();
        InnerRunnable(
            /* [in] */ UsbMidiDeviceAndroid* owner,
            /* [in] */ Int32 endpointNumber,
            /* [in] */ ArrayOf<Byte>* bs);

        CARAPI Run();

    private:
        UsbMidiDeviceAndroid* mOwner;
        Int32 mEndpointNumber;
        AutoPtr<ArrayOf<Byte> > mBs;
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
    CARAPI_(AutoPtr<ArrayOf<Byte> >) GetDescriptors();

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
    AutoPtr<IUsbDeviceConnection> mConnection;

    /**
     * A map from endpoint number to UsbEndpoint.
     */
    AutoPtr<ISparseArray> mEndpointMap;

    /**
     * A map from UsbEndpoint to UsbRequest associated to it.
     */
    //AutoPtr<IMap> mRequestMap;
    HashMap<AutoPtr<IUsbEndpoint>, AutoPtr<IUsbRequest> > mRequestMap;
    HashMap<AutoPtr<IUsbEndpoint>, AutoPtr<IByteBuffer> > mBufferForEndpoints;

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
