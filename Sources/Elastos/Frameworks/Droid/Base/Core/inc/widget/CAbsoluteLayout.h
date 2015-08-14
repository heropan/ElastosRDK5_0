#ifndef __ELASTOS_DROID_WIDGET_CABSOLUTELAYOUT_H__
#define __ELASTOS_DROID_WIDGET_CABSOLUTELAYOUT_H__

#include "_CAbsoluteLayout.h"

#include "widget/AbsoluteLayout.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CAbsoluteLayout), public AbsoluteLayout
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
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

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CABSOLUTELAYOUT_H__
