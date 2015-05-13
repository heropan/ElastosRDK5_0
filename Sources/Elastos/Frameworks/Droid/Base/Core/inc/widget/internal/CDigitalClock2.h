#ifndef __INTERNALCDIGITALCLOCK_H__
#define __INTERNALCDIGITALCLOCK_H__

#include "_CDigitalClock2.h"
#include "DigitalClock.h"
#include "view/ViewMacro.h"

namespace Elastos{
namespace Droid{
namespace Widget{
namespace Internal{

CarClass(CDigitalClock2) , public DigitalClock
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
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);


    CARAPI UpdateTime();

    CARAPI SetIgnoreGravity(
        /* [in] */ Int32 viewId);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetHorizontalGravity(
        /* [in] */ Int32 horizontalGravity);

    CARAPI SetVerticalGravity(
        /* [in] */ Int32 verticalGravity);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__INTERNALCDIGITALCLOCK_H__
