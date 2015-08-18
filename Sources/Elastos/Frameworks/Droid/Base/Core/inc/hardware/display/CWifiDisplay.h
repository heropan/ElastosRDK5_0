
#ifndef __ELASTOS_DROID_HARDWARE_DISPLAY_CWIFIDISPLAY_H__
#define __ELASTOS_DROID_HARDWARE_DISPLAY_CWIFIDISPLAY_H__

#include "_Elastos_Droid_Hardware_Display_CWifiDisplay.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

/**
 * Describes the properties of a Wifi display.
 * <p>
 * This object is immutable.
 * </p>
 *
 * @hide
 */
CarClass(CWifiDisplay)
{
public:
    static const AutoPtr<ArrayOf<IWifiDisplay*> > EMPTY_ARRAY;

public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& deviceAddress,
        /* [in] */ const String& deviceName,
        /* [in] */ const String& deviceAlias);

    /**
     * Gets the MAC address of the Wifi display device.
     */
    CARAPI GetDeviceAddress(
        /* [out] */ String* deviceAddress);

    /**
     * Gets the name of the Wifi display device.
     */
    CARAPI GetDeviceName(
        /* [out] */ String* deviceName);

    /**
     * Gets the user-specified alias of the Wifi display device, or null if none.
     * <p>
     * The alias should be used in the UI whenever available.  It is the value
     * provided by the user when renaming the device.
     * </p>
     */
    CARAPI GetDeviceAlias(
        /* [out] */ String* deviceAlias);

    /**
     * Gets the name to show in the UI.
     * Uses the device alias if available, otherwise uses the device name.
     */
    CARAPI GetFriendlyDisplayName(
        /* [out] */ String* name);

    CARAPI Equals(
        /* [in] */ IWifiDisplay* other,
        /* [out] */ Boolean* res);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* res);

    CARAPI GetHashCode(
        /* [out] */ Int32* code);

    CARAPI HasSameAddress(
        /* [in] */ IWifiDisplay* other,
        /* [out] */ Boolean* res);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    //@Override
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    // public static final Creator<WifiDisplay> CREATOR = new Creator<WifiDisplay>() {
    //     public WifiDisplay createFromParcel(Parcel in) {
    //         String deviceAddress = in.readString();
    //         String deviceName = in.readString();
    //         String deviceAlias = in.readString();
    //         return new WifiDisplay(deviceAddress, deviceName, deviceAlias);
    //     }

    //     public WifiDisplay[] newArray(int size) {
    //         return size == 0 ? EMPTY_ARRAY : new WifiDisplay[size];
    //     }
    // };

    CARAPI ToString(
        /* [out] */ String* info);
private:
    String mDeviceAddress;
    String mDeviceName;
    String mDeviceAlias;
};

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_HARDWARE_DISPLAY_CWIFIDISPLAY_H__
