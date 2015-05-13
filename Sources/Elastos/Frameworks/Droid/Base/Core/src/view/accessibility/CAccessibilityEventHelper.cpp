
#include "ext/frameworkdef.h"
#include "view/accessibility/CAccessibilityEventHelper.h"
#include "view/accessibility/CAccessibilityEvent.h"

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

ECode CAccessibilityEventHelper::ObtainEx(
    /* [in] */ IAccessibilityEvent* otherEvent,
    /* [out] */ IAccessibilityEvent** event)
{
    VALIDATE_NOT_NULL(event);
    return CAccessibilityEvent::ObtainEx(otherEvent, event);
}

ECode CAccessibilityEventHelper::ObtainEx2(
    /* [out] */ IAccessibilityEvent** event)
{
    VALIDATE_NOT_NULL(event);
    return CAccessibilityEvent::ObtainEx2(event);
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
