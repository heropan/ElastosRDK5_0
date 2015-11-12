
#ifndef __ELASTOS_DROID_HARDWARE_DISPLAY_CDISPLAYPOWERREQUEST_H__
#define __ELASTOS_DROID_HARDWARE_DISPLAY_CDISPLAYPOWERREQUEST_H__

#include "_Elastos_Droid_Hardware_Display_CDisplayPowerRequest.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>


namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

/**
 * Interface for receiving information about a {@link VirtualDisplay}'s state changes.
 */
CarClass(CDisplayPowerRequest)
    , public Object
    , public IDisplayPowerRequest
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IDisplayPowerRequest* other);

    CARAPI IsBrightOrDim(
        /* [out] */ Boolean* result);

    CARAPI CopyFrom(
        /* [in] */ IDisplayPowerRequest* other);

    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* result);

    CARAPI Equals(
        /* [in] */ IDisplayPowerRequest* other,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hc);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI PolicyToString(
        /* [in] */ Int32 policy,
        /* [out] */ String* result);

public:
    // The basic overall policy to apply: off, doze, dim or bright.
    Int32 mPolicy;

    // If true, the proximity sensor overrides the screen state when an object is
    // nearby, turning it off temporarily until the object is moved away.
    Boolean mUseProximitySensor;

    // The desired screen brightness in the range 0 (minimum / off) to 255 (brightest).
    // The display power controller may choose to clamp the brightness.
    // When auto-brightness is enabled, this field should specify a nominal default
    // value to use while waiting for the light sensor to report enough data.
    Int32 mScreenBrightness;

    // The screen auto-brightness adjustment factor in the range -1 (dimmer) to 1 (brighter).
    Float mScreenAutoBrightnessAdjustment;

    // If true, enables automatic brightness control.
    Boolean mUseAutoBrightness;

    //If true, scales the brightness to half of desired.
    Boolean mLowPowerMode;

    // If true, prevents the screen from completely turning on if it is currently off.
    // The display does not enter a "ready" state if this flag is true and screen on is
    // blocked.  The window manager policy blocks screen on while it prepares the keyguard to
    // prevent the user from seeing intermediate updates.
    //
    // Technically, we may not block the screen itself from turning on (because that introduces
    // extra unnecessary latency) but we do prevent content on screen from becoming
    // visible to the user.
    Boolean mBlockScreenOn;

    // Overrides the policy for adjusting screen brightness and state while dozing.
    Int32 mDozeScreenBrightness;
    Int32 mDozeScreenState;
};

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_HARDWARE_DISPLAY_CDISPLAYPOWERREQUEST_H__
