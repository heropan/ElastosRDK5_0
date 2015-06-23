#ifndef __CLATESTITEMVIEW_H_
#define __CLATESTITEMVIEW_H_

#include "_CLatestItemView.h"



#include "systemui/statusbar/LatestItemView.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {

CarClass(CLatestItemView), public LatestItemView
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IFRAMELAYOUT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
};


}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__CLATESTITEMVIEW_H_
