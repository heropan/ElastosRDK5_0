
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_CWEIGHTEDLINEARLAYOUT_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_CWEIGHTEDLINEARLAYOUT_H__

#include "_Elastos_Droid_Widget_Internal_CWeightedLinearLayout.h"



#include "widget/internal/WeightedLinearLayout.h"


namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CWeightedLinearLayout), public WeightedLinearLayout
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
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
};

} // namespace Internal
} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_INTERNAL_CWEIGHTEDLINEARLAYOUT_H__
