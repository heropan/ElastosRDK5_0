#ifndef __CVIEW_H_
#define __CVIEW_H_

#include "_CView.h"


namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CView), public Elastos::Droid::View::View
{
public:
    IVIEW_METHODS_DECL()
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

#endif //__CVIEW_H_
