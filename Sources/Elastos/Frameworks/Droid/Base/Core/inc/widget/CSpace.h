#ifndef __CSPACE_H_
#define __CSPACE_H_

#include "_CSpace.h"
#include "view/ViewMacro.h"
#include "widget/Space.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CSpace), public Space
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

#endif //__CSPACE_H_
