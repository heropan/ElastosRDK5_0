
#ifndef __CWIFIDISPLAYSTATUS_H__
#define __CWIFIDISPLAYSTATUS_H__

#include "_CWifiDisplayStatus.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

/**
 * Describes the current global state of Wifi display connectivity, including the
 * currently connected display and all available or remembered displays.
 * <p>
 * This object is immutable.
 * </p>
 *
 * @hide
 */
CarClass(CWifiDisplayStatus)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 featureState,
        /* [in] */ Int32 scanState,
        /* [in] */ Int32 activeDisplayState,
        /* [in] */ IWifiDisplay* activeDisplay,
        /* [in] */ ArrayOf<IWifiDisplay*>* availableDisplays,
        /* [in] */ ArrayOf<IWifiDisplay*>* rememberedDisplays);

    /**
     * Returns the state of the Wifi display feature on this device.
     * <p>
     * The value of this property reflects whether the device supports the Wifi display,
     * whether it has been enabled by the user and whether the prerequisites for
     * connecting to displays have been met.
     * </p>
     */
    CARAPI GetFeatureState(
        /* [out] */ Int32* state);

    /**
     * Returns the current state of the Wifi display scan.
     *
     * @return One of: {@link #SCAN_STATE_NOT_SCANNING} or {@link #SCAN_STATE_SCANNING}.
     */
    CARAPI GetScanState(
        /* [out] */ Int32* state);

    /**
     * Get the state of the currently active display.
     *
     * @return One of: {@link #DISPLAY_STATE_NOT_CONNECTED}, {@link #DISPLAY_STATE_CONNECTING},
     * or {@link #DISPLAY_STATE_CONNECTED}.
     */
    CARAPI GetActiveDisplayState(
        /* [out] */ Int32* state);

    /**
     * Gets the Wifi display that is currently active.  It may be connecting or
     * connected.
     */
    CARAPI GetActiveDisplay(
        /* [out] */ IWifiDisplay** display);

    /**
     * Gets the list of all available Wifi displays as reported by the most recent
     * scan, never null.
     * <p>
     * Some of these displays may already be remembered, others may be unknown.
     * </p>
     */
    CARAPI GetAvailableDisplays(
        /* [out] */ ArrayOf<IWifiDisplay*>** displays);

    /**
     * Gets the list of all remembered Wifi displays, never null.
     * <p>
     * Not all remembered displays will necessarily be available.
     * </p>
     */
    CARAPI GetRememberedDisplays(
        /* [out] */ ArrayOf<IWifiDisplay*>** displays);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ToString(
        /* [out] */ String* info);

private:
    Int32 mFeatureState;
    Int32 mScanState;
    Int32 mActiveDisplayState;
    AutoPtr<IWifiDisplay> mActiveDisplay;
    AutoPtr<ArrayOf<IWifiDisplay*> > mAvailableDisplays;
    AutoPtr<ArrayOf<IWifiDisplay*> > mRememberedDisplays;
};

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos

#endif // __CWIFIDISPLAYSTATUS_H__
