
#ifndef __CSHIRTPOCKET_H__
#define __CSHIRTPOCKET_H__

#include "_CShirtPocket.h"
#include "view/ViewMacro.h"
#include "widget/ImageViewMacro.h"
#include "systemui/statusbar/tablet/ShirtPocket.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Tablet {

CarClass(CShirtPocket), public ShirtPocket
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
        /* [in] */ IContext* pCtx,
        /* [in] */ IAttributeSet* pAttrs);

    CARAPI constructor(
        /* [in] */ IContext* pContext,
        /* [in] */ IAttributeSet* pAttrs,
        /* [in] */ Int32 defStyle);

    CARAPI Holding(
        /* [out] */ Boolean* result);

    CARAPI Stash(
        /* [in] */ IClipData* clipping);

    CARAPI SetAlphaEx(
        /* [in] */ Int32 alpha);
};

}// namespace Tablet
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif // __CSHIRTPOCKET_H__
