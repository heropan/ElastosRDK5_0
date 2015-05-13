
#ifndef __DISPLAYDEVICEINFO_H__
#define __DISPLAYDEVICEINFO_H__

#include "Elastos.Droid.Server_server.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

/**
 * Describes the characteristics of a physical display device.
 */
class DisplayDeviceInfo : public ElRefBase
{
public:
    DisplayDeviceInfo();

    CARAPI_(void) SetAssumedDensityForExternalDisplay(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI_(Boolean) Equals(
        /* [in] */ DisplayDeviceInfo* other);

    //@Override
    CARAPI_(Int32) GetHashCode();

    CARAPI_(void) CopyFrom(
        /* [in] */ DisplayDeviceInfo* other);

    CARAPI_(String) ToString();

private:
    static CARAPI_(String) TouchToString(
        /* [in] */ Int32 touch);

    static CARAPI_(String) FlagsToString(
        /* [in] */ Int32 flags);

public:
    /**
     * Flag: Indicates that this display device should be considered the default display
     * device of the system.
     */
    static const Int32 FLAG_DEFAULT_DISPLAY = 1 << 0;

    /**
     * Flag: Indicates that the orientation of this display device is coupled to the
     * rotation of its associated logical display.
     * <p>
     * This flag should be applied to the default display to indicate that the user
     * physically rotates the display when content is presented in a different orientation.
     * The display manager will apply a coordinate transformation assuming that the
     * physical orientation of the display matches the logical orientation of its content.
     * </p><p>
     * The flag should not be set when the display device is mounted in a fixed orientation
     * such as on a desk.  The display manager will apply a coordinate transformation
     * such as a scale and translation to letterbox or pillarbox format under the
     * assumption that the physical orientation of the display is invariant.
     * </p>
     */
    static const Int32 FLAG_ROTATES_WITH_CONTENT = 1 << 1;

    /**
     * Flag: Indicates that this display device has secure video output, such as HDCP.
     */
    static const Int32 FLAG_SECURE = 1 << 2;

    /**
     * Flag: Indicates that this display device supports compositing
     * from gralloc protected buffers.
     */
    static const Int32 FLAG_SUPPORTS_PROTECTED_BUFFERS = 1 << 3;

    /**
     * Touch attachment: Display does not receive touch.
     */
    static const Int32 TOUCH_NONE = 0;

    /**
     * Touch attachment: Touch input is via the internal interface.
     */
    static const Int32 TOUCH_INTERNAL = 1;

    /**
     * Touch attachment: Touch input is via an external interface, such as USB.
     */
    static const Int32 TOUCH_EXTERNAL = 2;

     /**
     * Gets the name of the display device, which may be derived from
     * EDID or other sources.  The name may be displayed to the user.
     */
    String mName;

    /**
     * The width of the display in its natural orientation, in pixels.
     * This value is not affected by display rotation.
     */
    Int32 mWidth;

    /**
     * The height of the display in its natural orientation, in pixels.
     * This value is not affected by display rotation.
     */
    Int32 mHeight;

    /**
     * The refresh rate of the display.
     */
    Float mRefreshRate;

    /**
     * The nominal apparent density of the display in DPI used for layout calculations.
     * This density is sensitive to the viewing distance.  A big TV and a tablet may have
     * the same apparent density even though the pixels on the TV are much bigger than
     * those on the tablet.
     */
    Int32 mDensityDpi;

    /**
     * The physical density of the display in DPI in the X direction.
     * This density should specify the physical size of each pixel.
     */
    Float mXDpi;

    /**
     * The physical density of the display in DPI in the X direction.
     * This density should specify the physical size of each pixel.
     */
    Float mYDpi;

    /**
     * Display flags.
     */
    Int32 mFlags;

    /**
     * The touch attachment, per {@link DisplayViewport#touch}.
     */
    Int32 mTouch;

    /**
     * The additional rotation to apply to all content presented on the display device
     * relative to its physical coordinate system.  Default is {@link Surface#ROTATION_0}.
     * <p>
     * This field can be used to compensate for the fact that the display has been
     * physically rotated relative to its natural orientation such as an HDMI monitor
     * that has been mounted sideways to appear to be portrait rather than landscape.
     * </p>
     */
    Int32 mRotation;

    /**
     * Display type.
     */
    Int32 mType;

    /**
     * Display address, or null if none.
     * Interpretation varies by display type.
     */
    String mAddress;
};

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__DISPLAYDEVICEINFO_H__
