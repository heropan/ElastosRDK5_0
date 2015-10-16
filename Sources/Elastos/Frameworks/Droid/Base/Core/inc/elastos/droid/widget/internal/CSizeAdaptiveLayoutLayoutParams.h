
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_CSIZEADAPTIVELAYOUTLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_CSIZEADAPTIVELAYOUTLAYOUTPARAMS_H__

#include "_Elastos_Droid_Widget_Internal_CSizeAdaptiveLayoutLayoutParams.h"
#include "elastos/droid/widget/internal/SizeAdaptiveLayoutLayoutParams.h"


namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CSizeAdaptiveLayoutLayoutParams), public SizeAdaptiveLayoutLayoutParams
{
public:
    IVIEWGROUPLP_METHODS_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 minHeight,
        /* [in] */ Int32 maxHeight);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);


    CARAPI SetMinHeight(
        /* [in] */ Int32 mh);

    CARAPI SetMaxHeight(
        /* [in] */ Int32 mh);

    CARAPI GetMinHeight(
        /* [out] */ Int32* mh);

    CARAPI GetMaxHeight(
        /* [out] */ Int32* mh);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_INTERNAL_CSIZEADAPTIVELAYOUTLAYOUTPARAMS_H__
