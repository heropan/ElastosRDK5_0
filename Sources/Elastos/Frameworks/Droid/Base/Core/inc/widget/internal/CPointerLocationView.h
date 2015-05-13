#ifndef __CPOINTERLOCATIONVIEW_H__
#define __CPOINTERLOCATIONVIEW_H__

#include "view/ViewMacro.h"
#include "_CPointerLocationView.h"
#include "widget/internal/PointerLocationView.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CPointerLocationView), public PointerLocationView
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CPointerLocationView();

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CPointerLocatonView();

    CARAPI SetPrintCoords(
        /* [in] */ Boolean state);

    CARAPI AddPointerEvent(
        /* [in] */ IMotionEvent* event);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif