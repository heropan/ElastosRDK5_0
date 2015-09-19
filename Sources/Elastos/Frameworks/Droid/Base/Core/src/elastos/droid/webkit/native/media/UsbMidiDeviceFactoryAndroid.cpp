
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

//===========================================================================
//            UsbMidiDeviceFactoryAndroid::InnerBroadcastReceiver
//===========================================================================

UsbMidiDeviceFactoryAndroid::InnerBroadcastReceiver::InnerBroadcastReceiver(
    /* [in] */ UsbMidiDeviceFactoryAndroid* owner)
    : mOwner(owner)
{
}

ECode UsbMidiDeviceFactoryAndroid::InnerBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(intent);

    String action;
    intent->GetAction(&action);
    if (ACTION_USB_PERMISSION.Equals(&action)) {
        OnRequestDone(context, intent);
    }

    return NOERROR;
}

//===============================================================
//                  UsbMidiDeviceFactoryAndroid
//===============================================================

const String UsbMidiDeviceFactoryAndroid::ACTION_USB_PERMISSION("org.chromium.media.USB_PERMISSION");

/**
 * Constructs a UsbMidiDeviceAndroid.
 * @param natviePointer The native pointer to which the created factory is associated.
 */
UsbMidiDeviceFactoryAndroid::UsbMidiDeviceFactoryAndroid(
    /* [in] */ Int64 nativePointer)
    : mNativePointer(nativePointer)
{
}

/**
 * Constructs a UsbMidiDeviceAndroid.
 * @param nativePointer The native pointer to which the created factory is associated.
 */
//@CalledByNative
AutoPtr<UsbMidiDeviceFactoryAndroid> UsbMidiDeviceFactoryAndroid::Create(
    /* [in] */ Int64 nativePointer)
{
    AutoPtr<UsbMidiDeviceFactoryAndroid> ret = new UsbMidiDeviceFactoryAndroid(nativePointer);
    return ret;
}

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
Boolean UsbMidiDeviceFactoryAndroid::EnumerateDevices(
    /* [in] */ IContext* context)
{
    mUsbManager = (UsbManager)context.getSystemService(Context.USB_SERVICE);
    Map<String, UsbDevice> devices = mUsbManager.getDeviceList();
    PendingIntent pendingIntent = PendingIntent.getBroadcast(
            context, 0, new Intent(ACTION_USB_PERMISSION), 0);
    mRequestedDevices = new HashSet<UsbDevice>();
    for (UsbDevice device : devices.values()) {
        boolean found = false;
        for (int i = 0; i < device.getInterfaceCount() && !found; ++i) {
            UsbInterface iface = device.getInterface(i);
            if (iface.getInterfaceClass() == UsbConstants.USB_CLASS_AUDIO &&
                iface.getInterfaceSubclass() == UsbMidiDeviceAndroid.MIDI_SUBCLASS) {
                found = true;
            }
        }
        if (found) {
            mUsbManager.requestPermission(device, pendingIntent);
            mRequestedDevices.add(device);
        }
    }
    if (mRequestedDevices.isEmpty()) {
        // No USB-MIDI devices are found.
        return FALSE;
    }

    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New(ACTION_USB_PERMISSION, (IIntentFilter**)&filter);
    mReceiver = new InnerBroadcastReceiver(this);
    context->RegisterReceiver(mReceiver, filter);

    return TRUE;
}

/**
 * Called when the user accepts or rejects the permission request requested by
 * EnumerateDevices.
 * If all permission requests are responded, this function calls
 * nativeOnUsbMidiDeviceRequestDone with the accessible USB-MIDI devices.
 */
void UsbMidiDeviceFactoryAndroid::OnRequestDone(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    AutoPtr<IUsbDevice> device;
    intent->GetParcelableExtra(IUsbManager::EXTRA_DEVICE, (IUsbDevice**)&device);
    if (!mRequestedDevices.contains(device)) {
        // We are not interested in the device.
        return;
    }

    mRequestedDevices.remove(device);
    if (!intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, FALSE)) {
        // The request was rejected.
        device = NULL;
    }

    if (device != NULL) {
        // Now we can add the device.
        AutoPtr<UsbMidiDeviceAndroid> uda = new UsbMidiDeviceAndroid(mUsbManager, device);
        mDevices.add(uda);
    }

    if (mRequestedDevices.isEmpty()) {
        // All requests are done.
        context->UnregisterReceiver(mReceiver);
        if (mNativePointer != 0) {
            NativeOnUsbMidiDeviceRequestDone(mNativePointer, mDevices.toArray());
        }
    }
}

/**
 * Disconnects the native object.
 */
//@CalledByNative
void UsbMidiDeviceFactoryAndroid::Close()
{
    mNativePointer = 0;
}

void UsbMidiDeviceFactoryAndroid::NativeOnUsbMidiDeviceRequestDone(
    /* [in] */ Int64 nativeUsbMidiDeviceFactoryAndroid,
    /* [in] */ ArrayOf<IInterface>* devices)
{
}

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
