#ifndef __ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYACTIONHELPER_H__
#define __ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYACTIONHELPER_H__

#include "_Elastos_Droid_View_Accessibility_CAccessibilityActionHelper.h"
#include <elastos/core/Singleton.h>

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CAccessibilityActionHelper)
    , public Singleton
    , public IAccessibilityActionHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI GetACTION_FOCUS(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_CLEAR_FOCUS(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_SELECT(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_CLEAR_SELECTION(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_CLICK(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_LONG_CLICK(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_ACCESSIBILITY_FOCUS(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_CLEAR_ACCESSIBILITY_FOCUS(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_NEXT_AT_MOVEMENT_GRANULARITY(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_NEXT_HTML_ELEMENT(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_PREVIOUS_HTML_ELEMENT(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_SCROLL_FORWARD(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_SCROLL_BACKWARD(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_COPY(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_PASTE(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_CUT(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_SET_SELECTION(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_EXPAND(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_COLLAPSE(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_DISMISS(
        /* [out] */ IAccessibilityAction** action);

    CARAPI GetACTION_SET_TEXT(
        /* [out] */ IAccessibilityAction** action);
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYACTIONHELPER_H__
