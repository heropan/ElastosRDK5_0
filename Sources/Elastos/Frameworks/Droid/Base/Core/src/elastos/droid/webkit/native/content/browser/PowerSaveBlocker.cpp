
#include "webkit/native/content/browser/PowerSaveBlocker.h"
//TODO #include "webkit/native/ui/base/ViewAndroid.h"

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
    assert(0);
    // TODO
    // view->IncrementKeepScreenOnCount();
    return NOERROR;
}

ECode PowerSaveBlocker::RemoveBlock(
    /* [in] */ ViewAndroid* view)
{
    assert(0);
    // TODO
    // view->DecrementKeepScreenOnCount();
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
