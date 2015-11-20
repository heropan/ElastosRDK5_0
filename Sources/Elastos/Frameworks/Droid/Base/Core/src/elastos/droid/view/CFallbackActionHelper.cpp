
#include "elastos/droid/view/CFallbackActionHelper.h"
#include "elastos/droid/view/KeyCharacterMap.h"

namespace Elastos {
namespace Droid {
namespace View {

CAR_INTERFACE_IMPL(CFallbackActionHelper, Singleton, IFallbackActionHelper);

CAR_SINGLETON_IMPL(CFallbackActionHelper);

ECode CFallbackActionHelper::Obtain(
    /* [out] */ IFallbackAction** action)
{
    VALIDATE_NOT_NULL(action);

    AutoPtr<KeyCharacterMap::FallbackAction> target =
            KeyCharacterMap::FallbackAction::Obtain();
    *action = (IFallbackAction*)target.Get();
    REFCOUNT_ADD(*action);

    return NOERROR;
}

}   //namespace View
}   //namespace Droid
}   //namespace Elastos
