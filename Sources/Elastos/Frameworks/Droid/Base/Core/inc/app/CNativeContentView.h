
#ifndef __CNATIVECONTENTVIEW_H__
#define __CNATIVECONTENTVIEW_H__

#include "_CNativeContentView.h"
#include "view/View.h"
#include "view/ViewMacro.h"

using Elastos::Droid::View::View;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CNativeContentView), public Elastos::Droid::View::View
{
public:
    IVIEW_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__CNATIVECONTENTVIEW_H__