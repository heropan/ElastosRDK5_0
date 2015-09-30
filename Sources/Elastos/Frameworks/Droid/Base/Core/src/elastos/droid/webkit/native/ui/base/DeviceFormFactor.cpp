
#include "elastos/droid/webkit/native/ui/base/DeviceFormFactor.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

//=====================================================================
//                           DeviceFormFactor
//=====================================================================
const Int32 DeviceFormFactor::MINIMUM_TABLET_WIDTH_DP;
Boolean DeviceFormFactor::sIsTablet = FALSE;

Boolean DeviceFormFactor::IsTablet(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // if (sIsTablet == null) {
    //     int minimumScreenWidthDp = context.getResources().getConfiguration().
    //             smallestScreenWidthDp;
    //     sIsTablet = minimumScreenWidthDp >= MINIMUM_TABLET_WIDTH_DP;
    // }
    // return sIsTablet;
    assert(0);
    return FALSE;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


