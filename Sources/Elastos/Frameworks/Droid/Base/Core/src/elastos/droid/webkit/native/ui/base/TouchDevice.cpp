
#include "elastos/droid/webkit/native/ui/base/TouchDevice.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

//=====================================================================
//                             TouchDevice
//=====================================================================
TouchDevice::TouchDevice()
{
}

Int32 TouchDevice::MaxTouchPoints(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // // Android only tells us if the device belongs to a "Touchscreen Class"
    // // which only guarantees a minimum number of touch points. Be
    // // conservative and return the minimum, checking membership from the
    // // highest class down.
    // if (context.getPackageManager().hasSystemFeature(
    //         PackageManager.FEATURE_TOUCHSCREEN_MULTITOUCH_JAZZHAND)) {
    //     return 5;
    // } else if (context.getPackageManager().hasSystemFeature(
    //         PackageManager.FEATURE_TOUCHSCREEN_MULTITOUCH_DISTINCT)) {
    //     return 2;
    // } else if (context.getPackageManager().hasSystemFeature(
    //         PackageManager.FEATURE_TOUCHSCREEN_MULTITOUCH)) {
    //     return 2;
    // } else if (context.getPackageManager().hasSystemFeature(
    //         PackageManager.FEATURE_TOUCHSCREEN)) {
    //     return 1;
    // } else {
    //     return 0;
    // }
    assert(0);
    return 0;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


