
#ifndef __ELASTOS_DROID_WEBKIT_MEDIA_USBMIDIDEVICEFACTORYANDROID_H__
#define __ELASTOS_DROID_WEBKIT_MEDIA_USBMIDIDEVICEFACTORYANDROID_H__

// import android.app.PendingIntent;
// import android.content.BroadcastReceiver;
// import android.content.Context;
// import android.content.Intent;
// import android.content.IntentFilter;
// import android.hardware.usb.UsbConstants;
// import android.hardware.usb.UsbDevice;
// import android.hardware.usb.UsbInterface;
// import android.hardware.usb.UsbManager;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

// import java.util.ArrayList;
// import java.util.HashSet;
// import java.util.List;
// import java.util.Map;
// import java.util.Set;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

/**
 * Owned by its native counterpart declared in
 * usb_midi_device_factory_android.h. Refer to that class for general comments.
 */
//@JNINamespace("media")
class UsbMidiDeviceFactoryAndroid
{
private:
    class InnerBroadcastReceiver
        : public Object
        , public IBroadcastReceiver
    {
    public:
        InnerBroadcastReceiver(
            /* [in] */ UsbMidiDeviceFactoryAndroid* owner);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        UsbMidiDeviceFactoryAndroid* mOwner;
    };

public:
    /**
     * Constructs a UsbMidiDeviceAndroid.
     * @param natviePointer The native pointer to which the created factory is associated.
     */
    UsbMidiDeviceFactoryAndroid(
        /* [in] */ Int64 nativePointer);

    /**
     * Constructs a UsbMidiDeviceAndroid.
     * @param nativePointer The native pointer to which the created factory is associated.
     */
    //@CalledByNative
    static CARAPI_(AutoPtr<UsbMidiDeviceFactoryAndroid>) Create(
        /* [in] */ Int64 nativePointer);

    /**
     * Enumerates USB-MIDI devices.
     * If there are devices having USB-MIDI interfaces, this function requests permission for
     * accessing the device to the user.
     * When the permission request is accepted or rejected onRequestDone will be called.
     *
     * If there are no USB-MIDI interfaces, this function returns false.
     * @return true if some permission requests are in progress.
     */
    //@CalledByNative
    CARAPI_(Boolean) EnumerateDevices(
        /* [in] */ IContext* context);

    /**
     * Disconnects the native object.
     */
    //@CalledByNative
    CARAPI_(void) Close();

private:
    /**
     * Called when the user accepts or rejects the permission request requested by
     * EnumerateDevices.
     * If all permission requests are responded, this function calls
     * nativeOnUsbMidiDeviceRequestDone with the accessible USB-MIDI devices.
     */
    CARAPI_(void) OnRequestDone(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

    static CARAPI_(void) NativeOnUsbMidiDeviceRequestDone(
        /* [in] */ Int64 nativeUsbMidiDeviceFactoryAndroid,
        /* [in] */ ArrayOf<IInterface> devices);

private:
    /**
     * The UsbManager of this system.
     */
    AutoPtr<IUsbManager> mUsbManager;

    /**
     * A BroadcastReceiver for USB device permission requests.
     */
    AutoPtr<IBroadcastReceiver> mReceiver;

    /**
     * Accessible USB-MIDI devices got so far.
     */
    List<UsbMidiDeviceAndroid> mDevices;

    /**
     * Devices whose access permission requested but not resolved so far.
     */
    Set<UsbDevice> mRequestedDevices;

    /**
     * The identifier of this factory.
     */
    Int64 mNativePointer;

    static const String ACTION_USB_PERMISSION;
};

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_MEDIA_USBMIDIDEVICEFACTORYANDROID_H__
