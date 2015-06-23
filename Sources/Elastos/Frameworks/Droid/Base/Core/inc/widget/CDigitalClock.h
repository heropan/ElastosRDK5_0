#ifndef __CDIGITALCLOCK__H_
#define __CDIGITALCLOCK__H_

#include "_CDigitalClock.h"
#include "widget/DigitalClock.h"



namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CDigitalClock), public DigitalClock
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ITEXTVIEW_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI OnPreDraw(
        /* [OUT] */ Boolean* result);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CDIGITALCLOCK__H_