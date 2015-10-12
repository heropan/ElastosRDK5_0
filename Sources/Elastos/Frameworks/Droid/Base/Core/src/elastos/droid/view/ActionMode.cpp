
#include "view/ActionMode.h"

namespace Elastos {
namespace Droid {
namespace View {

CAR_INTERFACE_IMPL(ActionMode, Object, IActionMode)

ActionMode::ActionMode()
    : mTitleOptionalHint(FALSE)
{
}

ECode ActionMode::SetTag(
    /* [in] */ IInterface* tag)
{
    mTag = tag;
    return NOERROR;
}

ECode ActionMode::GetTag(
    /* [out] */ IInterface** tag)
{
    VALIDATE_NOT_NULL(tag);
    *tag = mTag;
    REFCOUNT_ADD(*tag);
    return NOERROR;
}

ECode ActionMode::SetTitleOptionalHint(
    /* [in] */ Boolean titleOptional)
{
    mTitleOptionalHint = titleOptional;
    return NOERROR;
}

ECode ActionMode::GetTitleOptionalHint(
    /* [out] */ Boolean* titleOptionalHint)
{
    VALIDATE_NOT_NULL(titleOptionalHint);
    *titleOptionalHint = mTitleOptionalHint;
    return NOERROR;
}

ECode ActionMode::IsTitleOptional(
    /* [out] */ Boolean* isTitleOptional)
{
    VALIDATE_NOT_NULL(isTitleOptional);
    *isTitleOptional = FALSE;
    return NOERROR;
}

ECode ActionMode::IsUiFocusable(
    /* [out] */ Boolean* isUiFocusable)
{
    VALIDATE_NOT_NULL(isUiFocusable);
    *isUiFocusable = TRUE;
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
