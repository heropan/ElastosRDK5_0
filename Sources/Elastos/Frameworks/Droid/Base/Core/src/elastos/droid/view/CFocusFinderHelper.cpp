
#include "elastos/droid/view/CFocusFinderHelper.h"
#include "elastos/droid/view/FocusFinder.h"

namespace Elastos {
namespace Droid {
namespace View {

CAR_INTERFACE_IMPL(CFocusFinderHelper, Singleton, IFocusFinderHelper)

CAR_SINGLETON_IMPL(CFocusFinderHelper)

ECode CFocusFinderHelper::constructor()
{
    return NOERROR;
}

ECode CFocusFinderHelper::GetInstance(
    /* [out] */ IFocusFinder** finder)
{
    AutoPtr<FocusFinder> result = FocusFinder::GetInstance();
    *finder = result;
    REFCOUNT_ADD(*finder)

    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
