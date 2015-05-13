#ifndef __CFRAMELAYOUTLAYOUTPARAMS_H_
#define __CFRAMELAYOUTLAYOUTPARAMS_H_

#include "_CFrameLayoutLayoutParams.h"
#include "widget/FrameLayoutLayoutParams.h"
#include "view/ViewGroupLayoutParamsMacro.h"

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

#endif //__CFRAMELAYOUTLAYOUTPARAMS_H_
