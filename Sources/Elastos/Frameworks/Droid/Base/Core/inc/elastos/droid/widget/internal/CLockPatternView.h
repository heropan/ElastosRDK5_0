#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_CLOCKPATTERNVIEW_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_CLOCKPATTERNVIEW_H__

#include "_Elastos_Droid_Widget_Internal_CLockPatternView.h"

#include "elastos/droid/widget/internal/LockPatternView.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CLockPatternView), public LockPatternView
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI IsInStealthMode(
        /* [out] */ Boolean* mode);

    CARAPI IsTactileFeedbackEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetInStealthMode(
       /* [in] */ Boolean inStealthMode);

    CARAPI SetTactileFeedbackEnabled(
       /* [in] */ Boolean tactileFeedbackEnabled);

    CARAPI SetOnPatternListener(
       /* [in] */ IOnPatternListener* onPatternListener);

    CARAPI SetPattern(
       /* [in] */ DisplayMode displayMode,
       /* [in] */ IObjectContainer* pattern);

    CARAPI SetDisplayMode(
       /* [in] */ DisplayMode displayMode);

    CARAPI ClearPattern();

    CARAPI DisableInput();

    CARAPI EnableInput();

};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_INTERNAL_CLOCKPATTERNVIEW_H__
