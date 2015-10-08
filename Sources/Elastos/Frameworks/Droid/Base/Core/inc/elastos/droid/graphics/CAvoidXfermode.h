
#ifndef __ELASTOS_DROID_GRAPHICS_CAVOIDXFERMODE_H__
#define __ELASTOS_DROID_GRAPHICS_CAVOIDXFERMODE_H__

#include "_Elastos_Droid_Graphics_CAvoidXfermode.h"
#include "graphics/Xfermode.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

/**
 * AvoidXfermode xfermode will draw the src everywhere except on top of the
 * opColor or, depending on the Mode, draw only on top of the opColor.
 */
CarClass(CAvoidXfermode)
    , public Xfermode
    , public IAvoidXfermode
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    /** This xfermode draws, or doesn't draw, based on the destination's
     * distance from an op-color.
     *
     * There are two modes, and each mode interprets a tolerance value.
     *
     * Avoid: In this mode, drawing is allowed only on destination pixels that
     * are different from the op-color.
     * Tolerance near 0: avoid any colors even remotely similar to the op-color
     * Tolerance near 255: avoid only colors nearly identical to the op-color
     *
     * Target: In this mode, drawing only occurs on destination pixels that
     * are similar to the op-color
     * Tolerance near 0: draw only on colors that are nearly identical to the op-color
     * Tolerance near 255: draw on any colors even remotely similar to the op-color
     */
    CARAPI constructor(
        /* [in] */ Int32 opColor,
        /* [in] */ Int32 tolerance,
        /* [in] */ Int32 mode);

private:
    static CARAPI_(Int64) NativeCreate(
        /* [in] */ Int32 opColor,
        /* [in] */ Int32 tolerance,
        /* [in] */ Int32 nativeMode);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CAVOIDXFERMODE_H__
