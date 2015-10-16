
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/view/accessibility/CAccessibilityEventHelper.h"
#include "elastos/droid/view/accessibility/CAccessibilityEvent.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

ECode CAccessibilityEventHelper::Obtain(
    /* [in] */ Int32 eventType,
    /* [out] */ IAccessibilityEvent** event)
{
    VALIDATE_NOT_NULL(event);
    return CAccessibilityEvent::Obtain(eventType, event);
}

ECode CAccessibilityEventHelper::Obtain(
    /* [in] */ IAccessibilityEvent* otherEvent,
    /* [out] */ IAccessibilityEvent** event)
{
    VALIDATE_NOT_NULL(event);
    return CAccessibilityEvent::Obtain(otherEvent, event);
}

ECode CAccessibilityEventHelper::Obtain(
    /* [out] */ IAccessibilityEvent** event)
{
    VALIDATE_NOT_NULL(event);
    return CAccessibilityEvent::Obtain(event);
}

ECode CAccessibilityEventHelper::EventTypeToString(
    /* [in] */ Int32 eventType,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = CAccessibilityEvent::EventTypeToString(eventType);
    return NOERROR;
}

} // Accessibility
} // View
} // Droid
} // Elastos
