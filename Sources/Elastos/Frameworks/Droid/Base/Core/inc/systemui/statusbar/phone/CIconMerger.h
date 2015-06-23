#ifndef __CICONMERGER_H_
#define __CICONMERGER_H_

#include "_CIconMerger.h"



#include "systemui/statusbar/phone/IconMerger.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Phone {

CarClass(CIconMerger), public IconMerger
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ILINEARLAYOUT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetOverflowIndicator(
        /* [in] */ IView* v);
};

}// namespace Phone
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__CICONMERGER_H_
