#ifndef __ELASTOS_DROID_WIDGET_CLINEARLAYOUTLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_CLINEARLAYOUTLAYOUTPARAMS_H__

#include "_Elastos_Droid_Widget_CLinearLayoutLayoutParams.h"
#include "elastos/droid/widget/LinearLayoutLayoutParams.h"


namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CLinearLayoutLayoutParams), public LinearLayoutLayoutParams
{
public:

    IVIEWGROUPLP_METHODS_DECL()
    IVIEWGROUPMARGINLP_METHODS_DECL()

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

    CARAPI GetWeight(
        /* [out] */ Float* weight);

    CARAPI SetWeight(
        /* [in] */ Float weight);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CLINEARLAYOUTLAYOUTPARAMS_H__
