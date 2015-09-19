#ifndef __ELASTOS_DROID_WIDGET_CFRAMELAYOUTLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_CFRAMELAYOUTLAYOUTPARAMS_H__

#include "_Elastos_Droid_Widget_CFrameLayoutLayoutParams.h"
#include "widget/FrameLayoutLayoutParams.h"


namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CFrameLayoutLayoutParams), public FrameLayoutLayoutParams
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
        /* [in] */ Int32 weight);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams* source);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CFRAMELAYOUTLAYOUTPARAMS_H__
