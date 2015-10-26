
#include "elastos/droid/webkit/native/content/browser/PowerSaveBlocker.h"
#include "elastos/droid/webkit/native/ui/base/ViewAndroid.h"

using Elastos::Droid::Webkit::Ui::Base::ViewAndroid;

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
    return view->IncrementKeepScreenOnCount();
}

ECode PowerSaveBlocker::RemoveBlock(
    /* [in] */ ViewAndroid* view)
{
    return view->DecrementKeepScreenOnCount();
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
