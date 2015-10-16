#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_CROTARYSELECTOR_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_CROTARYSELECTOR_H__

#include "_Elastos_Droid_Widget_Internal_CRotarySelector.h"
#include "elastos/droid/widget/internal/RotarySelector.h"


namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CRotarySelector) , public RotarySelector
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CRotarySelector();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI SetLeftHandleResource(
        /* [in] */ Int32 resId);

    CARAPI SetRightHandleResource(
        /* [in] */ Int32 resId);

    CARAPI SetOnDialTriggerListener(
        /* [in] */ IOnDialTriggerListener* l);
};


}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
