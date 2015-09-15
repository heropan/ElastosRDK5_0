// wuweizuo automatic build .cpp file from .java file.

#include "PowerSaveBlocker.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                           PowerSaveBlocker
//=====================================================================
ECode PowerSaveBlocker::ApplyBlock(
    /* [in] */ ViewAndroid* view)
{
    VALIDATE_NOT_NULL(view);
    // ==================before translated======================
    // view.incrementKeepScreenOnCount();
    assert(0);
    return NOERROR;
}

ECode PowerSaveBlocker::RemoveBlock(
    /* [in] */ ViewAndroid* view)
{
    VALIDATE_NOT_NULL(view);
    // ==================before translated======================
    // view.decrementKeepScreenOnCount();
    assert(0);
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


