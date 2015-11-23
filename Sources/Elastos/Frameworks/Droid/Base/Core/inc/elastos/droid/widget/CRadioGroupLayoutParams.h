#ifndef __ELASTOS_DROID_WIDGET_CRADIOGROUPLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_CRADIOGROUPLAYOUTPARAMS_H__

#include "_Elastos_Droid_Widget_CRadioGroupLayoutParams.h"

#include "elastos/droid/widget/RadioGroupLayoutParams.h"


namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CRadioGroupLayoutParams), public RadioGroupLayoutParams
{
public:

    IVIEWGROUPLP_METHODS_DECL()
    IVIEWGROUPMARGINLP_METHODS_DECL()
    ILINEARLAYOUTLP_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Float weight);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams* source);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CRADIOGROUPLAYOUTPARAMS_H__
