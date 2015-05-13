
#ifndef __CDROPZONE_H__
#define __CDROPZONE_H__

#include "_CDropZone.h"
#include "view/ViewMacro.h"
#include "systemui/statusbar/tablet/DropZone.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Tablet {

CarClass(CDropZone), public DropZone
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* pCtx,
        /* [in] */ IAttributeSet* pAttrs);

    CARAPI SetPocket(
        /* [in] */ IShirtPocket* p);
};

}// namespace Tablet
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif // __CDROPZONE_H__
