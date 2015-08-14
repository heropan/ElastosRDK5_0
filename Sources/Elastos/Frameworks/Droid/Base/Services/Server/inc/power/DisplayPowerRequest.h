#ifndef __ELASTOS_DROID_SERVER_POWER_DISPLAYPOWERREQUEST_H__
#define __ELASTOS_DROID_SERVER_POWER_DISPLAYPOWERREQUEST_H__

#include "Elastos.Droid.Server_server.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

/**
 * Describes the requested power state of the display.
 *
 * This object is intended to describe the general characteristics of the
 * power state, such as whether the screen should be on or off and the current
 * brightness controls leaving the {@link DisplayPowerController} to manage the
 * details of how the transitions between states should occur.  The goal is for
 * the {@link PowerManagerService} to focus on the global power state and not
 * have to micro-manage screen off animations, auto-brightness and other effects.
 */
class DisplayPowerRequest : public ElRefBase
{
public:
    DisplayPowerRequest();

    DisplayPowerRequest(
        /* [in] */ DisplayPowerRequest* other);

    CARAPI_(void) CopyFrom(
        /* [in] */ DisplayPowerRequest* other);

    Boolean Equals(
        /* [in] */ DisplayPowerRequest* other);

public:
    static const Int32 SCREEN_STATE_OFF = 0;
    static const Int32 SCREEN_STATE_DIM = 1;
    static const Int32 SCREEN_STATE_BRIGHT = 2;

    // The requested minimum screen power state: off, dim or bright.
    Int32 mScreenState;

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

    // If true, prevents the screen from completely turning on if it is currently off.
    // The display does not enter a "ready" state if this flag is true and screen on is
    // blocked.  The window manager policy blocks screen on while it prepares the keyguard to
    // prevent the user from seeing intermediate updates.
    //
    // Technically, we may not block the screen itself from turning on (because that introduces
    // extra unnecessary latency) but we do prevent content on screen from becoming
    // visible to the user.
    Boolean mBlockScreenOn;
};

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_POWER_DISPLAYPOWERREQUEST_H__
