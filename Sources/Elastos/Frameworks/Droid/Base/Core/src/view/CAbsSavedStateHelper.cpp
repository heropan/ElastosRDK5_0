
#include "view/CAbsSavedStateHelper.h"
#include "view/AbsSavedState.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CAbsSavedStateHelper::GetEMPTY_STATE(
    /* [out] */ IAbsSavedState** emptyState)
{
    VALIDATE_NOT_NULL(emptyState)
    *emptyState = AbsSavedState::EMPTY_STATE;
    REFCOUNT_ADD(*emptyState)
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
