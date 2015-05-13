#ifndef __CCOMPATMODEBUTTON_H_
#define __CCOMPATMODEBUTTON_H_

#include "_CCompatModeButton.h"
#include "view/ViewMacro.h"
#include "widget/ImageViewMacro.h"
#include "systemui/statusbar/policy/CompatModeButton.h"

using Elastos::Droid::Widget::ImageViewScaleType;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Policy {

CarClass(CCompatModeButton), public CompatModeButton
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IIMAGEVIEW_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI Refresh();

    CARAPI SetAlphaEx(
        /* [in] */ Int32 alpha);
};

}// namespace Policy
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__CCOMPATMODEBUTTON_H_
