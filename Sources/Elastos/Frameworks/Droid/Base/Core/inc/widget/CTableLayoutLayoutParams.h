#ifndef __CTABLELAYOUTLAYOUTPARAMS_H_
#define __CTABLELAYOUTLAYOUTPARAMS_H_

#include "_CTableLayoutLayoutParams.h"
#include "view/ViewGroupLayoutParamsMacro.h"
#include "widget/TableLayoutLayoutParams.h"
#include "widget/LinearLayoutMacro.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CTableLayoutLayoutParams), public TableLayoutLayoutParams
{
public:

    IVIEWGROUPLP_METHODS_DECL()
    IVIEWGROUPMARGINLP_METHODS_DECL()
    ILINEARLAYOUTLP_METHODS_DECL()

    CARAPI constructor();

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

#endif //__CTABLELAYOUTLAYOUTPARAMS_H_
