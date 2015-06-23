#ifndef __CANIMATEDIMAGEVIEW_H__
#define __CANIMATEDIMAGEVIEW_H__

#include "_CAnimatedImageView.h"


#include "systemui/statusbar/AnimatedImageView.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {

CarClass(CAnimatedImageView), public AnimatedImageView
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
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);
};


}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__CANIMATEDIMAGEVIEW_H__
