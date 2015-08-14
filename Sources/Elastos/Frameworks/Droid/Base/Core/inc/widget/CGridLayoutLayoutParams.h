#ifndef __ELASTOS_DROID_WIDGET_CGRIDLAYOUTLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_CGRIDLAYOUTLAYOUTPARAMS_H__

#include "_CGridLayoutLayoutParams.h"
#include "widget/GridLayoutLayoutParams.h"


namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CGridLayoutLayoutParams), public GridLayoutLayoutParams
{
public:

    IVIEWGROUPLP_METHODS_DECL()
    IVIEWGROUPMARGINLP_METHODS_DECL()

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ISpec* rowSpec,
        /* [in] */ ISpec* columnSpec);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams* params);

    CARAPI constructor(
        /* [in] */ IGridLayoutLayoutParams* that);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

    CARAPI Equals(
        /* [in] */ IInterface* layout,
        /* [out] */ Boolean* result);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif // __ELASTOS_DROID_WIDGET_CGRIDLAYOUTLAYOUTPARAMS_H__
