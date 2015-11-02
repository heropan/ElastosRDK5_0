#include "elastos/droid/view/accessibility/CAccessibilityActionHelper.h"
#include "elastos/droid/view/accessibility/CAccessibilityAction.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CAR_INTERFACE_IMPL(CAccessibilityActionHelper, Singleton, IAccessibilityActionHelper)

CAR_SINGLETON_IMPL(CAccessibilityActionHelper)

ECode CAccessibilityActionHelper::GetACTION_FOCUS(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_FOCUS;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_CLEAR_FOCUS(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_CLEAR_FOCUS;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_SELECT(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_SELECT;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_CLEAR_SELECTION(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_CLEAR_SELECTION;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_CLICK(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_CLICK;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_LONG_CLICK(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_LONG_CLICK;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_ACCESSIBILITY_FOCUS(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_ACCESSIBILITY_FOCUS;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_CLEAR_ACCESSIBILITY_FOCUS(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_CLEAR_ACCESSIBILITY_FOCUS;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_NEXT_AT_MOVEMENT_GRANULARITY(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_NEXT_AT_MOVEMENT_GRANULARITY;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_NEXT_HTML_ELEMENT(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_NEXT_HTML_ELEMENT;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_PREVIOUS_HTML_ELEMENT(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_PREVIOUS_HTML_ELEMENT;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_SCROLL_FORWARD(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_SCROLL_FORWARD;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_SCROLL_BACKWARD(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_SCROLL_BACKWARD;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_COPY(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_COPY;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_PASTE(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_PASTE;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_CUT(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_CUT;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_SET_SELECTION(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_SET_SELECTION;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_EXPAND(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_EXPAND;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_COLLAPSE(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_COLLAPSE;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_DISMISS(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_DISMISS;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode CAccessibilityActionHelper::GetACTION_SET_TEXT(
    /* [out] */ IAccessibilityAction** action)
{
    VALIDATE_NOT_NULL(action);
    *action = CAccessibilityAction::ACTION_SET_TEXT;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

} // Accessibility
} // View
} // Droid
} // Elastos
