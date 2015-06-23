#ifndef __CABSOLUTELAYOUT_H_
#define __CABSOLUTELAYOUT_H_

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

#endif //__CABSOLUTELAYOUT_H_
