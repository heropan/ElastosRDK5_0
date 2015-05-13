#ifndef __CROTARYSELECTOR_H__
#define __CROTARYSELECTOR_H__

#include "_CRotarySelector.h"
#include "widget/internal/RotarySelector.h"
#include "view/ViewMacro.h"

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