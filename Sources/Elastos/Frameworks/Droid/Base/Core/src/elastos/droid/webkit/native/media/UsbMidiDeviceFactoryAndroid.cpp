#include "elastos/droid/webkit/native/media/UsbMidiDeviceFactoryAndroid.h"
#include "elastos/droid/webkit/native/media/UsbMidiDeviceAndroid.h"

#include "elastos/droid/content/CIntentFilter.h"
//TODO #include "elastos/droid/app/CPendingIntentHelper.h"
#include "elastos/droid/content/CIntent.h"

//TODO #include <elastos/utility/CHashSet.h>
//TODO #include <elastos/utility/CArrayList.h>

using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::App::IPendingIntentHelper;
//TODO using Elastos::Droid::App::CPendingIntentHelper;
using Elastos::Droid::Hardware::Usb::IUsbConstants;
using Elastos::Droid::Hardware::Usb::IUsbDevice;
using Elastos::Droid::Hardware::Usb::IUsbInterface;
using Elastos::Droid::Hardware::Usb::IUsbManager;

using Elastos::Utility::IMap;
//TODO using Elastos::Utility::CHashSet;
//TODO using Elastos::Utility::CArrayList;
using Elastos::Utility::ICollection;
using Elastos::Utility::IIterator;

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
    if (ACTION_USB_PERMISSION.Equals(action)) {
        mOwner->OnRequestDone(context, intent);
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
    //TODO CArrayList::New((IList**)&mDevices);
}

/**
 * Constructs a UsbMidiDeviceAndroid.
 * @param nativePointer The native pointer to which the created factory is associated.
 */
//@CalledByNative return UsbMidiDeviceFactoryAndroid
AutoPtr<IInterface> UsbMidiDeviceFactoryAndroid::Create(
    /* [in] */ Int64 nativePointer)
{
    AutoPtr<UsbMidiDeviceFactoryAndroid> usbmdFactory = new UsbMidiDeviceFactoryAndroid(nativePointer);
    AutoPtr<IInterface> ret = usbmdFactory->Probe(EIID_IInterface);
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
    context->GetSystemService(IContext::USB_SERVICE, (IInterface**)&mUsbManager);
    //Map<String, UsbDevice> devices = mUsbManager.GetDeviceList();
    AutoPtr<IMap> devices;
    mUsbManager->GetDeviceList((IMap**)&devices);
    AutoPtr<IPendingIntentHelper> piHelper;
    //TODO CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&piHelper);
    //PendingIntent pendingIntent = PendingIntent.getBroadcast(
    //        context, 0, new Intent(ACTION_USB_PERMISSION), 0);
    AutoPtr<IPendingIntent> pendingIntent;
    AutoPtr<IIntent> intent;
    CIntent::New(ACTION_USB_PERMISSION, (IIntent**)&intent);
    piHelper->GetBroadcast(context, 0, intent, 0, (IPendingIntent**)&pendingIntent);
    //mRequestedDevices = new HashSet<UsbDevice>();
    //TODO CHashSet::New((ISet**)&mRequestedDevices);
    //for (UsbDevice device : devices.values())
    AutoPtr<ICollection> devicesValues;
    devices->GetValues((ICollection**)&devicesValues);
    AutoPtr<IIterator> it;
    devicesValues->GetIterator((IIterator**)&it);
    Boolean hasNext = FALSE;
    while (it->HasNext(&hasNext), hasNext)
    {
        AutoPtr<IUsbDevice> device;
        it->GetNext((IInterface**)&device);
        Boolean found = false;
        Int32 interfaceCount;
        device->GetInterfaceCount(&interfaceCount);
        for (Int32 i = 0; i < interfaceCount && !found; ++i) {
            //UsbInterface iface = device.getInterface(i);
            AutoPtr<IUsbInterface> iface;
            device->GetInterface(i, (IUsbInterface**)&iface);
            Int32 interfaceClass, interfaceSubclass;
            iface->GetInterfaceClass(&interfaceClass);
            iface->GetInterfaceSubclass(&interfaceSubclass);
            if (interfaceClass  == IUsbConstants::_USB_CLASS_AUDIO &&
                interfaceSubclass == UsbMidiDeviceAndroid::MIDI_SUBCLASS) {
                found = true;
            }
        }
        if (found) {
            //java call this mUsbManager->RequestPermission(device, pendingIntent);
            mUsbManager->RequestDevicePermission(device, pendingIntent);
            mRequestedDevices->Add(device);
        }
    }
    Boolean isEmpty;
    mRequestedDevices->IsEmpty(&isEmpty);
    if (isEmpty) {
        // No USB-MIDI devices are found.
        return FALSE;
    }

    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New(ACTION_USB_PERMISSION, (IIntentFilter**)&filter);
    mReceiver = new InnerBroadcastReceiver(this);
    AutoPtr<IIntent> intentRR;
    context->RegisterReceiver(mReceiver, filter, (IIntent**)&intentRR);

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
    AutoPtr<IParcelable> parcelable;
    intent->GetParcelableExtra(IUsbManager::EXTRA_DEVICE, (IParcelable**)&parcelable);
    device = IUsbDevice::Probe(parcelable);
    Boolean contain;
    mRequestedDevices->Contains(device, &contain);
    if (!contain) {
        // We are not interested in the device.
        return;
    }

    mRequestedDevices->Remove(device);
    Boolean value;
    intent->GetBooleanExtra(IUsbManager::EXTRA_PERMISSION_GRANTED, FALSE, &value);
    if (!value) {
        // The request was rejected.
        device = NULL;
    }

    if (device != NULL) {
        // Now we can add the device.
        AutoPtr<UsbMidiDeviceAndroid> uda = new UsbMidiDeviceAndroid(mUsbManager, device);
        AutoPtr<IInterface> iuda = uda->Probe(EIID_IInterface);
        mDevices->Add(iuda);
    }

    Boolean isEmpty;
    mRequestedDevices->IsEmpty(&isEmpty);
    if (isEmpty) {
        // All requests are done.
        context->UnregisterReceiver(mReceiver);
        if (mNativePointer != 0) {
            AutoPtr<ArrayOf<IInterface*> > arrayDevices;
            mDevices->ToArray((ArrayOf<IInterface*>**)&arrayDevices);
            NativeOnUsbMidiDeviceRequestDone(mNativePointer, arrayDevices);
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
    /* [in] */ ArrayOf<IInterface*>* devices)
{
}

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
